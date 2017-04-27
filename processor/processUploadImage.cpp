#include "processUploadImage.h"
#include <boost/lexical_cast.hpp>
#include <json/jsonParser.h>
#include <templateServer/templateServerProxy.h>
#include <dataLayer/dataLayer.h>
#include <common/commonFunction.h>
#include <syscfg/sysConfig.h>
#include <sys/time.h>
#include <boost/lexical_cast.hpp>
#include "../syscfg/sysConfig.h"
#include "../common/commonFunction.h"
#include "../tool/dir.h"
#include "../tool/base64/base64.h"
#include <fstream>
#include <boost/scoped_array.hpp>
#include "../tool/uuid.h"
#include <boost/algorithm/string.hpp>

static const std::string strAlarmThreshold = "ALARM_THRESHOLD";        //告警阈值类型
static const std::string strContrastThreshold = "CONTRAST_THRESHOLD";  //比对阈值类型

void ProcessUploadImage::process( const HttpRequest& req, HttpResponse& resp )
{
	Json::Value respJson;
	ImageInfo imageInfo;

	if (!CJsonParser::parseUploadImageJson(req.httpBody, imageInfo))
	{
		CLogger::instance()->write_log(LOG_LEVEL_ERR, "uploadImage:解析包体json失败. body: %s", req.httpBody.c_str());
		respJson["code"] = 1;
		respJson["message"] = "invalid body json parameter";
		resp.bSuccess = true;
		resp.httpBody = respJson.toStyledString().c_str();
		return;
	}

	//save image
	std::string strPath, strFileName;
	if (!getImageFilePath(imageInfo.camerId, strPath, strFileName))
	{
		CLogger::instance()->write_log(LOG_LEVEL_ERR, "uploadImage:生成图片路径失败:%s", imageInfo.camerId.c_str());
		respJson["code"] = 1;
		respJson["message"] = "generate file path failed";
		resp.bSuccess = true;
		resp.httpBody = respJson.toStyledString().c_str();
		return;
	}

	if (!saveImageFile(strPath, strFileName, imageInfo.imageStr))
	{
		CLogger::instance()->write_log(LOG_LEVEL_ERR, "uploadImage:保存本地图片失败:%s", (strPath+"/"+strFileName).c_str());
		respJson["code"] = 1;
		respJson["message"] = "Save local image failed";
		resp.bSuccess = true;
		resp.httpBody = respJson.toStyledString().c_str();
		return;
	}
	CLogger::instance()->write_log(LOG_LEVEL_INFO, "uploadImage:保存本地图片成功:%s", (strPath + "/" + strFileName).c_str());
	imageInfo.photoPath = strPath + "/" + strFileName;
	imageInfo.sourceId = generate_uuid_string();

	//1.add template
	/*
	TemplateInfo templateInfo;
	time_t monitorTime = imageInfo.monitorTime;
	tm* timeInfo = localtime(&monitorTime);
	templateInfo.createTime = time2Str(timeInfo, 1);
	templateInfo.gender = imageInfo.gender;
	templateInfo.groupId = CSysConfig::instance().m_processConfig.m_defaultGroupId;
	templateInfo.imageStr = imageInfo.imageStr;

	CLogger::instance()->write_log(LOG_LEVEL_INFO, "uploadImage:调用添加模板请求:createTime=%s, gender=%d, groupId=%s",
		templateInfo.createTime.c_str(), templateInfo.gender, templateInfo.groupId.c_str());

	AddTemplateResp addTemplateResp;
	if (!TemplateServerProxy::addTemplate(templateInfo, addTemplateResp))
	{
		CLogger::instance()->write_log(LOG_LEVEL_ERR, "uploadImage:添加模板失败:%s", addTemplateResp.errorMsg.c_str());
		respJson["code"] = 1;
		respJson["message"] = addTemplateResp.errorMsg.c_str();
		resp.bSuccess = true;
		resp.httpBody = respJson.toStyledString().c_str();
		return;
	}
		
	imageInfo.templateId = boost::lexical_cast<std::string>(addTemplateResp.id);
	*/	
	if (!DataLayer::getLocationId(imageInfo.camerId, imageInfo.locationId)
		|| imageInfo.locationId.empty())
	{
		CLogger::instance()->write_log(LOG_LEVEL_ERR, "uploadImage:获取locationId失败");
		respJson["code"] = 1;
		respJson["message"] = "get locationId failed";
		resp.bSuccess = true;
		resp.httpBody = respJson.toStyledString().c_str();
		return;
	}

	//2.save to db
	if (!DataLayer::saveImage(imageInfo))
	{
		CLogger::instance()->write_log(LOG_LEVEL_ERR, "uploadImage:图片信息入库失败");
		respJson["code"] = 1;
		respJson["message"] = "save image to db failed";
		resp.bSuccess = true;
		resp.httpBody = respJson.toStyledString().c_str();
		return;
	}

	CLogger::instance()->write_log(LOG_LEVEL_INFO, "uploadImage:保存图片信息到数据库成功,locationId=%s, photoPath=%s",
		    imageInfo.locationId.c_str(), imageInfo.photoPath.c_str());

	//3.动态1:N
	DynamicOneToNReq oneToNReq;
	oneToNReq.sourceId = imageInfo.sourceId;
	oneToNReq.pic = imageInfo.imageStr;

	AlarmParam alarmParam;
	if (!DataLayer::getAlarmParam(alarmParam))
	{
		CLogger::instance()->write_log(LOG_LEVEL_ERR, "uploadImage:获取告警参数失败");
		respJson["code"] = 1;
		respJson["message"] = "get alarm parameter failed";
		resp.bSuccess = true;
		resp.httpBody = respJson.toStyledString().c_str();
		return;
	}

	if (alarmParam.groupIds.empty())
	{
		CLogger::instance()->write_log(LOG_LEVEL_DEBUG, "uploadImage:未获取到告警参数");
		respJson["code"] = HTTP_SUCCESS;
		respJson["message"] = "success";
		resp.bSuccess = true;
		resp.httpBody = respJson.toStyledString().c_str();
		return;
	}
	oneToNReq.count = alarmParam.maxReturnNumber;
	oneToNReq.groupIds = boost::algorithm::join(alarmParam.groupIds, ",");
	std::string strType = "ALARM_THRESHOLD";
	if (strType == strAlarmThreshold)
	{
		oneToNReq.threshold = alarmParam.alarmThreshold;      //告警阈值
		strType = "CONTRAST_THRESHOLD";
		DynamicOneToNResp oneToNResp;
		if (!TemplateServerProxy::dynamicOneToN(oneToNReq, oneToNResp))
		{
			CLogger::instance()->write_log(LOG_LEVEL_ERR, "uploadImage:调用动态1:N失败:%s", oneToNResp.errorMsg.c_str());
			respJson["code"] = 1;
			respJson["message"] = oneToNResp.errorMsg.c_str();
			resp.bSuccess = true;
			resp.httpBody = respJson.toStyledString().c_str();
		}
		else
		{
			CLogger::instance()->write_log(LOG_LEVEL_INFO, "uploadImage: ALARM_THRESHOLD, req sourceID=%s, resp sourceID=%s", 
				oneToNReq.sourceId.c_str(), oneToNResp.sourceID.c_str());
			//比对阈值，然后入告警库
			//4.入告警库
			typedef list<Matche>::iterator IT;
			for (IT it = oneToNResp.listMatches.begin(); it != oneToNResp.listMatches.end(); ++it)
			{
				if (it->score > oneToNReq.threshold)
				{
					SuspectAlarm suspectAlarm;
					suspectAlarm.monitorId = imageInfo.sourceId;

					if (!DataLayer::getFaceId(boost::lexical_cast<std::string>(it->id), suspectAlarm.faceId))
					{
						CLogger::instance()->write_log(LOG_LEVEL_ERR, "uploadImage:获取faceId失败");
						continue;
					}

					if (!DataLayer::getAlarmAddress(imageInfo.sourceId, suspectAlarm.alarmAddress))
					{
						CLogger::instance()->write_log(LOG_LEVEL_ERR, "uploadImage:获取alarmAddress失败");
						continue;
					}
					suspectAlarm.ID = generate_uuid_string();      //告警表ID自动生成uuid
					suspectAlarm.alarmTime = imageInfo.monitorTime;
					suspectAlarm.similarity = it->score;
					suspectAlarm.suspectState = "1"; // 1：未处理 2：已处理
					suspectAlarm.suspectType = "1"; // 1：布控自动告警 2：人工确认告警 3：人工比对告警

					if (!DataLayer::saveSuspectAlarm(suspectAlarm))
					{
						CLogger::instance()->write_log(LOG_LEVEL_ERR, "uploadImage:告警信息入库失败");
						continue;
					}

					CLogger::instance()->write_log(LOG_LEVEL_DEBUG, "uploadImage:告警信息入库成功monitorId=%s, faceId=%s, alarmAddress=%s",
						suspectAlarm.monitorId.c_str(), suspectAlarm.faceId.c_str(), suspectAlarm.alarmAddress.c_str());
					respJson["code"] = HTTP_SUCCESS;
					respJson["message"] = "success";
					resp.bSuccess = true;
					resp.httpBody = respJson.toStyledString().c_str();
				}
			}
		}	
	}
	if(strType == strContrastThreshold)
	{
		oneToNReq.threshold = alarmParam.contrastthreshold;   //比对阈值
		//strType = "CONTRAST_THRESHOLD";
		DynamicOneToNResp oneToNResp;
		if (!TemplateServerProxy::dynamicOneToN(oneToNReq, oneToNResp))
		{
			CLogger::instance()->write_log(LOG_LEVEL_ERR, "uploadImage:调用动态1:N失败:%s", oneToNResp.errorMsg.c_str());
			respJson["code"] = 1;
			respJson["message"] = oneToNResp.errorMsg.c_str();
			resp.bSuccess = true;
			resp.httpBody = respJson.toStyledString().c_str();
		}
		else
		{
			CLogger::instance()->write_log(LOG_LEVEL_INFO, "uploadImage: CONTRAST_THRESHOLD, req sourceID=%s, resp sourceID=%s",
				oneToNReq.sourceId.c_str(), oneToNResp.sourceID.c_str());
			//比对阈值，然后入告警库
			//4.入告警库
			typedef list<Matche>::iterator IT;
			for (IT it = oneToNResp.listMatches.begin(); it != oneToNResp.listMatches.end(); ++it)
			{
				if (it->score > oneToNReq.threshold)
				{
					SuspectAlarm suspectAlarm;
					suspectAlarm.monitorId = imageInfo.sourceId;

					if (!DataLayer::getFaceId(boost::lexical_cast<std::string>(it->id), suspectAlarm.faceId))
					{
						CLogger::instance()->write_log(LOG_LEVEL_ERR, "uploadImage:获取faceId失败");
						continue;
					}

					if (!DataLayer::getAlarmAddress(imageInfo.sourceId, suspectAlarm.alarmAddress))
					{
						CLogger::instance()->write_log(LOG_LEVEL_ERR, "uploadImage:获取alarmAddress失败");
						continue;
					}
					suspectAlarm.ID = generate_uuid_string();      //告警表ID自动生成uuid
					suspectAlarm.alarmTime = imageInfo.monitorTime;
					suspectAlarm.similarity = it->score;
					suspectAlarm.suspectState = "1"; // 1：未处理 2：已处理
					suspectAlarm.suspectType = "1"; // 1：布控自动告警 2：人工确认告警 3：人工比对告警

					if (!DataLayer::saveSuspectAlarm(suspectAlarm))
					{
						CLogger::instance()->write_log(LOG_LEVEL_ERR, "uploadImage:告警信息入库失败");
						continue;
					}

					CLogger::instance()->write_log(LOG_LEVEL_DEBUG, "uploadImage:告警信息入库成功monitorId=%s, faceId=%s, alarmAddress=%s",
						suspectAlarm.monitorId.c_str(), suspectAlarm.faceId.c_str(), suspectAlarm.alarmAddress.c_str());
					respJson["code"] = HTTP_SUCCESS;
					respJson["message"] = "success";
					resp.bSuccess = true;
					resp.httpBody = respJson.toStyledString().c_str();
				}
			}
		}	
	}
}

bool ProcessUploadImage::getImageFilePath(const std::string& camerCode, std::string & strPath, std::string & strFileName)
{
	std::string strRegionCode;
	std::string strAreaCode;
	std::string strLocationCode;

	if (!DataLayer::getImageFilePathInfo(camerCode, strRegionCode, strAreaCode, strLocationCode))
	{
		return false;
	}

	struct timeval tv;
	gettimeofday(&tv, NULL);
	uint64_t _misc = tv.tv_sec * 1000 + tv.tv_usec / 1000;
	strFileName = camerCode + "_" + boost::lexical_cast<std::string>(_misc) + ".jpeg";

	time_t _time = time(NULL);
	tm* timeInfo = localtime(&_time);
	strPath = CSysConfig::instance().m_storageConfig.m_rootPath + "/" + time2Str(timeInfo, 8) + "/"
		+ strRegionCode + "/" + strAreaCode + "/" + strLocationCode;

	return true;
}

bool ProcessUploadImage::saveImageFile(const std::string strPath, const std::string &strFileName,
	const std::string& content_base64)
{
	std::string strFullName = strPath + "/" + strFileName;
	createDir(strPath.c_str());

	boost::scoped_array<uint8_t> buffer(new uint8_t[content_base64.length()]);
	if (!buffer)
	{
		return false;
	}

	int len = CBase64::DecodeBase64(reinterpret_cast<const uint8_t*>(content_base64.c_str()), buffer.get(),
		content_base64.length());
	ofstream out(strFullName.c_str(), ios::binary);
	if (!out)
	{
		return false;
	}

	out.write(reinterpret_cast<const char*>(buffer.get()), len);
	bool bRet = out.good();
	out.close();
	return bRet;
}
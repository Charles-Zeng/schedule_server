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
	getImageFilePath(strPath, strFileName);
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

	//1.add template
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
		
	if (!DataLayer::getLocationId(imageInfo.camerId, imageInfo.locationId))
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
	oneToNReq.sourceId = "souceId"; // how to get??
	oneToNReq.pic = imageInfo.imageStr;

	AlarmParam alarmParam;
	if (!DataLayer::getOneToNGroupIds(oneToNReq.groupIds) || !DataLayer::getAlarmParam(alarmParam))
	{
		CLogger::instance()->write_log(LOG_LEVEL_ERR, "uploadImage:获取告警参数失败");
		respJson["code"] = 1;
		respJson["message"] = "get alarm parameter failed";
		resp.bSuccess = true;
		resp.httpBody = respJson.toStyledString().c_str();
		return;
	}
	
	oneToNReq.threshold = alarmParam.alarmThreshold;
	oneToNReq.count = alarmParam.maxReturnNumber;

	DynamicOneToNResp oneToNResp;
	if (!TemplateServerProxy::dynamicOneToN(oneToNReq, oneToNResp))
	{
		CLogger::instance()->write_log(LOG_LEVEL_ERR, "uploadImage:调用动态1:N失败:%s", oneToNResp.errorMsg.c_str());
		respJson["code"] = 1;
		respJson["message"] = oneToNResp.errorMsg.c_str();
		resp.bSuccess = true;
		resp.httpBody = respJson.toStyledString().c_str();
		return;
	}

	//4.入告警库
	typedef list<Matche>::iterator IT;
	for (IT it = oneToNResp.listMatches.begin(); it != oneToNResp.listMatches.end(); ++it)
	{
		SuspectAlarm suspectAlarm;
		suspectAlarm.faceId = "";
		suspectAlarm.monitorId = "";
		suspectAlarm.alarmTime = time(NULL);
		suspectAlarm.alarmAddress = "";
		suspectAlarm.similarity = it->score;
		suspectAlarm.suspectState = "1"; // 1：未处理 2：已处理
		suspectAlarm.suspectType = "1"; // 1：布控自动告警 2：人工确认告警 3：人工比对告警

		if (!DataLayer::saveSuspectAlarm(suspectAlarm))
		{
			CLogger::instance()->write_log(LOG_LEVEL_ERR, "uploadImage:告警信息入库失败");
			continue;
		}
		
	}

	respJson["code"] = HTTP_SUCCESS;
	respJson["message"] = "success";
	resp.bSuccess = true;
	resp.httpBody = respJson.toStyledString().c_str();
}

void ProcessUploadImage::getImageFilePath(std::string & strPath, std::string & strFileName)
{
	std::string strRegionCode;
	std::string strAreaCode;
	std::string strLocationCode;

	struct timeval tv;
	gettimeofday(&tv, NULL);
	uint64_t _misc = tv.tv_sec * 1000 + tv.tv_usec / 1000;
	strFileName = std::string("ZDJKRYK_") + boost::lexical_cast<std::string>(_misc) + ".jpeg";

	time_t _time = time(NULL);
	tm* timeInfo = localtime(&_time);
	strPath = CSysConfig::instance().m_storageConfig.m_rootPath + "/" + time2Str(timeInfo, 8) + "/"
		+ strRegionCode + "/" + strAreaCode + "/" + strLocationCode;
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
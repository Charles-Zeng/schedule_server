#include "processUploadImage.h"
#include <json/jsonParser.h>
#include <templateServer/templateServerProxy.h>
#include <dataLayer/dataLayer.h>
#include <common/commonFunction.h>
#include <syscfg/sysConfig.h>

void ProcessUploadImage::process( const HttpRequest& req, HttpResponse& resp )
{
	ImageInfo imageInfo;

	if (!CJsonParser::parseUploadImageJson(req.httpBody, imageInfo))
	{
		CLogger::instance()->write_log(LOG_LEVEL_ERR, "uploadImage: 解析包体json失败. body: %s", req.httpBody.c_str());
		return;
	}

	//1.add template
	TemplateInfo templateInfo;
	time_t monitorTime = imageInfo.monitorTime;
	tm* timeInfo = localtime(&monitorTime);
	templateInfo.createTime = time2Str(timeInfo, 1);
	templateInfo.gender = imageInfo.gender;
	templateInfo.groupId = CSysConfig::instance().m_processConfig.m_defaultGroupId;
	templateInfo.imageStr = imageInfo.imageStr;

	AddTemplateResp addTemplateResp;
	TemplateServerProxy::addTemplate(templateInfo, addTemplateResp);

	imageInfo.templateId = addTemplateResp.id;

	//2.save to db
	DataLayer::saveImage(imageInfo);

	//3.动态1:N
	DynamicOneToNReq oneToNReq;
	oneToNReq.sourceId = ""; // how to get??
	oneToNReq.pic = imageInfo.imageStr;
	oneToNReq.groupIds = DataLayer::getOneToNGroupIds();
	AlarmParam alarmParam = DataLayer::getAlarmParam();
	oneToNReq.threshold = alarmParam.alarmThreshold;
	oneToNReq.count = alarmParam.maxReturnNumber;

	DynamicOneToNResp oneToNResp;
	TemplateServerProxy::dynamicOneToN(oneToNReq, oneToNResp);
	//4.入告警库
}


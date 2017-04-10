#include "processUploadImage.h"
#include <json/jsonParser.h>
#include <templateServer/templateServerProxy.h>
#include <dataLayer/dataLayer.h>

void ProcessUploadImage::process( const HttpRequest& req, HttpResponse& resp )
{
	ImageInfo imageInfo;

	if (!CJsonParser::parseUploadImageJson(req.httpBody, imageInfo))
	{
		CLogger::instance()->write_log(LOG_LEVEL_ERR, "uploadImage: parse json body error. body: %s", req.httpBody.c_str());
		return;
	}

	//1.add template
	TemplateServerProxy::addTemplate(imageInfo);

	//2.save to db
	DataLayer::saveImage(imageInfo);

	//3.¶¯Ì¬1:N
	DynamicOneToNReq oneToNReq;
	oneToNReq.sourceId = ""; // how to get??
	oneToNReq.pic = imageInfo.imageStr;
	oneToNReq.groupIds = DataLayer::getOneToNGroupIds();
	AlarmParam alarmParam = DataLayer::getAlarmParam();
	oneToNReq.threshold = alarmParam.alarmThreshold;
	oneToNReq.count = alarmParam.maxReturnNumber;

	DynamicOneToNResp oneToNResp;
	TemplateServerProxy::dynamicOneToN(oneToNReq, oneToNResp);
	//4.Èë¸æ¾¯¿â
}


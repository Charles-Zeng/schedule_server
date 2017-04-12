#include "processOneToOne.h"

#include <json/jsonParser.h>
#include <templateServer/templateServerProxy.h>

void ProcessOneToOne::process( const HttpRequest& req, HttpResponse& resp )
{
	Json::Value respJson;
	OneToOneInfo oneToOneInfo;

	if (!CJsonParser::parseOneToOne(req.httpBody, oneToOneInfo))
	{
		CLogger::instance()->write_log(LOG_LEVEL_ERR, "oneToOne: 解析包体json失败. body: %s", req.httpBody.c_str());
		respJson["code"] = 1;
		respJson["message"] = "invalid body json";
		resp.bSuccess = true;
		resp.httpBody = respJson.toStyledString();
		return;
	}

	GetFaceInfoResp getFaceInfoResp;
	if (!TemplateServerProxy::getFaceInfo(oneToOneInfo.pic1Base64, getFaceInfoResp))
	{
		CLogger::instance()->write_log(LOG_LEVEL_ERR, "oneToOne: 获取人脸信息失败: %s", getFaceInfoResp.errorMsg.c_str());
		respJson["code"] = 1;
		respJson["message"] = getFaceInfoResp.errorMsg;
		resp.bSuccess = true;
		resp.httpBody = respJson.toStyledString();
		return;
	}

	FaceInfo pic1FaceInfo = getFaceInfoResp.faceInfo;

	if (!TemplateServerProxy::getFaceInfo(oneToOneInfo.pic2Base64, getFaceInfoResp))
	{
		CLogger::instance()->write_log(LOG_LEVEL_ERR, "oneToOne: 获取人脸信息失败: %s", getFaceInfoResp.errorMsg.c_str());
		respJson["code"] = 1;
		respJson["message"] = getFaceInfoResp.errorMsg;
		resp.bSuccess = true;
		resp.httpBody = respJson.toStyledString();
		return;
	}

	FaceInfo pic2FaceInfo = getFaceInfoResp.faceInfo;

	OneToOneInfo oneToOneReq;
	oneToOneReq.pic1Base64 = pic1FaceInfo.facePic;
	oneToOneReq.pic2Base64 = pic2FaceInfo.facePic;
	OneToOneResp oneToOneResp;
	if (!TemplateServerProxy::oneToOne(oneToOneReq, oneToOneResp))
	{
		CLogger::instance()->write_log(LOG_LEVEL_ERR, "oneToOne: 1:1比对失败: %s", oneToOneResp.errorMsg.c_str());
		respJson["code"] = 1;
		respJson["message"] = oneToOneResp.errorMsg;
		resp.bSuccess = true;
		resp.httpBody = respJson.toStyledString();
		return;
	}

	respJson["code"] = 0;
	respJson["message"] = "success";
	respJson["score"] = oneToOneResp.score;
	resp.bSuccess = true;
	resp.httpBody = respJson.toStyledString();
}


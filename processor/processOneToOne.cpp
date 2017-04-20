#include "processOneToOne.h"

#include <json/jsonParser.h>
#include <templateServer/templateServerProxy.h>

void ProcessOneToOne::process( const HttpRequest& req, HttpResponse& resp )
{
	Json::Value respJson;
	OneToOneInfo oneToOneInfo;
	//ͼƬ̫��,Ԥ�ȷ����ڴ�
	//oneToOneInfo.pic1Base64.reserve(100 * 1024);
	//oneToOneInfo.pic2Base64.reserve(100 * 1024);

	if (!CJsonParser::parseOneToOne(req.httpBody, oneToOneInfo))
	{

		CLogger::instance()->write_log(LOG_LEVEL_ERR, "oneToOne: ��������jsonʧ��.");
		respJson["code"] = 1;
		respJson["message"] = "invalid body json";
		resp.bSuccess = true;
		resp.httpBody = respJson.toStyledString().c_str();
		return;
	}
	/*
	GetFaceInfoResp getFaceInfoResp;
	if (!TemplateServerProxy::getFaceInfo(oneToOneInfo.pic1Base64, getFaceInfoResp))
	{
		CLogger::instance()->write_log(LOG_LEVEL_ERR, "oneToOne: ��ȡ������Ϣʧ��: %s", getFaceInfoResp.errorMsg.c_str());
		respJson["code"] = 1;
		respJson["message"] = getFaceInfoResp.errorMsg.c_str();
		resp.bSuccess = true;
		resp.httpBody = respJson.toStyledString().c_str();
		return;
	}

	FaceInfo pic1FaceInfo = getFaceInfoResp.faceInfo;

	if (!TemplateServerProxy::getFaceInfo(oneToOneInfo.pic2Base64, getFaceInfoResp))
	{
		CLogger::instance()->write_log(LOG_LEVEL_ERR, "oneToOne: ��ȡ������Ϣʧ��: %s", getFaceInfoResp.errorMsg.c_str());
		respJson["code"] = 1;
		respJson["message"] = getFaceInfoResp.errorMsg.c_str();
		resp.bSuccess = true;
		resp.httpBody = respJson.toStyledString().c_str();
		return;
	}

	FaceInfo pic2FaceInfo = getFaceInfoResp.faceInfo;
	*/
	OneToOneInfo oneToOneReq;
	oneToOneReq.pic1Base64 = oneToOneInfo.pic1Base64;
	oneToOneReq.pic2Base64 = oneToOneInfo.pic2Base64;
	OneToOneResp oneToOneResp;
	if (!TemplateServerProxy::oneToOne(oneToOneReq, oneToOneResp))
	{
		CLogger::instance()->write_log(LOG_LEVEL_ERR, "oneToOne: 1:1�ȶ�ʧ��: %s", oneToOneResp.errorMsg.c_str());
		respJson["code"] = 1;
		respJson["message"] = oneToOneResp.errorMsg.c_str();
		resp.bSuccess = true;
		resp.httpBody = respJson.toStyledString().c_str();
		return;
	}

	respJson["code"] = HTTP_SUCCESS;
	respJson["message"] = "success";
	respJson["score"] = oneToOneResp.score;
	resp.bSuccess = true;
	resp.httpBody = respJson.toStyledString().c_str();
}


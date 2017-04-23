#include "processGetFaceInfo.h"

#include <json/jsonParser.h>
#include <json/json.h>
#include <common/dataStruct.h>
#include <templateServer/templateServerProxy.h>
#include <boost/lexical_cast.hpp>

void ProcessGetFaceInfo::process(const HttpRequest& req, HttpResponse& resp)
{
	Json::Value respJson;
	GetFaceInfoReq getfaceinfo;
	if (!CJsonParser::parseGetFaceInfo(req.httpBody, getfaceinfo))
	{
		CLogger::instance()->write_log(LOG_LEVEL_ERR, "getFaceInfo: 解析包体json失败. body: %s", req.httpBody.c_str());
		respJson["code"] = 1;
		respJson["message"] = "invalid body json";
		resp.bSuccess = true;
		resp.httpBody = respJson.toStyledString().c_str();
		return;
	}

	GetFaceInfoResp getFaceInfoResp;
	if (!TemplateServerProxy::getFaceInfo(getfaceinfo.picBase64, getFaceInfoResp))
	{
		CLogger::instance()->write_log(LOG_LEVEL_ERR, "getFaceInfo: 获取人脸信息失败: %s", getFaceInfoResp.errorMsg.c_str());
		respJson["code"] = 1;
		respJson["message"] = getFaceInfoResp.errorMsg.c_str();
		resp.bSuccess = true;
		resp.httpBody = respJson.toStyledString().c_str();
		return;
	}

	respJson["code"] = HTTP_SUCCESS;
	respJson["message"] = "success";
	//返回请求接口中的json体 strRespJson
	respJson["faceInfos"] = getFaceInfoResp.strRespJson;
	resp.bSuccess = true;
	resp.httpBody = respJson.toStyledString().c_str();
}


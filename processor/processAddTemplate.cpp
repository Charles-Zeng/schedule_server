#include "processAddTemplate.h"

#include <json/jsonParser.h>
#include <json/json.h>
#include <common/dataStruct.h>
#include <templateServer/templateServerProxy.h>

void ProcessAddTemplate::process( const HttpRequest& req, HttpResponse& resp )
{
	Json::Value respJson;

	TemplateInfo templateInfo;

	if (!CJsonParser::parseAddTemplate(req.httpBody, templateInfo))
	{
		CLogger::instance()->write_log(LOG_LEVEL_ERR, "addTemplate: 解析包体json失败. body: %s", req.httpBody.c_str());
		respJson["code"] = 1;
		respJson["message"] = "invalid body json";
		resp.bSuccess = true;
		resp.httpBody = respJson.toStyledString().c_str();
		return;
	}

	AddTemplateResp addTemplateResp;
	if (!TemplateServerProxy::addTemplate(templateInfo, addTemplateResp))
	{
		CLogger::instance()->write_log(LOG_LEVEL_ERR, "addTemplate: 添加模板失败: %s", addTemplateResp.errorMsg.c_str());
		respJson["code"] = 1;
		respJson["message"] = addTemplateResp.errorMsg.c_str();
		resp.bSuccess = true;
		resp.httpBody = respJson.toStyledString().c_str();
		return;
	}
	

	respJson["code"] = HTTP_SUCCESS;
	respJson["message"] = "success";
	respJson["templateID"] = addTemplateResp.id;
	resp.bSuccess = true;
	resp.httpBody = respJson.toStyledString().c_str();
}


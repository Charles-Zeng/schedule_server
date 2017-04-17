#include "processDelTemplate.h"

#include <json/jsonParser.h>
#include <templateServer/templateServerProxy.h>

void ProcessDelTemplate::process( const HttpRequest& req, HttpResponse& resp )
{
	Json::Value respJson;
	int32_t templateId;

	if (!CJsonParser::parseDelTemplate(req.httpBody, templateId))
	{
		CLogger::instance()->write_log(LOG_LEVEL_ERR, "delTemplate: ½âÎö°üÌåjsonÊ§°Ü. body: %s", req.httpBody.c_str());
		respJson["code"] = 1;
		respJson["message"] = "invalid body json";
		resp.bSuccess = true;
		resp.httpBody = respJson.toStyledString().c_str();
		return;
	}

	DelTemplateResp delTemplateResp;
	if (!TemplateServerProxy::delTemplate(templateId, delTemplateResp))
	{
		CLogger::instance()->write_log(LOG_LEVEL_ERR, "delTemplate: É¾³ýÄ£°åIDÊ§°Ü: %s", delTemplateResp.errorMsg.c_str());
		respJson["code"] = 1;
		respJson["message"] = delTemplateResp.errorMsg.c_str();
		resp.bSuccess = true;
		resp.httpBody = respJson.toStyledString().c_str();
		return;
	}

	respJson["code"] = HTTP_SUCCESS;
	respJson["message"] = "success";
	resp.bSuccess = true;
	resp.httpBody = respJson.toStyledString().c_str();
}


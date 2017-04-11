#include "processAddTemplate.h"

#include <json/jsonParser.h>
#include <json/json.h>
#include <common/dataStruct.h>
#include <templateServer/templateServerProxy.h>

void ProcessAddTemplate::process( const HttpRequest& req, HttpResponse& resp )
{
	Json::Value respBodyJson;

	TemplateInfo templateInfo;

	if (!CJsonParser::parseAddTemplate(req.httpBody, templateInfo))
	{
		respBodyJson["code"] = 1;
		respBodyJson["message"] = "invalid body json";
		resp.bSuccess = false;
		resp.httpBody = respBodyJson.toStyledString();
		return;
	}

	AddTemplateResp addTemplateResp;
	TemplateServerProxy::addTemplate(templateInfo, addTemplateResp);

	respBodyJson["code"] = addTemplateResp.code;
	respBodyJson["message"] = addTemplateResp.errorMsg;
	resp.bSuccess = true;
	resp.httpBody = respBodyJson.toStyledString();
}


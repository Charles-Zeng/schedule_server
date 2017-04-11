#include "processDelGroup.h"

#include <json/jsonParser.h>
#include <json/json.h>
#include <templateServer/templateServerProxy.h>

void ProcessDelGroup::process( const HttpRequest& req, HttpResponse& resp )
{
	Json::Value respBodyJson;

	int64_t groupId;

	if (!CJsonParser::parseDelGroup(req.httpBody, groupId))
	{
		respBodyJson["code"] = 1;
		respBodyJson["message"] = "invalid body json";
		resp.bSuccess = false;
		resp.httpBody = respBodyJson.toStyledString();
		return;
	}

	DelGroupResp delGroupResp;
	TemplateServerProxy::delGroupId(groupId, delGroupResp);

	respBodyJson["code"] = delGroupResp.code;
	respBodyJson["message"] = delGroupResp.errorMsg;
	resp.bSuccess = true;
	resp.httpBody = respBodyJson.toStyledString();
}


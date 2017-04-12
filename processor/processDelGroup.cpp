#include "processDelGroup.h"

#include <json/jsonParser.h>
#include <json/json.h>
#include <templateServer/templateServerProxy.h>

void ProcessDelGroup::process( const HttpRequest& req, HttpResponse& resp )
{
	Json::Value respJson;
	int64_t groupId;

	if (!CJsonParser::parseDelGroup(req.httpBody, groupId))
	{
		CLogger::instance()->write_log(LOG_LEVEL_ERR, "delGroup: ½âÎö°üÌåjsonÊ§°Ü. body: %s", req.httpBody.c_str());
		respJson["code"] = 1;
		respJson["message"] = "invalid body json";
		resp.bSuccess = true;
		resp.httpBody = respJson.toStyledString();
		return;
	}

	DelGroupResp delGroupResp;
	if (!TemplateServerProxy::delGroupId(groupId, delGroupResp))
	{
		CLogger::instance()->write_log(LOG_LEVEL_ERR, "delGroup: É¾³ý¿âIDÊ§°Ü: %s", delGroupResp.errorMsg.c_str());
		respJson["code"] = 1;
		respJson["message"] = delGroupResp.errorMsg;
		resp.bSuccess = true;
		resp.httpBody = respJson.toStyledString();
		return;
	}

	respJson["code"] = delGroupResp.code;
	respJson["message"] = delGroupResp.errorMsg;
	resp.bSuccess = true;
	resp.httpBody = respJson.toStyledString();
}


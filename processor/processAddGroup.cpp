#include "processAddGroup.h"
#include <json/json.h>
#include <json/jsonParser.h>
#include <templateServer/templateServerProxy.h>
#include <set>

void ProcessAddGroup::process( const HttpRequest& req, HttpResponse& resp )
{
	Json::Value respJson;
	std::string groupName;
	if (!CJsonParser::parseAddGroup(req.httpBody, groupName))
	{
		CLogger::instance()->write_log(LOG_LEVEL_ERR, "addGroup: 解析包体json失败. body: %s", req.httpBody.c_str());
		respJson["code"] = 1;
		respJson["message"] = "invalid body json parameter";
		resp.bSuccess = true;
		resp.httpBody = respJson.toStyledString().c_str();
		return;
	}

	GetGroupIdResp getGroupIdResp;
	if (!TemplateServerProxy::getGroupIdInfos(getGroupIdResp))
	{
		CLogger::instance()->write_log(LOG_LEVEL_ERR, "addGroup: 获取库ID信息失败: %s", getGroupIdResp.errorMsg.c_str());
		respJson["code"] = 1;
		respJson["message"] = getGroupIdResp.errorMsg.c_str();
		resp.bSuccess = true;
		resp.httpBody = respJson.toStyledString().c_str();
		return;
	}

	int newGroupId = getNewGroupId(getGroupIdResp.groupIdInfos);

	GroupIdInfo newGroupIdInfo;
	newGroupIdInfo.id = newGroupId;
	newGroupIdInfo.name = groupName;

	AddGroupResp addGroupResp;
	if (!TemplateServerProxy::addGroupId(newGroupIdInfo, addGroupResp))
	{
		CLogger::instance()->write_log(LOG_LEVEL_ERR, "addGroup: 添加库ID失败: %s", addGroupResp.errorMsg.c_str());
		respJson["code"] = 1;
		respJson["message"] = addGroupResp.errorMsg.c_str();
		resp.bSuccess = true;
		resp.httpBody = respJson.toStyledString().c_str();
		return;
	}

	respJson["code"] = addGroupResp.code;
	respJson["message"] = addGroupResp.errorMsg.c_str();
	respJson["groupId"] = addGroupResp.id.c_str();

	resp.bSuccess = true;
	resp.httpBody = respJson.toStyledString().c_str();
}

int ProcessAddGroup::getNewGroupId( const std::list<GroupIdInfo> &groupIdInfos )
{
	std::set<int> grouIds;

	typedef std::list<GroupIdInfo>::const_iterator IT;
	for (IT it = groupIdInfos.begin(); it != groupIdInfos.end(); ++it)
	{
		grouIds.insert(it->id);
	}

	for (int i = GROUP_ID_BEGAIN; i < GROUP_ID_END; i++)
	{
		std::set<int>::iterator it = grouIds.find(i);
		if (it == grouIds.end())
		{
			//first not used id as new id
			return i;
		}
	}

	return GROUP_ID_END;
}


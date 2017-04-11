#include "processAddGroup.h"
#include <json/json.h>
#include <json/jsonParser.h>
#include <templateServer/templateServerProxy.h>
#include <set>

void ProcessAddGroup::process( const HttpRequest& req, HttpResponse& resp )
{
	std::string groupName;
	CJsonParser::parseAddGroup(req.httpBody, groupName);

	GetGroupIdResp getGroupIdResp;
	TemplateServerProxy::getGroupIdInfos(getGroupIdResp);
	int newGroupId = getNewGroupId(getGroupIdResp.groupIdInfos);

	GroupIdInfo newGroupIdInfo;
	newGroupIdInfo.id = newGroupId;
	newGroupIdInfo.name = groupName;

	AddGroupResp addGroupResp;
	TemplateServerProxy::addGroupId(newGroupIdInfo, addGroupResp);

	Json::Value respJson;
	respJson["code"] = addGroupResp.code;
	respJson["message"] = addGroupResp.errorMsg;
	respJson["groupId"] = addGroupResp.id;

	resp.bSuccess = true;
	resp.httpBody = respJson.toStyledString();
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


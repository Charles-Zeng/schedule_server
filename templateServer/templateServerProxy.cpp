#include "templateServerProxy.h"


bool TemplateServerProxy::addGroupId( const GroupIdInfo& req, AddGroupResp& resp )
{
	resp.code = 0;
	resp.id = "35";
	return true;
}

bool TemplateServerProxy::delGroupId( int64_t groupId, DelGroupResp& resp )
{
	return true;
}

bool TemplateServerProxy::addTemplate( const TemplateInfo& templateInfo, AddTemplateResp& resp )
{
	resp.code = 0;
	resp.id = 11;
	return true;
}

bool TemplateServerProxy::delTemplate( int id, DelTemplateResp& resp )
{
	return true;
}

bool TemplateServerProxy::dynamicOneToN( const DynamicOneToNReq& req, DynamicOneToNResp& resp )
{
	return true;
}

bool TemplateServerProxy::getGroupIdInfos( GetGroupIdResp& resp )
{
	resp.code = 0;
	GroupIdInfo info;
	info.id = 35;
	info.name = "test";
	resp.groupIdInfos.push_back(info);
	return true;
}

bool TemplateServerProxy::getFaceInfo( const std::string pic, GetFaceInfoResp& resp )
{
	return true;
}

bool TemplateServerProxy::oneToOne( const OneToOneInfo& req, OneToOneResp& resp )
{
	return true;
}



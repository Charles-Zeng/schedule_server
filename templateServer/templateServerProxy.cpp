#include "templateServerProxy.h"


bool TemplateServerProxy::addGroupId( const GroupIdInfo& req, AddGroupResp& resp )
{
	return true;
}

bool TemplateServerProxy::delGroupId( int64_t groupId, DelGroupResp& resp )
{
	return true;
}

bool TemplateServerProxy::addTemplate( const TemplateInfo& templateInfo, AddTemplateResp& resp )
{
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



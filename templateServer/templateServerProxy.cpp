#include "templateServerProxy.h"
#include <boost/lexical_cast.hpp>
#include <gsoap/MygSoapProcess.h>
#include <json/jsonBuilder.h>
#include <json/jsonParser.h>
#include <logger/logger.h>

//请求类型
static const std::string strAddGroupID = "AddGroupId";		//添加库类别
static const std::string strDelGroupID = "DelGroupId";		//删除库类别
static const std::string strAddTemplate = "AddTemplate";	//添加模板
static const std::string strDelTemplate = "DeleteTemplate"; //删除模板
static const std::string strDynamicOneToN = "SendFrame";	//动态1：N
static const std::string strGetGroupInfos = "GetGroupId";	//查询库类别
static const std::string strGetFaceInfo = "GetFaceInfo";	//获取人脸信息
static const std::string strOneToOne = "OneToOne";			//1：1接口

bool TemplateServerProxy::addGroupId( const GroupIdInfo& req, AddGroupResp& resp )
{
	std::string reqJson = CJsonBuilder::buildAddGroupId(req);

	std::string respJson;
	MygSoapProcess gSoapProxy;
	if (!gSoapProxy.FaceServiceAPI(strAddGroupID, reqJson, respJson))
	{
		resp.code = 0;
		resp.errorMsg = "invoke web service failed";
		return false;
	}

	if (!CJsonParser::parseAddGroupResp(respJson, resp))
	{
		resp.code = 0;
		resp.errorMsg = "parse web service response failed";
		return false;
	}

	if (GSOAP_FAILED == resp.code)
	{
		return false;
	}

	return true;
}

bool TemplateServerProxy::delGroupId( int64_t groupId, DelGroupResp& resp )
{
	std::string reqJson = CJsonBuilder::buildDelGroupId(boost::lexical_cast<std::string>(groupId));

	std::string respJson;
	MygSoapProcess gSoapProxy;
	if (!gSoapProxy.FaceServiceAPI(strDelGroupID, reqJson, respJson))
	{
		resp.code = 0;
		resp.errorMsg = "invoke web service failed";
		return false;
	}

	if (!CJsonParser::parseDelGroupResp(respJson, resp))
	{
		resp.code = 0;
		resp.errorMsg = "parse web service response failed";
		return false;
	}

	if (GSOAP_FAILED == resp.code)
	{
		return false;
	}

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



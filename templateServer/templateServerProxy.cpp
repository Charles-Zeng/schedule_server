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
static const std::string strOneToN = "OneToN";              //静态1:N接口

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
		resp.errorMsg = resp.errorMsg;
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
		resp.errorMsg = resp.errorMsg;
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
	std::string reqJson = CJsonBuilder::buildAddTemplate(templateInfo);

	std::string respJson;
	MygSoapProcess gSoapProxy;
	if (!gSoapProxy.FaceServiceAPI(strAddTemplate, reqJson, respJson))
	{
		resp.code = 0;
		resp.errorMsg = "invoke web service failed";
		return false;
	}

	if (!CJsonParser::parseAddTemplateResp(respJson, resp))
	{
		resp.code = 0;
		resp.errorMsg = resp.errorMsg;
		return false;
	}

	if (GSOAP_FAILED == resp.code)
	{
		return false;
	}

	return true;
}

bool TemplateServerProxy::delTemplate(std::string id, DelTemplateResp& resp )
{
	std::string reqJson = CJsonBuilder::buildDelTemplate(id);

	std::string respJson;
	MygSoapProcess gSoapProxy;
	if (!gSoapProxy.FaceServiceAPI(strDelTemplate, reqJson, respJson))
	{
		resp.code = 0;
		resp.errorMsg = "invoke web service failed";
		return false;
	}

	if (!CJsonParser::parseDelTemplateResp(respJson, resp))
	{
		resp.code = 0;
		resp.errorMsg = resp.errorMsg;
		return false;
	}

	if (GSOAP_FAILED == resp.code)
	{
		return false;
	}

	return true;
}

bool TemplateServerProxy::dynamicOneToN(const DynamicOneToNReq& req, DynamicOneToNResp& resp )
{
	std::string reqJson = CJsonBuilder::buildDynamicOneToN(req);

	std::string respJson;
	MygSoapProcess gSoapProxy;
	if (!gSoapProxy.FaceServiceAPI(strDynamicOneToN, reqJson, respJson))
	{
		resp.code = 0;
		resp.errorMsg = "invoke web service failed";
		return false;
	}

	if (!CJsonParser::parseDynamicOneToNResp(respJson, resp))
	{
		resp.code = 0;
		resp.errorMsg = resp.errorMsg;
		return false;
	}

	if (GSOAP_FAILED == resp.code)
	{
		return false;
	}

	return true;
}

bool TemplateServerProxy::getGroupIdInfos( GetGroupIdResp& resp )
{
	std::string respJson;
	MygSoapProcess gSoapProxy;
	if (!gSoapProxy.FaceServiceAPI(strGetGroupInfos, "", respJson))
	{
		resp.code = 0;
		resp.errorMsg = "invoke web service failed";
		return false;
	}

	if (!CJsonParser::parseGetGroupIdResp(respJson, resp))
	{
		resp.code = 0;
		resp.errorMsg = resp.errorMsg;
		return false;
	}

	if (GSOAP_FAILED == resp.code)
	{
		return false;
	}

	return true;
}

bool TemplateServerProxy::getFaceInfo( const std::string pic, GetFaceInfoResp& resp )
{
	std::string reqJson = CJsonBuilder::buildGetFaceInfo(pic);

	std::string respJson;
	MygSoapProcess gSoapProxy;
	if (!gSoapProxy.FaceServiceAPI(strGetFaceInfo, reqJson, respJson))
	{
		resp.code = 0;
		resp.errorMsg = "invoke web service failed";
		return false;
	}

	if (!CJsonParser::parseGetFaceInfoResp(respJson, resp))
	{
		resp.code = 0;
		resp.errorMsg = resp.errorMsg;
		return false;
	}

	if (GSOAP_FAILED == resp.code)
	{
		return false;
	}

	return true;
}

bool TemplateServerProxy::oneToOne( const OneToOneInfo& req, OneToOneResp& resp )
{
	std::string reqJson = CJsonBuilder::buildOneToOne(req);

	std::string respJson;
	MygSoapProcess gSoapProxy;
	if (!gSoapProxy.FaceServiceAPI(strOneToOne, reqJson, respJson))
	{
		resp.code = 0;
		resp.errorMsg = "invoke web service failed";
		return false;
	}

	if (!CJsonParser::parseOneToOneResp(respJson, resp))
	{
		resp.code = 0;
		resp.errorMsg = resp.errorMsg;
		return false;
	}

	if (GSOAP_FAILED == resp.code)
	{
		return false;
	}

	return true;
}

bool TemplateServerProxy::oneToN( const OneToNInfo& req, OneToNResp& resp )
{
	std::string reqJson = CJsonBuilder::buildOneToN(req);

	std::string respJson;
	MygSoapProcess gSoapProxy;
	if (!gSoapProxy.FaceServiceAPI(strOneToN, reqJson, respJson))
	{
		resp.code = 0;
		resp.errorMsg = "invoke web service failed";
		return false;
	}

	if (!CJsonParser::parseOneToNResp(respJson, resp))
	{
		resp.code = 0;
		resp.errorMsg = resp.errorMsg;
		return false;
	}

	if (GSOAP_FAILED == resp.code)
	{
		return false;
	}

	return true;
}




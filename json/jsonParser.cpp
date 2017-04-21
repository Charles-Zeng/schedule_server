#include "jsonParser.h"
#include <json/json.h>
#include <boost/tokenizer.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>
#include <vector>

CJsonParser::CJsonParser()
{
}


CJsonParser::~CJsonParser()
{
}

bool CJsonParser::parseUploadImageJson(const std::string& strJson, ImageInfo& imageInfo)
{
	Json::Reader reader;
	Json::Value value;

	if (reader.parse(strJson, value))
	{
		if (value["camerId"] == Json::Value::null  || value["location"] == Json::Value::null
			|| value["time"] == Json::Value::null || value["gender"] == Json::Value::null
			|| value["image"] == Json::Value::null)
		{
			return false;
		}

        imageInfo.camerId = value["camerId"].asCString();
        imageInfo.location = value["location"].asCString();
		imageInfo.monitorTime = value["time"].asInt64();
		imageInfo.gender = value["gender"].asInt();
        imageInfo.imageStr = value["image"].asCString();

		return true;
	}
	
    return false;
}

bool CJsonParser::parseAddGroup(const std::string& strJson, std::string &groupName)
{
    Json::Reader reader;
    Json::Value value;

    if (reader.parse(strJson, value))
    {
        if(value["name"] == Json::Value::null)
        {
            return false;
        }

        groupName = value["name"].asCString();

		return true;
    }

    return false;
}

bool CJsonParser::parseDelGroup(const std::string& strJson, int64_t &groupId)
{
    Json::Reader reader;
    Json::Value value;

    if (reader.parse(strJson, value))
    {
        if(value["groupId"] == Json::Value::null)
        {
            return false;
        }

        groupId = boost::lexical_cast<int64_t>(value["groupId"].asCString());

		return true;
    }

    return false;
}

bool CJsonParser::parseAddTemplate(const std::string &strJson, TemplateInfo& templateInfo)
{
    Json::Reader reader;
    Json::Value value;
	//由于建模那边json字段grounId改为grounIds
    if (reader.parse(strJson, value))
    {
        if(value["createTime"] == Json::Value::null || value["groupIds"] == Json::Value::null
                || value["gender"] == Json::Value::null || value["image"] == Json::Value::null)
        {
            return false;
        }

        templateInfo.createTime = value["createTime"].asCString();
        templateInfo.groupId = value["groupIds"].asCString();
        templateInfo.gender = boost::lexical_cast<int>(value["gender"].asCString());
        templateInfo.imageStr = value["image"].asCString();

		return true;
    }

    return false;
}

bool CJsonParser::parseDelTemplate(const std::string &strJson, std::string &templateId)
{
    Json::Reader reader;
    Json::Value value;

    if (reader.parse(strJson, value))
    {
        if(value["templateId"] == Json::Value::null)
        {
            return false;
        }

        templateId = value["templateId"].asCString();

		return true;
    }

    return false;
}
//add liyong 20170420 
bool CJsonParser::parseGetFaceInfo(const std::string& strJson, GetFaceInfoReq& getfaceinfo)
{
	Json::Reader reader;
	Json::Value value;

	if (reader.parse(strJson, value))
	{
		if (value["pic"] == Json::Value::null)
		{
			return false;
		}

		getfaceinfo.picBase64 = value["pic"].asCString();
		return true;
	}

	return false;
}

bool CJsonParser::parseOneToOne(const std::string &strJson, OneToOneInfo &oneToOneInfo)
{
    Json::Reader reader;
    Json::Value value;

    if (reader.parse(strJson, value))
    {
        if(value["pic1"] == Json::Value::null || value["pic2"] == Json::Value::null)
        {
            return false;
        }

        oneToOneInfo.pic1Base64 = value["pic1"].asCString();
        oneToOneInfo.pic2Base64 = value["pic2"].asCString();

		return true;
    }

    return false;
}

bool CJsonParser::parseOneToN(const std::string &strJson, OneToNInfo &oneToNInfo)
{
    Json::Reader reader;
    Json::Value value;

    if (reader.parse(strJson, value))
    {
        if(value["pic"] == Json::Value::null || value["threshold"] == Json::Value::null
                || value["count"] == Json::Value::null || value["groupIds"] == Json::Value::null
                || value["gender"] == Json::Value::null)
        {
            return false;
        }

        oneToNInfo.picBase64 = value["pic"].asCString();
        oneToNInfo.threshold = value["threshold"].asFloat();
        oneToNInfo.count = value["count"].asInt();
        std::string strGroupIds = value["groupIds"].asCString();
        std::vector<std::string> vecGroupIds;
        boost::split(vecGroupIds, strGroupIds, boost::is_any_of(","));
        BOOST_FOREACH(const std::string &strId, vecGroupIds)
        {
            oneToNInfo.groupIds.push_back(atoi(strId.c_str()));
        }
        oneToNInfo.gender = value["gender"].asInt();

		return true;
    }

    return false;
}

bool CJsonParser::parseAddGroupResp( const std::string& strJson, AddGroupResp& addGroupResp )
{
	Json::Reader reader;
	Json::Value value;

	if (reader.parse(strJson, value))
	{
		if(value["code"] == Json::Value::null
			|| value["id"] == Json::Value::null)
		{
			return false;
		}

		addGroupResp.code = value["code"].asInt();		
		addGroupResp.id = boost::lexical_cast<std::string>(value["id"].asInt());

		if (value["errorMessage"] != Json::Value::null)
		{
			addGroupResp.errorMsg = value["errorMessage"].asCString();
		}

		return true;
	}

	return false;
}

bool CJsonParser::parseDelGroupResp( const std::string& strJson, DelGroupResp& delGroupResp )
{
	Json::Reader reader;
	Json::Value value;

	if (reader.parse(strJson, value))
	{
		if(value["code"] == Json::Value::null || value["id"] == Json::Value::null)
		{
			return false;
		}

		delGroupResp.code = value["code"].asInt();		
		delGroupResp.id = value["id"].asInt();

		if (value["errorMessage"] != Json::Value::null)
		{
			delGroupResp.errorMsg = value["errorMessage"].asCString();
		}

		return true;
	}

	return false;
}

bool CJsonParser::parseAddTemplateResp( const std::string& strJson, AddTemplateResp& addTemplateResp )
{
	Json::Reader reader;
	Json::Value value;

	if (reader.parse(strJson, value))
	{
		if(value["code"] == Json::Value::null
			|| value["id"] == Json::Value::null)
		{
			return false;
		}

		addTemplateResp.code = value["code"].asInt();		
		addTemplateResp.id = value["id"].asInt();
		if (value["errorMessage"] != Json::Value::null)
		{
			addTemplateResp.errorMsg = value["errorMessage"].asCString();
		}

		return true;
	}

	return false;
}

bool CJsonParser::parseDelTemplateResp( const std::string& strJson, DelTemplateResp& delTemplateResp )
{
	Json::Reader reader;
	Json::Value value;

	if (reader.parse(strJson, value))
	{
		if(value["code"] == Json::Value::null
			|| value["id"] == Json::Value::null)
		{
			return false;
		}

		delTemplateResp.code = value["code"].asInt();		
		delTemplateResp.id = value["id"].asInt();
		if (value["errorMessage"] != Json::Value::null)
		{
			delTemplateResp.errorMsg = value["errorMessage"].asCString();
		}

		return true;
	}

	return false;
}

bool CJsonParser::parseDynamicOneToNResp( const std::string& strJson, DynamicOneToNResp& dynamicOneToNResp )
{
	Json::Reader reader;
	Json::Value value;

	if (reader.parse(strJson, value))
	{
		if(value["code"] == Json::Value::null
			|| value["listMatches"] == Json::Value::null)
		{
			return false;
		}

		dynamicOneToNResp.code = value["code"].asInt();
		
		if (value["errorMessage"] != Json::Value::null)
		{
			dynamicOneToNResp.errorMsg = value["errorMessage"].asCString();
		}

		Json::Value matchArray = value["listMatches"];
		if (!matchArray.isArray())
		{
			return false;
		}

		for (unsigned int i = 0; i < matchArray.size(); i++)
		{
			Matche match;
			match.id = matchArray[i]["id"].asInt();
			match.score = matchArray[i]["score"].asFloat();
			match.sourceId = matchArray[i]["sourceID"].asCString();
			dynamicOneToNResp.listMatches.push_back(match);
		}

		return true;
	}

	return false;
}

bool CJsonParser::parseGetGroupIdResp( const std::string& strJson, GetGroupIdResp& getGroupIdResp )
{
	Json::Reader reader;
	Json::Value value;

	if (reader.parse(strJson, value))
	{
		if(value["code"] == Json::Value::null
			|| value["groupIdInfos"] == Json::Value::null)
		{
			return false;
		}

		getGroupIdResp.code = value["code"].asInt();
		if (value["errorMessage"] != Json::Value::null)
		{
			getGroupIdResp.errorMsg = value["errorMessage"].asCString();
		}
		

		Json::Value groupIdInfoArray = value["groupIdInfos"];
		if (!groupIdInfoArray.isArray())
		{
			return false;
		}

		for (unsigned int i = 0; i < groupIdInfoArray.size(); i++)
		{
			GroupIdInfo info;
			info.id = groupIdInfoArray[i]["id"].asInt();
			info.name = groupIdInfoArray[i]["name"].asCString();
			getGroupIdResp.groupIdInfos.push_back(info);
		}

		return true;
	}

	return false;
}

bool CJsonParser::parseGetFaceInfoResp( const std::string& strJson, GetFaceInfoResp& getFaceInfoResp )
{
	Json::Reader reader;
	Json::Value value;

	if (reader.parse(strJson, value))
	{
		if(value["code"] == Json::Value::null)
		{
			return false;
		}

		getFaceInfoResp.code = value["code"].asInt();
		
		if (value["errorMessage"] != Json::Value::null)
		{
			getFaceInfoResp.errorMsg = value["errorMessage"].asCString();
		}

		if (value["faceInfos"] == Json::Value::null)
		{
			return false;
		}
		//add liyong 将返回的faceInfos 值返回给客户端
		Json::Value faceInfoArray = value["faceInfos"];
		if (!faceInfoArray.isArray())
		{
			return false;
		}
		getFaceInfoResp.strRespJson = faceInfoArray;
		/*getFaceInfoResp.faceInfo.qualityScore = 0.0001;
		for (unsigned int i = 0; i < faceInfoArray.size(); i++)
		{
			float qualityScore = faceInfoArray[i]["qualityScore"].asFloat();
			if (qualityScore > getFaceInfoResp.faceInfo.qualityScore)
			{
				getFaceInfoResp.faceInfo.qualityScore = faceInfoArray[i]["qualityScore"].asFloat();
				getFaceInfoResp.faceInfo.facePic = faceInfoArray[i]["facePic"].asCString();
			}

		}*/

		return true;
	}

	return false;
}

bool CJsonParser::parseOneToOneResp( const std::string& strJson, OneToOneResp& oneToOneResp )
{
	Json::Reader reader;
	Json::Value value;

	if (reader.parse(strJson, value))
	{
		if(value["code"] == Json::Value::null
			|| value["score"] == Json::Value::null)
		{
			return false;
		}

		oneToOneResp.code = value["code"].asInt();		
		oneToOneResp.score = value["score"].asFloat();
		if (value["errorMessage"] != Json::Value::null)
		{
			oneToOneResp.errorMsg = value["errorMessage"].asCString();
		}

		return true;
	}

	return false;
}

bool CJsonParser::parseOneToNResp( const std::string& strJson, OneToNResp& oneToNResp )
{
	Json::Reader reader;
	Json::Value value;

	if (reader.parse(strJson, value))
	{
		if(value["code"] == Json::Value::null
			|| value["listMachingValues"] == Json::Value::null)
		{
			return false;
		}

		oneToNResp.code = value["code"].asInt();
		
		if (value["errorMessage"] != Json::Value::null)
		{
			oneToNResp.errorMsg = value["errorMessage"].asCString();
		}

		Json::Value matchingArray = value["listMachingValues"];
		if (!matchingArray.isArray())
		{
			return false;
		}

		for (unsigned int i = 0; i < matchingArray.size(); i++)
		{
			MachingValue match;
			match.id = matchingArray[i]["id"].asInt();
			match.score = matchingArray[i]["score"].asFloat();
			oneToNResp.listMachingValues.push_back(match);
		}

		return true;
	}

	return false;
}

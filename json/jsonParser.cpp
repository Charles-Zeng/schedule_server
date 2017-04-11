#include "jsonParser.h"
#include <json/json.h>
#include <boost/tokenizer.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>
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

        groupId = value["groupId"].asInt64();
    }

    return false;
}

bool CJsonParser::parseAddTemplate(const std::string &strJson, TemplateInfo& templateInfo)
{
    Json::Reader reader;
    Json::Value value;

    if (reader.parse(strJson, value))
    {
        if(value["createTime"] == Json::Value::null || value["groupId"] == Json::Value::null
                || value["gender"] == Json::Value::null || value["image"] == Json::Value::null)
        {
            return false;
        }

        templateInfo.createTime = value["createTime"].asCString();
        templateInfo.groupId = value["groupId"].asCString();
        templateInfo.gender = value["gender"].asInt();
        templateInfo.imageStr = value["image"].asCString();
    }

    return false;
}

bool CJsonParser::parseDelTemplate(const std::string &strJson, int32_t &templateId)
{
    Json::Reader reader;
    Json::Value value;

    if (reader.parse(strJson, value))
    {
        if(value["templateId"] == Json::Value::null)
        {
            return false;
        }

        templateId = value["templateId"].asInt();
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

        oneToOneInfo.pic1Base64 = value["pic1"].asCString();\
        oneToOneInfo.pic2Base64 = value["pic2"].asCString();
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
    }

    return false;
}

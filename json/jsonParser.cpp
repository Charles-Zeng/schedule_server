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
		if (value["camerId"] == Json::Value::null || value["time"] == Json::Value::null 
			|| value["image"] == Json::Value::null || value["gender"] == Json::Value::null)
		{
			return false;
		}

        imageInfo.camerId = value["camerId"].asCString();
		imageInfo.monitorTime = value["time"].asInt64();
		imageInfo.gender = value["gender"].asCString();
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
	//���ڽ�ģ�Ǳ�json�ֶ�grounId��ΪgrounIds
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

bool CJsonParser::parseOneToN(const std::string& strJson, OneToNInfo& oneToNInfo)
{
	Json::Reader reader;
	Json::Value value;
	if (reader.parse(strJson, value))
	{
		if (value["pic"] == Json::Value::null || value["threshold"] == Json::Value::null
			|| value["count"] == Json::Value::null || value["groupIds"] == Json::Value::null)
		{
			return false;
		}
		oneToNInfo.picBase64 = value["pic"].asCString();
		oneToNInfo.threshold = boost::lexical_cast<float>(value["threshold"].asCString());
		oneToNInfo.count = boost::lexical_cast<int>(value["count"].asCString());
		std::string strGroupIds = value["groupIds"].asCString();
		std::vector<std::string> vecGroupIds;
		boost::split(vecGroupIds, strGroupIds, boost::is_any_of(","));
		BOOST_FOREACH(const std::string &strId, vecGroupIds)
		{
			oneToNInfo.groupIds.push_back(strId);
		}
		oneToNInfo.gender = value["gender"].asCString();

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
		//errorMessage
		if (value["errorMessage"] != Json::Value::null)
		{
			addGroupResp.errorMsg = value["errorMessage"].asCString();
		}
		//code 
		if (value["code"] != Json::Value::null)
		{
			addGroupResp.code = value["code"].asInt();
			if (0 == addGroupResp.code)
			{
				return false;
			}
		}
		else
		{
			addGroupResp.errorMsg = "��ӿ�id����json����code Ϊ�գ�";
		}
		//id
		if(value["id"] != Json::Value::null)
		{
			addGroupResp.id = boost::lexical_cast<std::string>(value["id"].asInt());
		}
		else
		{
			addGroupResp.errorMsg = "��ӿ�id��idֵΪ�գ�";
			return false;
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
		//errorMessage
		if (value["errorMessage"] != Json::Value::null)
		{
			delGroupResp.errorMsg = value["errorMessage"].asCString();
		}
		//code
		if (value["code"] != Json::Value::null)
		{
			delGroupResp.code = value["code"].asInt();
			if (0 == value["code"].asInt())
			{
				return false;
			}
		}
		else
		{
			delGroupResp.errorMsg = "ɾ����id codeʧ�ܣ�";
		}
		//id
		if(value["id"] != Json::Value::null)
		{
			delGroupResp.id = value["id"].asInt();	
		}
		else
		{
			delGroupResp.errorMsg = "ɾ����idΪ�գ�";
			return false;
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
		if (value["errorMessage"] != Json::Value::null)
		{
			addTemplateResp.errorMsg = value["errorMessage"].asCString();
		}
		//code 
		if (value["code"] != Json::Value::null)
		{
			addTemplateResp.code = value["code"].asInt();
			if (0 == value["code"].asInt())
			{
				return false;
			}
		}
		else
		{
			addTemplateResp.errorMsg = "���ģ��codeΪnull��";
			return false;
		}
		//id
		if(value["id"] != Json::Value::null)
		{
			addTemplateResp.id = value["id"].asInt();	
		}
		else
		{
			addTemplateResp.errorMsg = "���idΪ�գ�";
			return false;
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
		//errorMessage
		if (value["errorMessage"] != Json::Value::null)
		{
			delTemplateResp.errorMsg = value["errorMessage"].asCString();
		}
		//code
		if (value["code"] != Json::Value::null)
		{
			delTemplateResp.code = value["code"].asInt();
			if (0 == delTemplateResp.code)
			{
				return false;
			}
		}
		else
		{
			delTemplateResp.errorMsg = "����ɾ��ģ�巵��json�е�codeΪ�գ�";
			return false;
		}
		//id
		if(value["id"] != Json::Value::null)
		{
			delTemplateResp.id = value["id"].asInt();	
		}
		else
		{
			delTemplateResp.errorMsg = "����ɾ��ģ�巵��json�е�idΪ�գ�";
			return false;
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
		//errorMessage
		if (value["errorMessage"] != Json::Value::null)
		{
			dynamicOneToNResp.errorMsg = value["errorMessage"].asCString();
		}
		//code
		if(value["code"] != Json::Value::null)
		{
			dynamicOneToNResp.code = value["code"].asInt();
			if (0 == dynamicOneToNResp.code)
			{
				return false;
			}
		}
		else
		{
			dynamicOneToNResp.errorMsg = "��̬һ��Ncode����Ϊ�գ�";
			return false;
		}
		//sourceID
		if (value["sourceID"] != Json::Value::null)
		{
			dynamicOneToNResp.sourceID = value["sourceID"].asCString();
		}
		else
		{
			dynamicOneToNResp.errorMsg = "��̬һ��N sourceID is �գ�";
			return false;
		}
		//listMatches
		if (value["listMatches"] != Json::Value::null)
		{
			Json::Value matchArray = value["listMatches"];
			if (!matchArray.isArray())
			{
				dynamicOneToNResp.errorMsg = "invalid webservice resp json, listMatches is not array";
				return false;
			}
			for (unsigned int i = 0; i < matchArray.size(); i++)
			{
				Matche match;
				match.id = matchArray[i]["id"].asInt();
				match.score = matchArray[i]["score"].asFloat();
				match.sourceId = dynamicOneToNResp.sourceID;
				dynamicOneToNResp.listMatches.push_back(match);
			}
		}
		else
		{
			dynamicOneToNResp.errorMsg = "��̬һ��N listMatches is ��";
			return false;
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
		//errorMessage
		if (value["errorMessage"] != Json::Value::null)
		{
			getGroupIdResp.errorMsg = value["errorMessage"].asCString();
		}
		//code
		if (value["code"] != Json::Value::null)
		{
			getGroupIdResp.code = value["code"].asInt();
			if (0 == getGroupIdResp.code)
			{
				return false;
			}
		}
		else
		{
			getGroupIdResp.errorMsg = "��ȡ��id���ص�json codeΪ�գ�";
		}
		//groupIdInfos
		if(value["groupIdInfos"] != Json::Value::null)
		{
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
		}
		else
		{
			getGroupIdResp.errorMsg = "��ȡ�ⷵ�ص�json groupIdInfosΪ�գ�";
			return false;
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
		//errorMessage
		if (value["errorMessage"] != Json::Value::null)
		{
			getFaceInfoResp.errorMsg = value["errorMessage"].asCString();
		}
		//code
		if(value["code"] != Json::Value::null)
		{
			getFaceInfoResp.code = value["code"].asInt();
			if (0 == getFaceInfoResp.code)
			{
				return false;
			}
		}
		else
		{
			getFaceInfoResp.errorMsg = "��ȡ������ϢcodeΪ�գ�";
			return false;
		}
		//faceInfos
		if (value["faceInfos"] != Json::Value::null)
		{
			//add liyong �����ص�faceInfos ֵ���ظ��ͻ���
			Json::Value faceInfoArray = value["faceInfos"];
			if (!faceInfoArray.isArray())
			{
				return false;
			}
			getFaceInfoResp.strRespJson = faceInfoArray;	
		}
		else
		{
			getFaceInfoResp.errorMsg = "��ȡ������ϢfaceInfoArrayΪ�գ�";
			return false;
		}	
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
		//errorMessage
		if (value["errorMessage"] != Json::Value::null)
		{
			oneToOneResp.errorMsg = value["errorMessage"].asCString();
		}
		//code 
		if (value["code"] != Json::Value::null)
		{
			oneToOneResp.code = value["code"].asInt();
			if (0 == oneToOneResp.code)
			{
				return false;
			}
		}
		else
		{
			oneToOneResp.errorMsg = "һ��һ��Ӧjson codeΪ�գ�";
			return false;
		}
		//score
		if(value["score"] != Json::Value::null)
		{
			oneToOneResp.score = value["score"].asFloat();	
		}
		else
		{
			oneToOneResp.errorMsg = "һ��һ��Ӧjson score ����Ϊ��";
			return false;
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
		//errorMessage
		if (value["errorMessage"] != Json::Value::null)
		{
			oneToNResp.errorMsg = value["errorMessage"].asCString();
		}
		//code
		if (value["code"] != Json::Value::null)
		{
			oneToNResp.code = value["code"].asInt();
			if (0 == oneToNResp.code)
			{
				return false;
			}
		}
		else
		{
			oneToNResp.errorMsg = "��̬һ��N codeΪ�գ�";
			return false;
		}
		//listMachingValues
		if(value["listMachingValues"] != Json::Value::null)
		{
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
		}
		else
		{
			oneToNResp.errorMsg = "��̬һ��N listMachingValuesΪ�գ�";
			return false;
		}
		return true;
	}
	return false;
}

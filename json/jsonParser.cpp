#include "jsonParser.h"
#include <json/json.h>

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

		imageInfo.camerId = value["camerId"].asString();
		imageInfo.location = value["location"].asString();
		imageInfo.monitorTime = value["time"].asInt64();
		imageInfo.gender = value["gender"].asInt();
		imageInfo.imageStr = value["image"].asString();

		return true;
	}
	
	return false;
}

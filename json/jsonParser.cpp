#include "jsonParser.h"
#include <json/json.h>

CJsonParser::CJsonParser()
{
}


CJsonParser::~CJsonParser()
{
}

bool CJsonParser::parseUploadImageJson(const std::string& strJson)
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
	}
	
	return false;
}

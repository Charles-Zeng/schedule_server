#pragma once

#include <string>

class CJsonParser
{
public:
	CJsonParser();
	virtual ~CJsonParser();
	static bool parseUploadImageJson(const std::string& strJson);
};


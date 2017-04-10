#pragma once

#include <string>

#include <common/dataStruct.h>

class CJsonParser
{
public:
	CJsonParser();
	virtual ~CJsonParser();
	static bool parseUploadImageJson(const std::string& strJson, ImageInfo& imageInfo);
};


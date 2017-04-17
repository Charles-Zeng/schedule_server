#pragma once

#include <string>

#include <common/dataStruct.h>

class CJsonParser
{
private:
	CJsonParser();
public:
	virtual ~CJsonParser();
	static bool parseUploadImageJson(const std::string& strJson, ImageInfo& imageInfo);
    static bool parseAddGroup(const std::string& strJson, std::string& groupName);
    static bool parseDelGroup(const std::string& strJson, int64_t& groupId);
    static bool parseAddTemplate(const std::string& strJson, TemplateInfo& templateInfo);
    static bool parseDelTemplate(const std::string& strJson, int32_t& templateId);
    static bool parseOneToOne(const std::string& strJson, OneToOneInfo& oneToOneInfo);
    static bool parseOneToN(const std::string& strJson, OneToNInfo& oneToNInfo);

	//web service œÏ”¶Ω‚Œˆ
	static bool parseAddGroupResp(const std::string& strJson, AddGroupResp& addGroupResp);
	static bool parseDelGroupResp(const std::string& strJson, DelGroupResp& delGroupResp);
};


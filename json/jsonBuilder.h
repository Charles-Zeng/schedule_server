#ifndef _JSON_BUILDER_H_
#define _JSON_BUILDER_H_

#include <string>
#include <common/dataStruct.h>

class CJsonBuilder
{
public:
	static std::string buildAddTemplate(const TemplateInfo& templateInfo);
	static std::string buildDelTemplate(std::string id);
	static std::string buildAddGroupId(const GroupIdInfo& groupIdInfo);
	static std::string buildDelGroupId(const std::string& id);
	static std::string buildGetFaceInfo(const std::string& pic);
	static std::string buildOneToOne(const OneToOneInfo& oneToOneInfo);
	static std::string buildOneToN(const OneToNInfo& oneToNInfo);
	static std::string buildDynamicOneToN(const DynamicOneToNReq& dynamicOneToNReq);
};

#endif //_JSON_BUILDER_H_

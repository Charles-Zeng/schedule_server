#ifndef _TEMPLATE_SERVER_PROXY_H_
#define _TEMPLATE_SERVER_PROXY_H_

#include <common/dataStruct.h>

class TemplateServerProxy
{
public:
	static void addTemplate(const TemplateInfo& templateInfo, AddTemplateResp& resp);
	static void dynamicOneToN(const DynamicOneToNReq& req, DynamicOneToNResp& resp);
	static void getGroupIdInfos(GetGroupIdResp& resp);
	static void addGroupId(const GroupIdInfo& req, AddGroupResp& resp);
	static void delGroupId(int64_t groupId, DelGroupResp& resp);
};

#endif // _TEMPLATE_SERVER_PROXY_H_

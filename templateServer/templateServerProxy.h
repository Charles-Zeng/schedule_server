#ifndef _TEMPLATE_SERVER_PROXY_H_
#define _TEMPLATE_SERVER_PROXY_H_

#include <common/dataStruct.h>

class TemplateServerProxy
{
public:
	static bool addGroupId(const GroupIdInfo& req, AddGroupResp& resp);
	static bool delGroupId(int64_t groupId, DelGroupResp& resp);
	static bool addTemplate(const TemplateInfo& templateInfo, AddTemplateResp& resp);
	static bool delTemplate(int id, DelTemplateResp& resp);		 
	static bool dynamicOneToN(const DynamicOneToNReq& req, DynamicOneToNResp& resp);
	static bool getGroupIdInfos(GetGroupIdResp& resp);
	static bool getFaceInfo(const std::string pic, GetFaceInfoResp& resp);
	static bool oneToOne(const OneToOneInfo& req, OneToOneResp& resp);
	static bool oneToN(const OneToNInfo& req, OneToNResp& resp);
};

#endif // _TEMPLATE_SERVER_PROXY_H_

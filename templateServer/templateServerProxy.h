#ifndef _TEMPLATE_SERVER_PROXY_H_
#define _TEMPLATE_SERVER_PROXY_H_

#include <common/dataStruct.h>

class TemplateServerProxy
{
public:
	static void addTemplate(ImageInfo& imageInfo);
	static void dynamicOneToN(const DynamicOneToNReq& req, DynamicOneToNResp& resp);
};

#endif // _TEMPLATE_SERVER_PROXY_H_

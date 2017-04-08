#include "sysService.h"
#include "../httpSvr/httpSvr.h"
#include "../syscfg/sysConfig.h"
#include "../logger/logger.h"

CSysService::CSysService()
{
}


CSysService::~CSysService()
{
}

bool CSysService::start()
{
	CHttpSvr httpSvr;
	int port = CSysConfig::instance().m_httpConfig.m_httpPort;
	if (!httpSvr.init(port))
	{
		CLogger::instance()->write_log(LOG_LEVEL_ERR, "启动http服务器失败，配置端口=%d", port);
		return false;
	}
	CLogger::instance()->write_log(LOG_LEVEL_INFO, "启动http服务器成功，配置端口=%d", port);
	return true;
}
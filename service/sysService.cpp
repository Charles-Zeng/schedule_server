#include "sysService.h"
#include "../syscfg/sysConfig.h"
#include "../logger/logger.h"
#include "../processor/processorManager.h"

CSysService::CSysService()
{
}


CSysService::~CSysService()
{
}

bool CSysService::start()
{
	int port = CSysConfig::instance().m_httpConfig.m_httpPort;
	if (!m_httpSvr.init(port))
	{
		CLogger::instance()->write_log(LOG_LEVEL_ERR, "启动http服务器失败，配置端口=%d", port);
		return false;
	}
	CLogger::instance()->write_log(LOG_LEVEL_INFO, "启动http服务器成功，配置端口=%d", port);

	int processThreadNum = CSysConfig::instance().m_processConfig.m_threadNum;
	if (!ProcessorManager::instance().init(processThreadNum))
	{
		CLogger::instance()->write_log(LOG_LEVEL_ERR, "start processor module failed");
		return false;
	}
	CLogger::instance()->write_log(LOG_LEVEL_INFO, "start processor module success，thread num:%d", processThreadNum);


	return true;
}
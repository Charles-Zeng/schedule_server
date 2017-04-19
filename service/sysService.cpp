#include "sysService.h"
#include "../syscfg/sysConfig.h"
#include "../logger/logger.h"
#include "../processor/processorManager.h"
#include <database/DBAdaptor.h>

CSysService::CSysService()
{
}


CSysService::~CSysService()
{
}

bool CSysService::start()
{
	std::string username = CSysConfig::instance().m_dbConfig.m_username;
	std::string pwd = CSysConfig::instance().m_dbConfig.m_password;
	std::string connStr = CSysConfig::instance().m_dbConfig.m_connection;
	int poolMinConn = CSysConfig::instance().m_dbConfig.m_poolMinConn;
	int poolMaxConn = CSysConfig::instance().m_dbConfig.m_poolMaxConn;
	int poolIncrConn = CSysConfig::instance().m_dbConfig.m_poolIncrConn;
	if (!DBAdaptor::instance()->init(username, pwd, connStr, poolMinConn, poolMaxConn, poolIncrConn))
	{
		CLogger::instance()->write_log(LOG_LEVEL_ERR, "初始化数据库失败");
		return false;
	}

	int port = CSysConfig::instance().m_httpConfig.m_httpPort;
    m_httpSvr.init(port);
	CLogger::instance()->write_log(LOG_LEVEL_INFO, "启动http服务器成功，配置端口=%d", port);

	int processThreadNum = CSysConfig::instance().m_processConfig.m_threadNum;
	if (!ProcessorManager::instance().init(processThreadNum))
	{
		CLogger::instance()->write_log(LOG_LEVEL_ERR, "启动业务处理模块失败");
		return false;
	}
	CLogger::instance()->write_log(LOG_LEVEL_INFO, "启动业务处理模块成功，线程数:%d", processThreadNum);


	return true;
}

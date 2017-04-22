#include "sysConfig.h"
#include "cfgReader.h"
#include "../global/config.h"
#include "../logger/logger.h"

CSysConfig CSysConfig::m_instance;

CSysConfig::CSysConfig()
{
}


CSysConfig::~CSysConfig()
{
}

CSysConfig& CSysConfig::instance()
{
	return m_instance;
}

bool CSysConfig::loadConfig()
{
	CCfgReader reader(CONF_FILE);

	//读取http端口配置
	if (!reader.getIntValue("httpsvr.port", m_instance.m_httpConfig.m_httpPort))
	{
		CLogger::instance()->write_log(LOG_LEVEL_ERR, "读取http服务器端口配置失败");
		return false;
	}

	//读取数据库用户名
	if (!reader.getStringValue("database.username", m_instance.m_dbConfig.m_username))
	{
		CLogger::instance()->write_log(LOG_LEVEL_ERR, "读取数据库用户名配置失败");
		return false;
	}
	//读取数据库密码
	if (!reader.getStringValue("database.password", m_instance.m_dbConfig.m_password))
	{
		CLogger::instance()->write_log(LOG_LEVEL_ERR, "读取数据库密码配置失败");
		return false;
	}
	//读取数据库连接字符串
	if (!reader.getStringValue("database.connection", m_instance.m_dbConfig.m_connection))
	{
		CLogger::instance()->write_log(LOG_LEVEL_ERR, "读取数据库连接字符串置失败");
		return false;
	}
	//读取数据库连接池最小连接数
	if (!reader.getIntValue("database.poolMinConn", m_instance.m_dbConfig.m_poolMinConn))
	{
		CLogger::instance()->write_log(LOG_LEVEL_ERR, "读取数据库连接池最小连接数失败");
		return false;
	}
	//读取数据库连接池最大连接数
	if (!reader.getIntValue("database.poolMaxConn", m_instance.m_dbConfig.m_poolMaxConn))
	{
		CLogger::instance()->write_log(LOG_LEVEL_ERR, "读取数据库连接池最大连接数失败");
		return false;
	}
	//读取数据库连接池连接数增加步长
	if (!reader.getIntValue("database.poolIncrConn", m_instance.m_dbConfig.m_poolIncrConn))
	{
		CLogger::instance()->write_log(LOG_LEVEL_ERR, "读取数据库连接池连接数增加步长失败");
		return false;
	}
	//读取业务处理线程个数
	if (!reader.getIntValue("process.threadNum", m_instance.m_processConfig.m_threadNum))
	{
		CLogger::instance()->write_log(LOG_LEVEL_ERR, "读取业务处理线程数配置失败");
		return false;
	}
	//读取默认库ID
	if (!reader.getStringValue("process.defaultGroupId", m_instance.m_processConfig.m_defaultGroupId))
	{
		CLogger::instance()->write_log(LOG_LEVEL_ERR, "读取默认库ID配置失败");
		return false;
	}

	std::string url;
	//读取webservice地址
	if (!reader.getStringValue("process.webserviceUrls", url))
	{
		CLogger::instance()->write_log(LOG_LEVEL_ERR, "读取webservice地址失败");
		return false;
	}
	else
	{
		m_instance.m_processConfig.m_webSrvList.push_back(url);
		CLogger::instance()->write_log(LOG_LEVEL_INFO, "webservice地址为:%s", url.c_str());
	}

	//读取默认图片路径
	if (!reader.getStringValue("storage.root", m_storageConfig.m_rootPath))
	{
		CLogger::instance()->write_log(LOG_LEVEL_ERR, "读取图片存储路径失败");
	}
	else
	{
		CLogger::instance()->write_log(LOG_LEVEL_INFO, "当前图片存储路径为:%s", m_storageConfig.m_rootPath.c_str());
	}

	return true;
}

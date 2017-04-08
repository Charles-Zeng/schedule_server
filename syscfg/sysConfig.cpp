#include "SysConfig.h"
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

	//��ȡhttp�˿�����
	if (!reader.getIntValue("httpsvr.port", m_instance.m_httpConfig.m_httpPort))
	{
		CLogger::instance()->write_log(LOG_LEVEL_ERR, "��ȡhttp�������˿�����ʧ��");
		return false;
	}

	//��ȡ���ݿ��û���
	if(!reader.getStringValue("database.username", m_instance.m_dbConfig.m_username))
	{
		CLogger::instance()->write_log(LOG_LEVEL_ERR, "��ȡ���ݿ��û�������ʧ��");
		return false;
	}
	//��ȡ���ݿ�����
	if (!reader.getStringValue("database.password", m_instance.m_dbConfig.m_password))
	{
		CLogger::instance()->write_log(LOG_LEVEL_ERR, "��ȡ���ݿ���������ʧ��");
		return false;
	}
	//��ȡ���ݿ������ַ���
	if (!reader.getStringValue("database.connection", m_instance.m_dbConfig.m_connection))
	{
		CLogger::instance()->write_log(LOG_LEVEL_ERR, "��ȡ���ݿ���������ʧ��");
		return false;
	}

	return true;
}

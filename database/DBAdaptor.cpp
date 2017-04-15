#include "DBAdaptor.h"

#include <logger/logger.h>

using namespace oracle::occi;

DBAdaptor* DBAdaptor::m_instance = NULL;
boost::mutex DBAdaptor::m_mutex;



DBAdaptor::DBAdaptor()
	:m_env(NULL), m_connPool(NULL)
{

}

DBAdaptor::~DBAdaptor()
{
	m_env->terminateStatelessConnectionPool(m_connPool);
	Environment::terminateEnvironment(m_env);
	m_connPool = NULL;
	m_env = NULL;
}

DBAdaptor* DBAdaptor::instance()
{
	if (NULL == m_instance)
	{
		boost::mutex::scoped_lock lock_gard(m_mutex);
		if (NULL == m_instance)
		{
			m_instance = new DBAdaptor();
		}
	}

	return m_instance;
}

bool DBAdaptor::init( const std::string& username,
					 const std::string& password,
					 const std::string& connStr,
					 std::size_t poolMinConn,
					 std::size_t poolMaxConn,
					 std::size_t poolIncrConn )
{
	if (NULL != m_env)
	{
		return false;
	}

	m_userName = username;
	m_password = password;
	m_connectString = connStr;
	m_minConn = poolMinConn;
	m_maxConn = poolMaxConn;
	m_incrConn = poolIncrConn;

	try
	{
		m_env = Environment::createEnvironment(Environment::THREADED_MUTEXED);

		m_connPool = m_env->createStatelessConnectionPool(
								m_userName,
								m_password,
								m_connectString,
								m_minConn,
								m_maxConn,
								m_incrConn);
	}
	catch (SQLException &e)
	{
		CLogger::instance()->write_log(LOG_LEVEL_ERR, "初始化数据库失败：%s", e.what());
		return false;
	}
	catch(std::exception &e)
	{		
		CLogger::instance()->write_log(LOG_LEVEL_ERR, "初始化数据库失败：%s", e.what());
		return false;
	}

	return true;
}

oracle::occi::Connection* DBAdaptor::getConnection()
{
	return m_connPool->getConnection();
}

void DBAdaptor::terminateConnection( oracle::occi::Connection* conn )
{
	m_connPool->releaseConnection(conn);
}



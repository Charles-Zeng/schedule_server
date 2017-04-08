#ifndef _DBADAPTOR_H_
#define _DBADAPTOR_H_

#include <string>

#include <boost/thread/mutex.hpp>
#include <boost/noncopyable.hpp>

#include <occi.h>

class DBAdaptor
{
public:
	static DBAdaptor* instance();

	bool init(const std::string& username,
		      const std::string& password,
			  const std::string& connStr,
			  std::size_t poolMinConn,
			  std::size_t poolMaxConn,
			  std::size_t poolIncrConn);

	oracle::occi::Connection* getConnection();
	void terminateConnection(oracle::occi::Connection* conn);

private:
	DBAdaptor();
	~DBAdaptor();
	DBAdaptor(const DBAdaptor&);
	DBAdaptor& operator=(const DBAdaptor&);
private:
	static DBAdaptor* m_instance;	
	static boost::mutex m_mutex;

	std::string m_userName;
	std::string m_password;
	std::string m_connectString;
	std::size_t m_minConn;
	std::size_t m_maxConn;
	std::size_t m_incrConn;

	oracle::occi::Environment *m_env;
	oracle::occi::ConnectionPool *m_connPool;
};


class ConnectionObj : public boost::noncopyable
{
public:
	oracle::occi::Connection *conn;

	ConnectionObj() : conn(DBAdaptor::instance()->getConnection())
	{
	}

	~ConnectionObj()
	{
		DBAdaptor::instance()->terminateConnection(conn);
	}
};

#endif // _DBADAPTOR_H_


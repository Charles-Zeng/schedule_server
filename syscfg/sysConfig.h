#pragma once

#include <string>
#include <vector>

typedef struct stHttpConfig
{
	int m_httpPort;	//http服务端口
}HttpConfig;

typedef struct stDbConfig
{
	std::string m_username;	//数据库用户名
	std::string m_password;	//数据库密码
	std::string m_connection;	//连接字符串
	int m_poolMinConn;
	int m_poolMaxConn;
	int m_poolIncrConn;
}DbConfig;

typedef struct stStorageConfig
{
	std::string m_rootPath;
}StorageConfig;

typedef struct stProcessConfig
{
	int m_threadNum;
	std::string m_defaultGroupId;
	std::vector<std::string> m_webSrvList;
}ProcessConfig;

class CSysConfig
{
private:
	CSysConfig();
	static CSysConfig m_instance;
public:
	virtual ~CSysConfig();
	static CSysConfig& instance();
	bool loadConfig();
public:
	HttpConfig m_httpConfig;
	DbConfig m_dbConfig;
	ProcessConfig m_processConfig;
	stStorageConfig m_storageConfig;
};


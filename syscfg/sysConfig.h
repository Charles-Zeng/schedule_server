#pragma once

#include <string>

typedef struct stHttpConfig
{
	int m_httpPort;	//http����˿�
}HttpConfig;

typedef struct stDbConfig
{
	std::string m_username;	//���ݿ��û���
	std::string m_password;	//���ݿ�����
	std::string m_connection;	//�����ַ���
}DbConfig;

class CSysConfig
{
private:
	CSysConfig();
	static CSysConfig m_instance;
public:
	virtual ~CSysConfig();
	static CSysConfig& instance();
	bool loadConfig();
private:
	HttpConfig m_httpConfig;
	DbConfig m_dbConfig;
};


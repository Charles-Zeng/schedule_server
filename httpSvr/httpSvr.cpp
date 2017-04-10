#include "httpSvr.h"
#include "../logger/logger.h"
#include <pthread.h>
#include "../json/jsonParser.h"
#include <string.h>

CHttpSvr::CHttpSvr()
{
}


CHttpSvr::~CHttpSvr()
{
}

bool CHttpSvr::init(int port)
{
	//如果已经开启服务
	if (m_pHttpDaemon)
	{
		return true;
	}

	//启动http服务
	m_pHttpDaemon = MHD_start_daemon(MHD_USE_EPOLL_INTERNALLY_LINUX_ONLY,
		port, NULL, NULL, httpCallBack, this, MHD_OPTION_END);

	return m_pHttpDaemon ? true : false;

}

void CHttpSvr::finit()
{
	if (m_pHttpDaemon)
	{
		MHD_stop_daemon(m_pHttpDaemon);
		m_pHttpDaemon = NULL;
	}
}

int CHttpSvr::httpCallBack(void *cls,
	MHD_Connection *connection,
	const char* url,
	const char* method,
	const char* version,
	const char* upload_data,
	size_t *upload_data_size,
	void ** ptr)
{
	if (!strcasecmp(method, "POST"))
	{
		return MHD_NO;
	}
	static int used;
	if (*ptr != &used)
	{
		*ptr = &used; 
		CLogger::instance()->write_log(LOG_LEVEL_INFO, "收到Http请求,url=%s, method=%s, version=%s, upload_data=%s",
			url, method, version, upload_data);
		//CJsonParser::parseUploadImageJson(upload_data);
		pthread_t thr;
		pthread_create(&thr, NULL, testThread, connection);
		return MHD_YES;
	}
	return MHD_YES;
}

MHD_Response* CHttpSvr::buildRespons(int code, const std::string& message, const std::string& listMatchValues)
{
	MHD_Response *response = MHD_create_response_from_data(message.length(), (void*)message.c_str(),
		MHD_NO, MHD_YES);
	if (NULL == response)
	{
		return NULL;
	}
	MHD_add_response_header(response, MHD_HTTP_HEADER_CONTENT_TYPE, "application/json");
	return response;
}

void* CHttpSvr::testThread(void* arg)
{
	MHD_Connection *connection = (MHD_Connection*)arg;
	MHD_Response *response = buildRespons(1, "111", "222");
	if (response)
	{
		MHD_queue_response(connection, MHD_HTTP_OK, response);
		MHD_destroy_response(response);
	}
}
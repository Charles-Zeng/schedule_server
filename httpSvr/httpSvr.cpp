#include "httpSvr.h"
#include "../logger/logger.h"
#include "../json/jsonParser.h"
#include <string.h>
#include "../dataQueue/dataQueue.h"

CHttpSvr::CHttpSvr()
{
	m_httpTypes["/uploadImage"] = E_HTTP_UPLOAD_IMAGE;
	m_httpTypes["/addGroupId"] = E_HTTP_ADD_GROUP;
	m_httpTypes["/deleteGroupId"] = E_HTTP_DEL_GROUP;
	m_httpTypes["/addTemplate"] = E_HTTP_ADD_TEMPLATE;
	m_httpTypes["/deleteTemplate"] = E_HTTP_DEL_TEMPLATE;
	m_httpTypes["/oneToOne"] = E_HTTP_ONE_TO_ONE;
	m_httpTypes["/oneToN"] = E_HTTP_ONE_TO_N;
	m_pHttpDaemon = NULL;
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
	//开启应答处理线程
	if (m_pHttpDaemon)
	{
		pthread_create(&m_thr, NULL, respProcThr, this);
	}
	
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
	CHttpSvr *pThis = (CHttpSvr*)cls;
	if (strcmp(method, "POST"))
	{
		return MHD_NO;
	}
	if (pThis->m_httpTypes.find(url) == pThis->m_httpTypes.end())
	{
		return MHD_NO;
	}
	static int used;
	if (*ptr != &used)
	{
		*ptr = &used; 
		CLogger::instance()->write_log(LOG_LEVEL_INFO, "收到Http请求,url=%s, method=%s",
			url, method);
		const char* body = MHD_lookup_connection_value(connection, MHD_POSTDATA_KIND, NULL);
		HttpRequest req;
		req.connection = connection;
		req.httpBody = body;
		req.httpType = pThis->m_httpTypes[url];
		CDataQueue& dataQueue = CDataQueue::instance();
		if (req.httpType == E_HTTP_ONE_TO_ONE || req.httpType == E_HTTP_ONE_TO_N)
		{
			dataQueue.pushHttpReq(req, true);
		}
		else
		{
			dataQueue.pushHttpReq(req);
		}
		return MHD_YES;
	}
	return MHD_YES;
}

void * CHttpSvr::respProcThr(void *arg)
{
	CHttpSvr* pThis = (CHttpSvr*)arg;
	CDataQueue& dataQueue = CDataQueue::instance();
	for (;;)
	{
		HttpResponse resp;
		if (!dataQueue.getHttpResp(resp))
		{
			usleep(5000);
			continue;
		}
		MHD_Connection *connection = (MHD_Connection*)resp.connection;
		pThis->buildRespons(connection, resp.bSuccess, resp.httpBody);
	}
	return NULL;
}

void CHttpSvr::buildRespons(MHD_Connection * connection, bool bSuccess, const std::string & strBody)
{
	MHD_Response *response = MHD_create_response_from_data(strBody.length(), (void*)strBody.c_str(), MHD_NO, MHD_YES);
	MHD_add_response_header(response, MHD_HTTP_HEADER_CONTENT_TYPE, "application/json");
	MHD_queue_response(connection, bSuccess ? MHD_HTTP_OK : MHD_HTTP_INTERNAL_SERVER_ERROR, response);
	MHD_destroy_response(response);
}

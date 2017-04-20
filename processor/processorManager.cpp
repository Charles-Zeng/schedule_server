#include "processorManager.h"

#include <boost/bind.hpp>

#include <dataQueue/dataQueue.h>
#include <logger/logger.h>

//processor header
#include "processUploadImage.h"
#include "processAddGroup.h"
#include "processDelGroup.h"
#include "processAddTemplate.h"
#include "processDelTemplate.h"
#include "processOneToOne.h"
#include "processOneToN.h"


ProcessorManager::ProcessorManager()
	: m_isRun(false), m_threadNum(0)
{

}

ProcessorManager::~ProcessorManager()
{
	m_isRun = false;
	m_threadPool.join_all();
}

bool ProcessorManager::init( std::size_t threadNum )
{
	if (m_isRun)
	{
		return false;
	}

	m_threadNum = threadNum;
	m_isRun = true;

	for (std::size_t i = 0; i < m_threadNum; ++i)
	{
		m_threadPool.create_thread(boost::bind(&ProcessorManager::run, this));
	}

	return true;
}

void ProcessorManager::run()
{
	while (m_isRun)
	{
		HttpRequest req;
		HttpResponse resp;

		//队列改成条件变量阻塞队列
		if (!CDataQueue::instance().getHttpReq(req))
		{
			boost::this_thread::sleep(boost::posix_time::seconds(1));
			continue;
		}

		processHttpReq(req, resp);
		resp.connection = req.connection;

		CDataQueue::instance().pushHttpResp(resp, isPriorityReq(req.httpType));
	}

}

void ProcessorManager::processHttpReq( const HttpRequest& req, HttpResponse& resp )
{
	if (req.httpBody.size() < 1024)
	{
		CLogger::instance()->write_log(LOG_LEVEL_INFO, "收到http请求,httpType=%d, httpBody=%s",
			req.httpType, req.httpBody.c_str());
	}
	

	switch (req.httpType)
	{
	case E_HTTP_UPLOAD_IMAGE:
		{
			ProcessUploadImage processUploadImage;
			processUploadImage.process(req, resp);
			break;
		}
	case E_HTTP_ADD_GROUP:
		{
			ProcessAddGroup addGroup;
			addGroup.process(req, resp);
			break;
		}
	case E_HTTP_DEL_GROUP:
		{
			ProcessDelGroup delGroup;
			delGroup.process(req, resp);
			break;
		}
	case E_HTTP_ADD_TEMPLATE:
		{
			ProcessAddTemplate addTemplate;
			addTemplate.process(req, resp);
			break;
		}
	case E_HTTP_DEL_TEMPLATE:
		{
			ProcessDelTemplate delTemplate;
			delTemplate.process(req, resp);
			break;
		}
	case E_HTTP_ONE_TO_ONE:
		{
			ProcessOneToOne oneToOne;
			oneToOne.process(req, resp);
			break;
		}
	case E_HTTP_ONE_TO_N:
		{
			ProcessOneToN oneToN;
			oneToN.process(req, resp);
			break;
		}
	default:
		{
			CLogger::instance()->write_log(LOG_LEVEL_ERR, "未知的http请求类型 %d", req.httpType);
			break;
		}
	}

	CLogger::instance()->write_log(LOG_LEVEL_INFO, "返回http响应,httpBody=%s",
		resp.httpBody.c_str());
}

bool ProcessorManager::isPriorityReq( E_HTTP_TYPE reqType )
{
	if (E_HTTP_ONE_TO_ONE == reqType || E_HTTP_ONE_TO_N == reqType)
	{
		return true;
	}

	return false;
}



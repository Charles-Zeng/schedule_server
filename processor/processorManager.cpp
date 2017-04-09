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
			CLogger::instance()->write_log(LOG_LEVEL_ERR, "processor module, %s:%d: get http request failed", __FILE__, __LINE__);
			boost::this_thread::sleep(boost::posix_time::seconds(5));
			continue;
		}

		processHttpReq(req, resp);

		CDataQueue::instance().pushHttpResp(resp, isPriorityReq(req.httpType));
	}

}

void ProcessorManager::processHttpReq( const HttpRequest& req, HttpResponse& resp )
{
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
			break;
		}
	case E_HTTP_DEL_GROUP:
		{
			break;
		}
	case E_HTTP_ADD_TEMPLATE:
		{
			break;
		}
	case E_HTTP_DEL_TEMPLATE:
		{
			break;
		}
	case E_HTTP_ONE_TO_ONE:
		{
			break;
		}
	case E_HTTP_ONE_TO_N:
		{
			break;
		}
	default:
		break;
	}
}

bool ProcessorManager::isPriorityReq( E_HTTP_TYPE reqType )
{
	if (E_HTTP_ONE_TO_ONE == reqType || E_HTTP_ONE_TO_N == reqType)
	{
		return true;
	}

	return false;
}



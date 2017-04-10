#include "dataQueue.h"

CDataQueue CDataQueue::m_instance;

CDataQueue::CDataQueue()
{
}


CDataQueue::~CDataQueue()
{
}

CDataQueue& CDataQueue::instance()
{
	return m_instance;
}

bool CDataQueue::pushHttpReq(const HttpRequest& req, bool front)
{
	return m_queueReq.push(req, front);
}

bool CDataQueue::getHttpReq(HttpRequest& req)
{
	return m_queueReq.pop(req);
}

bool CDataQueue::pushHttpResp(const HttpResponse &resp, bool front)
{
	return m_queueResp.push(resp, front);
}

bool CDataQueue::getHttpResp(HttpResponse& resp)
{
	return m_queueResp.pop(resp);
}

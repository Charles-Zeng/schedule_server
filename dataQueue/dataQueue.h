#pragma once

#include "../tool/synclist.h"
#include "../common/dataStruct.h"

class CDataQueue
{
private:
	CDataQueue();
	static CDataQueue m_instance;
public:
	static CDataQueue& instance();
	virtual ~CDataQueue();
	static CDataQueue& instance();
	bool pushHttpReq(const HttpRequest& req, bool front = false);
	bool getHttpReq(HttpRequest& req);
	bool pushHttpResp(const HttpResponse &resp, bool front = false);
	bool getHttpResp(HttpResponse& resp);
private:
	CSyncList<HttpRequest> m_queueReq;
	CSyncList<HttpResponse> m_queueResp;
};


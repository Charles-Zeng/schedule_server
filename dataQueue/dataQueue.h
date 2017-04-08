#pragma once

#include "../tool/synclist.h"
#include "../common/dataStruct.h"

class CDataQueue
{
private:
	CDataQueue();
	static CDataQueue m_instance;
	static CDataQueue& instance();
public:
	virtual ~CDataQueue();
	bool pushHttpReq(const HttpRequest& req, bool front = false);
	bool getHttpReq(HttpRequest& req);
	bool pushHttpResp(const httpResponse &resp, bool front = false);
	bool getHttpResp(httpResponse& resp);
private:
	CSyncList<HttpRequest> m_queueReq;
	CSyncList<httpResponse> m_queueResp;
};


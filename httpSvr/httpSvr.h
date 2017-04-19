#pragma once

#include <string>
#include <string.h>
#include <map>
#include <event.h>
#include <evhttp.h>
#include "../common/dataStruct.h"

class CHttpSvr
{
public:
	CHttpSvr();
	virtual ~CHttpSvr();
    void init(int port = 80);
protected:
    void __init(int port);
    static void httpd_handler(evhttp_request *req, void *arg);
    void sendHttpResp(evhttp_request *req, int code, const std::string& body);
    void processHttpResp();
private:
    std::map<std::string, E_HTTP_TYPE> m_httpTypes;
};

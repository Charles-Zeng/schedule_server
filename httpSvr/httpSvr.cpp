#include "httpSvr.h"
#include "../logger/logger.h"
#include "../json/jsonParser.h"
#include <string.h>
#include "../dataQueue/dataQueue.h"
#include "../processor/processorManager.h"
#include <boost/thread.hpp>

CHttpSvr::CHttpSvr()
{
    m_httpTypes["/uploadImage"] = E_HTTP_UPLOAD_IMAGE;
    m_httpTypes["/addGroupId"] = E_HTTP_ADD_GROUP;
    m_httpTypes["/deleteGroupId"] = E_HTTP_DEL_GROUP;
    m_httpTypes["/addTemplate"] = E_HTTP_ADD_TEMPLATE;
    m_httpTypes["/deleteTemplate"] = E_HTTP_DEL_TEMPLATE;
    m_httpTypes["/oneToOne"] = E_HTTP_ONE_TO_ONE;
    m_httpTypes["/oneToN"] = E_HTTP_ONE_TO_N;
	m_httpTypes["/getFaceInfo"] = E_HTTP_GET_FACE_INFO;   //暴露获取人脸信息接口
}


CHttpSvr::~CHttpSvr()
{
}

void CHttpSvr::init(int port)
{
    boost::thread thr_init(boost::bind(&CHttpSvr::__init, this, _1), port);
    boost::thread thr_proc(boost::bind(&CHttpSvr::processHttpResp, this));
}

void CHttpSvr::__init(int port)
{
    event_init();
    struct evhttp *httpd;
    httpd = evhttp_start("0.0.0.0", port);
	if (!httpd)
	{
		CLogger::instance()->write_log(LOG_LEVEL_ERR, "启动http服务失败");
		return;
	}
    evhttp_set_gencb(httpd, httpd_handler, this);
    event_dispatch();
}

void CHttpSvr::httpd_handler(evhttp_request *req, void *arg)
{
    CHttpSvr *pThis = static_cast<CHttpSvr*>(arg);
    const char *uri = evhttp_request_uri(req);
    if(evhttp_request_get_command(req) == EVHTTP_REQ_POST)
    {
        if(pThis->m_httpTypes.find(uri) == pThis->m_httpTypes.end())
        {
            pThis->sendHttpResp(req, HTTP_BADREQUEST, "Invalid url");
            return;
        }
        char *post_data = (char *) EVBUFFER_DATA(req->input_buffer);
        HttpRequest httpReq;
        httpReq.connection = req;
        httpReq.httpBody = post_data;
        httpReq.httpType = pThis->m_httpTypes[uri];
        CDataQueue::instance().pushHttpReq(httpReq,
                                           httpReq.httpType == E_HTTP_ONE_TO_ONE || httpReq.httpType == E_HTTP_ONE_TO_N ? true : false);
    }
    else
    {
        pThis->sendHttpResp(req, HTTP_BADREQUEST, "Unsupport method");
    }
}

void CHttpSvr::sendHttpResp(evhttp_request *req, int code, const string &body)
{
    evbuffer *buf = evbuffer_new();
    evbuffer_add_printf(buf, "%s", body.c_str());
    evhttp_add_header(req->output_headers, "Content-Type", "application/json");
    evhttp_add_header(req->output_headers, "Connection", "close");
    evhttp_send_reply(req, code, "", buf);
    evbuffer_free(buf);
}

void CHttpSvr::processHttpResp()
{
    for(;;)
    {
        HttpResponse resp;
        if(!CDataQueue::instance().getHttpResp(resp))
        {
            boost::this_thread::sleep(boost::posix_time::milliseconds(50));
            continue;
        }
        sendHttpResp(static_cast<evhttp_request*>(resp.connection),
                     resp.bSuccess ? HTTP_OK : HTTP_INTERNAL, resp.httpBody.c_str());
    }
}

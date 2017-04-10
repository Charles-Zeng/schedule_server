#pragma once

#include <string>
#include <string.h>
#include <map>
#include "../common/dataStruct.h"
#include <pthread.h>

#ifdef __cplusplus
extern "C" {
#endif
#include <microhttpd.h>
#ifdef __cplusplus
}
#endif // __cplusplus


class CHttpSvr
{
public:
	CHttpSvr();
	virtual ~CHttpSvr();
	bool init(int port = 80);
	void finit();
protected:
	static int httpCallBack(void *cls,
		MHD_Connection *connection,
		const char* url,
		const char* method,
		const char* version,
		const char* upload_data,
		size_t *upload_data_size,
		void ** ptr);
	static void* respProcThr(void*);
	void buildRespons(MHD_Connection *connection, bool bSuccess, const std::string& strBody);
private:
	MHD_Daemon *m_pHttpDaemon;
	std::map<std::string, E_HTTP_TYPE> m_httpTypes;
	pthread_t m_thr;
};
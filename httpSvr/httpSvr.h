#pragma once

#include <string>
#include <string.h>

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
	static MHD_Response* buildRespons(int code, const std::string& message, const std::string& listMatchValues);
	static void* testThread(void* arg);
private:
	MHD_Daemon *m_pHttpDaemon;
};
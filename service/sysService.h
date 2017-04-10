#pragma once

#include "../httpSvr/httpSvr.h"

class CSysService
{
public:
	CSysService();
	~CSysService();

	bool start();
private:
	CHttpSvr m_httpSvr;
};


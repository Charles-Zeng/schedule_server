#include "MygSoapProcess.h"
#include "BasicHttpBinding_USCOREIFACE.nsmap"
#include "soapBasicHttpBinding_USCOREIFACEProxy.h"
#include "stdsoap2.h"
#include <sys/types.h>
#include <stdio.h> 
#include <locale.h>
#include <stdlib.h>
#include <logger/logger.h>

MygSoapProcess::MygSoapProcess()
{
	 setlocale(LC_CTYPE,setlocale(LC_ALL,""));
}

MygSoapProcess::~MygSoapProcess()
{
	
}

std::wstring MygSoapProcess::utf8ToUnicode(const std::string& strUtf8)
{
	size_t length = strUtf8.length();
	wchar_t *szDstTemp = new wchar_t[length+1];
	wmemset(szDstTemp, 0, length + 1);
	mbstowcs(szDstTemp, strUtf8.c_str(), length);
	std::wstring strUnicode = szDstTemp;
	delete[] szDstTemp; // 析构该内存
	return strUnicode;
}

std::string MygSoapProcess::unicodeToUtf8(const std::wstring& strUnicode)
{
	size_t length = strUnicode.length();
	char *psText = new char[length*3+1];
	memset(psText, 0, length * 3 + 1);
	wcstombs(psText, strUnicode.c_str(), length*3);
	std::string strUtf8 = psText;
	delete[] psText;	// psText的清除
	return strUtf8;
}

//FaceServiceAPI实现
bool MygSoapProcess::FaceServiceAPI(const std::string& ReqType, const std::string& ReqJson, std::string& strResult)
{
	//请求类型为空 直接返回，不处理
	if ("" == ReqType)
	{
		return false;
	}
	const char server[] = "http://192.168.0.156:6666/FACE?wsdl"; //服务器地址
	BasicHttpBinding_USCOREIFACEProxy gsoapFace;                 //代理对象
	gsoapFace.soap_endpoint = server;							 //连接服务器地址，绑定服务地址
	_ns1__FaceService ReqObject;								 //请求对象
	_ns1__FaceServiceResponse RepObject;						 //返回对象
	std::wstring strType, strJson;
	strType = utf8ToUnicode(ReqType);
	strJson = utf8ToUnicode(ReqJson);
	ReqObject.type = (wchar_t*)strType.c_str();
	ReqObject.json = (wchar_t*)strJson.c_str();
	if (SOAP_OK == gsoapFace.FaceService(&ReqObject, RepObject))
	{
		strResult = unicodeToUtf8(RepObject.FaceServiceResult);
		CLogger::instance()->write_log(LOG_LEVEL_INFO, "webservice: resp json %s", strResult.c_str());
		return true;
	}
	else 
	{
		return false;
	}
}

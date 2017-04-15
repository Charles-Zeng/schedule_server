#include "gsoap/MygSoapProcess.h"
#include "gsoap/BasicHttpBinding_USCOREIFACE.nsmap"
#include "gsoap/soapBasicHttpBinding_USCOREIFACEProxy.h"
#include "gsoap/stdsoap2.h"
#include "gsoap/MygSoapReqRepStruct.h"
#include "json/json.h"


MygSoapProcess::MygSoapProcess()
{
}


MygSoapProcess::~MygSoapProcess()
{
	delete[] szDstTemp; // 析构该内存
	delete[] psText;	// psText的清除
}

//string 转wchar_t *
wchar_t* MygSoapProcess::StringToWchar_tP(wchar_t *szDst, string strDes)
{
	int mystringSize = (int)(strDes.length() + 1);
	szDstTemp = new wchar_t[mystringSize];
	MultiByteToWideChar(CP_ACP, 0, strDes.c_str(), -1, szDstTemp, mystringSize);
	szDst = szDstTemp;
	return szDst;
}
//wchar_t 转string
bool MygSoapProcess::Wchar_tToString(std::string& szDst, wchar_t *DesWchar)
{
	if (DesWchar == NULL)
	{
		return false;
	}
	else
	{
		wchar_t * wText = DesWchar;
		DWORD dwNum = WideCharToMultiByte(CP_OEMCP, NULL, wText, -1, NULL, 0, NULL, FALSE);// WideCharToMultiByte的运用
		psText = new char[dwNum];
		WideCharToMultiByte(CP_OEMCP, NULL, wText, -1, psText, dwNum, NULL, FALSE);// WideCharToMultiByte的再次运用
		szDst = psText;// std::string赋值
		return true;
	}	
}

//FaceServiceAPI实现
bool MygSoapProcess::FaceServiceAPI(std::string ReqType, std::string ReqJson)
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
	ReqObject.type = StringToWchar_tP(ReqObject.type, ReqType);
	ReqObject.json = StringToWchar_tP(ReqObject.json, ReqJson);
	if (SOAP_OK == gsoapFace.FaceService(&ReqObject, RepObject))
	{
		cout << "调用成功" << endl;
		if (Wchar_tToString(szReqJson, RepObject.FaceServiceResult))
		{
			//处理
		}
	}
	else 
	{
		cout << "调用失败" << endl;
	}
	return true;
}

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
	delete[] szDstTemp; // �������ڴ�
	delete[] psText;	// psText�����
}

//string תwchar_t *
wchar_t* MygSoapProcess::StringToWchar_tP(wchar_t *szDst, string strDes)
{
	int mystringSize = (int)(strDes.length() + 1);
	szDstTemp = new wchar_t[mystringSize];
	MultiByteToWideChar(CP_ACP, 0, strDes.c_str(), -1, szDstTemp, mystringSize);
	szDst = szDstTemp;
	return szDst;
}
//wchar_t תstring
bool MygSoapProcess::Wchar_tToString(std::string& szDst, wchar_t *DesWchar)
{
	if (DesWchar == NULL)
	{
		return false;
	}
	else
	{
		wchar_t * wText = DesWchar;
		DWORD dwNum = WideCharToMultiByte(CP_OEMCP, NULL, wText, -1, NULL, 0, NULL, FALSE);// WideCharToMultiByte������
		psText = new char[dwNum];
		WideCharToMultiByte(CP_OEMCP, NULL, wText, -1, psText, dwNum, NULL, FALSE);// WideCharToMultiByte���ٴ�����
		szDst = psText;// std::string��ֵ
		return true;
	}	
}

//FaceServiceAPIʵ��
bool MygSoapProcess::FaceServiceAPI(std::string ReqType, std::string ReqJson)
{
	//��������Ϊ�� ֱ�ӷ��أ�������
	if ("" == ReqType)
	{
		return false;
	}
	const char server[] = "http://192.168.0.156:6666/FACE?wsdl"; //��������ַ
	BasicHttpBinding_USCOREIFACEProxy gsoapFace;                 //�������
	gsoapFace.soap_endpoint = server;							 //���ӷ�������ַ���󶨷����ַ
	_ns1__FaceService ReqObject;								 //�������
	_ns1__FaceServiceResponse RepObject;						 //���ض���
	ReqObject.type = StringToWchar_tP(ReqObject.type, ReqType);
	ReqObject.json = StringToWchar_tP(ReqObject.json, ReqJson);
	if (SOAP_OK == gsoapFace.FaceService(&ReqObject, RepObject))
	{
		cout << "���óɹ�" << endl;
		if (Wchar_tToString(szReqJson, RepObject.FaceServiceResult))
		{
			//����
		}
	}
	else 
	{
		cout << "����ʧ��" << endl;
	}
	return true;
}

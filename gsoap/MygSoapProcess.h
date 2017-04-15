#pragma once
#include <iostream>

//gsoap 业务逻辑处理类
//处理的接口包括
/*1.添加模板
2.删除模板
3.1:1接口
4.1:N接口
5.静态1:1接口
6.添加库类别
7.删除库类别
8.查询库类别
9.查询人脸信息*/
//对外暴露一个统一的接口 string FaceServiceAPI(string ReqType,//请求类别 string ReqJson//请求json串);
class MygSoapProcess
{
public:
	MygSoapProcess();
	~MygSoapProcess();
	bool FaceServiceAPI(std::string ReqType,/*请求类别*/ std::string ReqJson/*请求json串*/);
	std::string szReqJson;                //返回的json值
private:
	wchar_t* StringToWchar_tP(wchar_t * szDst, std::string strDes);           //string 转wchar_t
	bool Wchar_tToString(std::string& szDst, wchar_t *DesWchar);
	wchar_t *szDstTemp;                //私有变量
	char   *psText;  // psText为char*的临时数组，作为赋值给std::string的中间变量
};


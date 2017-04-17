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
	bool FaceServiceAPI(const std::string& ReqType, /*请求类别*/
						const std::string& ReqJson,/*请求json串*/
						std::string& strResult); 
	//std::string szReqJson;                //返回的json值
private:
	std::wstring utf8ToUnicode(const std::string& strUtf8);           //utf8转unicode
	std::string unicodeToUtf8(const std::wstring& strUnicode);
};


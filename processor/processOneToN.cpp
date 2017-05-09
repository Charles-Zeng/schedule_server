#include "processOneToN.h"
//#include <sys/time.h>
#include <iostream>
#include <json/jsonParser.h>
#include <templateServer/templateServerProxy.h>

void ProcessOneToN::process( const HttpRequest& req, HttpResponse& resp )
{
	Json::Value respJson;
	OneToNInfo oneToNInfo;
	if (!CJsonParser::parseOneToN(req.httpBody, oneToNInfo))
	{
		CLogger::instance()->write_log(LOG_LEVEL_ERR, "oneToN: 解析包体json失败. body: %s", req.httpBody.c_str());
		respJson["code"] = 1;
		respJson["message"] = "invalid body json";
		resp.bSuccess = true;
		resp.httpBody = respJson.toStyledString().c_str();
		return;
	}
	/*
	GetFaceInfoResp getFaceInfoResp;
	if (!TemplateServerProxy::getFaceInfo(oneToNInfo.picBase64, getFaceInfoResp))
	{
		CLogger::instance()->write_log(LOG_LEVEL_ERR, "oneToN: 获取人脸信息失败: %s", getFaceInfoResp.errorMsg.c_str());
		respJson["code"] = 1;
		respJson["message"] = getFaceInfoResp.errorMsg.c_str();
		resp.bSuccess = true;
		resp.httpBody = respJson.toStyledString().c_str();
		return;
	}
	*/
	////测试耗时长
	//timeval start_tv, end_tv;
	//gettimeofday(&start_tv, NULL);
	//printf("millisecond:%ld\n", start_tv.tv_sec * 1000 + start_tv.tv_usec / 1000);  //毫秒
	OneToNResp oneToNResp;
	if (!TemplateServerProxy::oneToN(oneToNInfo, oneToNResp))
	{
		CLogger::instance()->write_log(LOG_LEVEL_ERR, "oneToN: 静态1:N比对失败: %s", oneToNResp.errorMsg.c_str());
		respJson["code"] = 1;
		respJson["message"] = oneToNResp.errorMsg.c_str();
		resp.bSuccess = true;
		resp.httpBody = respJson.toStyledString().c_str();
		return;
	}
	//gettimeofday(&end_tv, NULL);
	//printf("millisecond:%ld\n", end_tv.tv_sec * 1000 + end_tv.tv_usec / 1000);  //毫秒
	//int timeuse = 1000000 * (end_tv.tv_sec - start_tv.tv_sec) + end_tv.tv_usec - start_tv.tv_usec;
	//printf("time: %d us\n", timeuse);

	respJson["code"] = HTTP_SUCCESS;
	respJson["message"] = "success";

	Json::Value listMachingValues;
	typedef std::list<MachingValue>::iterator IT;
	for (IT it = oneToNResp.listMachingValues.begin(); it != oneToNResp.listMachingValues.end(); ++it)
	{
		Json::Value matchValue;
		matchValue["id"] = it->id;
		matchValue["score"] = it->score;
		listMachingValues.append(matchValue);
	}
	respJson["listMachingValues"] = listMachingValues;

	resp.bSuccess = true;
	resp.httpBody = respJson.toStyledString().c_str();
}


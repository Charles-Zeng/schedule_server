#ifndef _DATA_STRUCT_H_
#define _DATA_STRUCT_H_

#include <stdint.h>
#include <string>
#include <list>
#include <vector>
#include <json/json.h>

enum Gender
{
	GENDER_OTHER,
	GENDER_MAN,
	GENDER_WOMAN
};

const int GROUP_ID_BEGAIN = 33;
const int GROUP_ID_END = 126;

const int HTTP_SUCCESS = 0;
const int HTTP_FAILED = 1;

const int GSOAP_SUCCESS = 1;
const int GSOAP_FAILED = 0;

struct ImageInfo
{
	std::string camerId;
	std::string location;
	std::string locationId; // read from db
	int64_t monitorTime;
	int gender;
	std::string imageStr;
	std::string templateId;
	std::string photoPath;
	std::string sourceId; //uuid
};

struct TemplateInfo
{
    std::string createTime;
    std::string groupId;
    int gender;
    std::string imageStr;
};

struct OneToOneInfo
{
    std::string pic1Base64;
    std::string pic2Base64;
};

struct OneToNInfo
{
    std::string picBase64;
    float threshold;
    int32_t count;
    std::list<int32_t> groupIds;
    int32_t gender;
};

struct AlarmParam
{
	int maxReturnNumber;
	float alarmThreshold;
	std::vector<std::string> groupIds;
};

struct SuspectAlarm
{
	std::string faceId;
	std::string monitorId;
	int64_t alarmTime;
	std::string alarmAddress;
	float similarity;
	std::string suspectState;
	std::string suspectType;
};

typedef enum _httpType
{
	E_HTTP_UPLOAD_IMAGE,
	E_HTTP_ADD_GROUP,
	E_HTTP_DEL_GROUP,
	E_HTTP_ADD_TEMPLATE,
	E_HTTP_DEL_TEMPLATE,
	E_HTTP_ONE_TO_ONE,
	E_HTTP_ONE_TO_N,
	//add liyong 20170420添加获取人脸信息接口
	E_HTTP_GET_FACE_INFO
}E_HTTP_TYPE;

struct HttpRequest
{
	void *connection;
	E_HTTP_TYPE httpType;
	std::string httpBody;
};

struct HttpResponse
{
	void *connection;
	std::string httpBody;
	bool bSuccess;
};

//dynamic 1:N request
struct DynamicOneToNReq
{
	std::string sourceId;
	std::string  pic;
	float threshold;
	int count;
	std::string groupIds;
};

struct Matche
{
	std::string sourceId;
	int id; // 比中的模板ID
	float score;
};

//dynamic 1:N response
struct DynamicOneToNResp
{
	int code;
	std::string errorMsg;
	std::list<Matche> listMatches;
};

//库类别信息
struct GroupIdInfo
{
	int id;
	std::string name;
};
//查询库类别响应
struct GetGroupIdResp
{
	int code;
	std::string errorMsg;
	std::list<GroupIdInfo> groupIdInfos;
};

//添加库响应
struct AddGroupResp
{
	int code;
	std::string errorMsg;
	std::string id;
};

//删除库响应
struct DelGroupResp
{
	int code;
	std::string errorMsg;
	std::string id;
};

//添加模板响应
struct AddTemplateResp
{
	int code;
	std::string errorMsg;
	int id;
};

//删除模板响应
struct DelTemplateResp
{
	int code;
	std::string errorMsg;
	int id;
};

struct FaceInfo
{
	std::string facePic;
	float qualityScore;
};

//add liyong 20170420 添加获取人脸信息请求
struct GetFaceInfoReq
{
	//请求人脸字符串
	std::string picBase64;
};
//获取人脸信息响应
struct GetFaceInfoResp
{
	int code;
	std::string errorMsg;
	FaceInfo faceInfo;
	//由于接口调整变化故返回请求的人脸json丢给客户端 添加字段strRespJson
	Json::Value strRespJson;
};

//1:1比对响应
struct OneToOneResp
{
	int code;
	std::string errorMsg;
	float score;
};

struct MachingValue
{
	int id;
	float score;
};

//静态1:N比对响应
struct OneToNResp
{
	int code;
	std::string errorMsg;
	std::list<MachingValue> listMachingValues;
};

#endif // _DATA_STRUCT_H_

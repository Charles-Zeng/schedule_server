#ifndef _DATA_STRUCT_H_
#define _DATA_STRUCT_H_

#include <stdint.h>
#include <string>
#include <list>

enum Gender
{
	GENDER_OTHER,
	GENDER_MAN,
	GENDER_WOMAN
};

const int GROUP_ID_BEGAIN = 33;
const int GROUP_ID_END = 126;

struct ImageInfo
{
	std::string camerId;
	std::string location;
	int64_t monitorTime;
	int gender;
	std::string imageStr;
	std::string templateId;
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
	E_HTTP_ONE_TO_N

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
	std::string errorMessage;
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

struct AddGroupResp
{
	int code;
	std::string errorMsg;
	std::string id;
};

struct DelGroupResp
{
	int code;
	std::string errorMsg;
	std::string id;
};

struct AddTemplateResp
{
	int code;
	std::string errorMsg;
	std::string id;
};

#endif // _DATA_STRUCT_H_

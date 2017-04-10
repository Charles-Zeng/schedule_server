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

struct ImageInfo
{
	std::string camerId;
	std::string location;
	int64_t monitorTime;
	int gender;
	std::string imageStr;
	std::string templateId;
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

#endif // _DATA_STRUCT_H_

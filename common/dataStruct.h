#ifndef _DATA_STRUCT_H_
#define _DATA_STRUCT_H_

#include <stdint.h>
#include <string>

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

#endif // _DATA_STRUCT_H_

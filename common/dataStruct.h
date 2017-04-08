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
	E_HTTP_UPLOAD_IMAGE
}E_HTTP_TYPE;

struct HttpRequest
{
	void *connection;
	E_HTTP_TYPE httpType;
	std::string httpBody;
};

struct httpResponse
{
	void *connection;
	E_HTTP_TYPE httpType;
	std::string httpBody;
	bool bSuccess;
};

#endif // _DATA_STRUCT_H_

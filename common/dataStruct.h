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

#endif // _DATA_STRUCT_H_

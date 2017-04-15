#pragma once

#include <uuid/uuid.h>
#include <string>

inline std::string generate_uuid_string()
{
	uuid_t guid;
	uuid_generate(guid);
	char buf[100] = {0};
	snprintf(buf, sizeof(buf),
		"%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X",
		guid[0], guid[1], guid[2], guid[3],
		guid[4], guid[5], guid[6], guid[7],
		guid[8], guid[9], guid[10], guid[11],
		guid[12], guid[13], guid[14], guid[15]);
	return buf;
}

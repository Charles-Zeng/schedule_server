#ifndef _CMD_PARSE_H_
#define _CMD_PARSE_H_

#include <unistd.h>

#define UNKNOWN 0
#define START 1
#define SHUTDOWN 2

static inline int cmdParse(int argc, char* argv[])
{
	char* opt = "su";
	char ch;
	while((ch = getopt(argc, argv, opt))!= -1)
	{
		switch(ch)
		{
			case 's':
				return START;
			case 'u':
				return SHUTDOWN;
			default:
				return UNKNOWN;
		}

		return UNKNOWN;
	}

	return UNKNOWN;
}

#endif//_CMD_PARSE_H_


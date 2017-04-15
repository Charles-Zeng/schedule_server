//////////////////////////////////////////////////////////////////////////  
///    COPYRIGHT NOTICE
///    Copyright (c) 2014, iNOKNOK
///    All rights reserved.
///
/// @file		cmdParse.h
/// @brief	控制台命令行处理头文件
///
///	对命令行参数的处理
///
/// @version	1.0
/// @author	谢崇竹
/// @date		2014.7.14
//////////////////////////////////////////////////////////////////////////  


#ifndef _CMD_PARSE_H_
#define _CMD_PARSE_H_

#include <unistd.h>

#define UNKNOWN		0
#define START				1
#define SHUTDOWN		2
#define	OPENDEBUG		3
#define CLOSEDEBUG	4

///解析控制台命令行
///@param	argc	参数个数
///@param	argv	参数数组
///@return	参数对应的功能 -s启动 -u停止
static inline int cmdParse(int argc, char* argv[])
{
	const char* opt = "sudi";
	char ch;
	while((ch = getopt(argc, argv, opt))!= -1)
	{
		switch(ch)
		{
			case 's':
				return START;
			case 'u':
				return SHUTDOWN;
			case 'd':
				return OPENDEBUG;
			case 'i':
				return CLOSEDEBUG;
			default:
				return UNKNOWN;
		}

		return UNKNOWN;
	}

	return UNKNOWN;
}

#endif//_CMD_PARSE_H_

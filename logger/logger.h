//////////////////////////////////////////////////////////////////////////  
///    COPYRIGHT NOTICE  
///    Copyright (c) 2014, iNOKNOK  
///    All rights reserved.  
///  
/// @file		logger.h
/// @brief	日志类头文件
///  
///	日志操作
///  
/// @version	1.0
/// @author	谢崇竹
/// @date		2014.7.13
//////////////////////////////////////////////////////////////////////////

#ifndef _H_LOGGER_H_
#define _H_LOGGER_H_

#include <pthread.h>
#include "../tool/synclist.h"
#include <string>

using namespace std;

///日志等级
typedef enum _log_level
{
	///一般日志
	LOG_LEVEL_INFO,
	///警告日志
	LOG_LEVEL_WARN,
	///错误日志
	LOG_LEVEL_ERR,
	///调试日志
	LOG_LEVEL_DEBUG
}LOG_LEVEL;

typedef int LOG_METHOD;
static const LOG_METHOD LOG_METHOD_CONSOLE = 0x01;	//写控制台
static const LOG_METHOD LOG_METHOD_FILE = 0x02;		//写文件

/** 
* @struct LOG_CONTENT
* @brief	日志记录数据结构
*/
typedef struct
{
	///日志等级
	///@see	LOG_LEVEL
	LOG_LEVEL lvl;
	///日志信息
	string message;
}LOG_CONTENT;

#define MAX_LOG_FILE_SIZE (10 * 1000 * 1000)
#define LOG_DIR "./log"

/** 
* @class CLogger
* @brief 日志类
*/
class CLogger
{
public:
	///取得唯一实例
	///@return	CLogger对象指针
	static CLogger* instance();
	///析构函数
	virtual ~CLogger(){pthread_mutex_destroy(&m_instance_mutex);}
	///开始记录日志
	///@param	max_log_buffer	内存中缓存的最大日志条数
	///@param	method	日志记录方式
	///@return	成功或失败
	bool start_logger(size_t max_log_buffer, LOG_METHOD method);
	///停止日志记录
	void stop_logger();
	///记录日志
	///@param	lvl	日志等级
	///@param	formate	日志内容
	void write_log(LOG_LEVEL lvl, string formate,...);
	///设置是否打印调试日志
	///@param	bDebug		true打印调试日志 false不打印
	void set_log_debug(bool bDebug);
protected:
	///构造函数
	CLogger() :m_active(false), m_bDebug(false){}
	///日志线程函数，在屏幕上打印日志并将日志压入文件日志队列
	///@param	arg	线程函数参数
	static void* logger_thread(void* arg);
	///写日志文件线程函数
	///@param	arg	线程函数参数
	static void* logger_file_thread(void* arg);
	///将LOG_LEVEL转化为字符串
	///@param	lvl	日志等级
	///@return	日志等级对应的字符串描述
	static const char* log_level_to_string(LOG_LEVEL lvl);
	///生成当前时间yyyy-mm-dd H24:MM:SS
	///@return	当前时间的字符串表示
	static string get_log_time();
	///生成当前日志文件名称
	///@return	日志文件名
	static string get_log_file_name();
	///创建日志目录
	///@return	成功或失败
	bool create_log_dir();
private:
	///CLogger对象单例
	static CLogger* m_instance;
	///是否被激活
	volatile bool m_active;
	///单例对象互斥锁
	static pthread_mutex_t m_instance_mutex;
	///存放控制台日志信息
	CSyncList<LOG_CONTENT*> m_logger_list;
	///存放文件日志信息
	CSyncList<LOG_CONTENT*> m_file_logger_list;
	///日志记录方式
	LOG_METHOD m_log_method;
	///是否打印调试日志
	volatile bool m_bDebug;
	pthread_t tid_logger_main, tid_logger_file;
};

#endif//_H_LOGGER_H_


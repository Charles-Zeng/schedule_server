//////////////////////////////////////////////////////////////////////////  
///    COPYRIGHT NOTICE  
///    Copyright (c) 2014, iNOKNOK  
///    All rights reserved.  
///  
/// @file		logger.cpp
/// @brief	日志类实现文件
///  
///	日志操作
///  
/// @version	1.0
/// @author	谢崇竹
/// @date		2014.7.13
//////////////////////////////////////////////////////////////////////////


#include "logger.h"
#include <stdarg.h>
#include <fstream>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

using namespace std;

CLogger* CLogger::m_instance = NULL;

pthread_mutex_t CLogger::m_instance_mutex = PTHREAD_MUTEX_INITIALIZER;

CLogger* CLogger::instance()
{
	if(!m_instance)
	{
		pthread_mutex_lock(&m_instance_mutex);
		if(!m_instance)
		{
			m_instance = new CLogger;
		}
		pthread_mutex_unlock(&m_instance_mutex);
		return m_instance;
	}
	
	return m_instance;
}

void* CLogger::logger_file_thread(void* arg)
{
	size_t file_size = 0;
	ofstream f;
	LOG_CONTENT* content;
	char msg_buf[4096];
	CLogger* pLogger = (CLogger*)arg;
	
	if(!pLogger)
	{
		return NULL;
	}

	CSyncList<LOG_CONTENT*>& lst = pLogger->m_file_logger_list;

	while(pLogger->m_active)
	{
		bool bRet = lst.pop(content);
		if(!bRet || !content)
		{
			usleep(50000);
			continue;
		}

		if(!f.is_open())
		{
			string str_file_name = get_log_file_name();
			f.open(str_file_name.c_str(), ios::out | ios::binary);
			if(!f.is_open())
			{
				delete content;
				continue;
			}
		}

		int len = snprintf(msg_buf, sizeof(msg_buf)-1, "[%s][%s]%s\n",
				get_log_time().c_str(),
				log_level_to_string(content->lvl),
				content->message.c_str());
		f << msg_buf;
		f.flush();
		file_size += len;

		if(file_size >= MAX_LOG_FILE_SIZE)
		{
			f.close();
			file_size = 0;
		}

		delete content;
	}

	if(f.is_open())
	{
		f.close();
	}

	while(lst.pop(content))
	{
		delete content;
	}
	
	return NULL;
}

void* CLogger::logger_thread(void* arg)
{
	LOG_CONTENT* content;
	char msg_buf[4096];
	CLogger* pLogger = (CLogger*)arg;
	
	if(!pLogger)
	{
		return NULL;
	}

	CSyncList<LOG_CONTENT*>& lst = pLogger->m_logger_list;
	
	while(pLogger->m_active)
	{
		bool bRet = lst.pop(content);
		if(!bRet || !content)
		{
			usleep(50000);
			continue;
		}
		
		if(pLogger->m_log_method | LOG_METHOD_CONSOLE)
		{
			snprintf(msg_buf, sizeof(msg_buf)-1, "[%s][%s]%s\n",
				get_log_time().c_str(),
				log_level_to_string(content->lvl),
				content->message.c_str());
			printf("%s", msg_buf);
		}
		if(pLogger->m_log_method | LOG_METHOD_FILE)
		{
			LOG_CONTENT *pNewContent = new LOG_CONTENT;
			if(!pNewContent)
			{
				delete content;
				return NULL;
			}
			pNewContent->lvl = content->lvl;
			pNewContent->message = content->message;
			if(!pLogger->m_file_logger_list.push(pNewContent))
			{
				delete pNewContent;
			}
		}
		delete content;
	}

	while(lst.pop(content))
	{
		delete content;
	}
	
	return NULL;
}


bool CLogger::start_logger(size_t max_log_buffer, LOG_METHOD method)
{
	if(m_active)
	{
		return false;
	}
	if(!create_log_dir())
	{
		return false;
	}
	m_active = true;
	if(max_log_buffer)
	{
		m_logger_list.set_max_size(max_log_buffer);
		m_file_logger_list.set_max_size(max_log_buffer);
	}
	m_log_method = method;

	if(pthread_create(&tid_logger_main, NULL, logger_thread, this))
	{
		m_active = false;
		return false;
	}
	//pthread_detach(tid_logger_main);
	
	if(pthread_create(&tid_logger_file, NULL, logger_file_thread, this))
	{
		m_active = false;
		return false;
	}
	//pthread_detach(tid_logger_file);
	
	return true;
}

void CLogger::stop_logger()
{
	m_active = false;
	pthread_join(tid_logger_main, NULL);
	pthread_join(tid_logger_file, NULL);
}

void CLogger::write_log(LOG_LEVEL lvl, string formate,...)
{
	//非活动状态直接返回
	if(!m_active)
	{
		return;
	}
	//如果没有设置打印调试日志
	if (!m_bDebug && LOG_LEVEL_DEBUG == lvl)
	{
		return;
	}

	char fmt_buf[4096];
	va_list arg_lst;
	va_start(arg_lst, formate);
	vsnprintf(fmt_buf, 4090, formate.c_str(), arg_lst);
	va_end(arg_lst);

	LOG_CONTENT* log_content = new LOG_CONTENT;
	if(!log_content)
	{
		return;
	}
	log_content->lvl = lvl;
	log_content->message = fmt_buf;
	if(!m_logger_list.push(log_content))
	{
		delete log_content;
	}
}

const char* CLogger::log_level_to_string(LOG_LEVEL lvl)
{
	const char *strLvl;
	switch(lvl)
	{
		case LOG_LEVEL_INFO:
			strLvl = "INFO";
			break;
		case LOG_LEVEL_WARN:
			strLvl = "WARNING";
			break;
		case LOG_LEVEL_ERR:
			strLvl = "ERROR";
			break;
		case LOG_LEVEL_DEBUG:
			strLvl = "DEBUG";
			break;
		default:
			strLvl = "UNKNOW";
	}
	return strLvl;
}

string CLogger::get_log_time()
{
	time_t time_now;
	tm tm_now;
	char str_time[50] = {0};

	time_now = time(0);
	localtime_r(&time_now, &tm_now);
	strftime(str_time, sizeof(str_time), "%F %T", &tm_now);
	return str_time;
}

string CLogger::get_log_file_name()
{
	time_t time_now;
	tm tm_now;
	char str_time[50] = {0};

	time_now = time(0);
	localtime_r(&time_now, &tm_now);
	strftime(str_time, sizeof(str_time), "/log%Y%m%d%H%M%S.log", &tm_now);
	return string(LOG_DIR) + str_time;
}

bool CLogger::create_log_dir()
{
	struct stat buf;
	int state = stat(LOG_DIR, &buf);

	if(state || !(buf.st_mode | S_IFDIR))
	{
		if(mkdir(LOG_DIR, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH))
		{
			return false;
		}
	}
	return true;
}

void CLogger::set_log_debug(bool bDebug)
{
	m_bDebug = bDebug;
}
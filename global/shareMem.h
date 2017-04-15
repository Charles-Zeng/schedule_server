//////////////////////////////////////////////////////////////////////////  
///    COPYRIGHT NOTICE
///    Copyright (c) 2014, iNOKNOK
///    All rights reserved.
///
/// @file		shareMem.h
/// @brief	共享内存头文件
///
///	对共享内存的处理
///
/// @version	1.0
/// @author	谢崇竹
/// @date		2014.7.14
//////////////////////////////////////////////////////////////////////////  


#ifndef _SHARE_MEM_H_
#define _SHARE_MEM_H_

#include <sys/shm.h>
#include <time.h>

/** 
* @struct share_memory
* @brief 共享内存中存放的数据结构
*/
typedef struct
{
	int run;
	bool log_debug;
	time_t last_access;
}share_memory;

///得到字符串的哈希值
///@param	str	字符串指针
///@return	字符串的哈希值
unsigned int string_hash(char *str)
{
    unsigned int hash = 0;
 
    while (*str)
    {
        hash = (*str++) + (hash << 6) + (hash << 16) - hash;
    }
 
    return (hash & 0x7FFFFFFF);
}

///获取共享内存的key
///@return	共享内存key值
static inline unsigned int get_share_mem_key()
{
	#define MAX_PATH_LEN 256
	pid_t pid = getpid();
	char proc[MAX_PATH_LEN] = {0};
	char program[MAX_PATH_LEN] = {0};
	size_t program_len = 0;
	sprintf(proc, "/proc/%d/exe", pid);
	program_len = readlink(proc, program, sizeof(program));
	if((size_t)-1 == program_len)
	{
		return (unsigned int)-1;
	}

	return string_hash(program);
}

///创建共享内存
///@param	shmid	共享内存id
///@param	create	打开方式 0表示打开已有，非0表示创建并打开
///@return	共享内存指针
static inline void* create_share_memory(int* shmid, int create)
{
	void *shm = NULL;
	int shmflag;
	if(create)
	{
		shmflag = 0666|IPC_CREAT;
	}
	else
	{
		shmflag = 0666;
	}
	
	*shmid = shmget((key_t)get_share_mem_key(), sizeof(share_memory), shmflag);
	if(*shmid == -1)
    {
        return NULL;
    }

	shm = shmat(*shmid, 0, 0); 
	if(shm == (void*)-1)
	{
		return NULL;
	}

	return shm;
}

///销毁共享内存
///@param	shm		共享内存地址
///@param	shmid	共享内存id
static inline int destroy_share_memory(void* shm, int shmid)
{
	if(shmdt(shm) == -1)
	{
		return -1;
	}

	if(shmctl(shmid, IPC_RMID, 0) == -1)
	{
		return -1;
	}

	return 0;
}


#endif//_SHARE_MEM_H_


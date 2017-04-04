#ifndef _SHARE_MEM_H_
#define _SHARE_MEM_H_

#include <sys/shm.h>
#include <time.h>

#define SHARE_MEM_KEY get_share_mem_key()

typedef struct _share_memory_st
{
	int run;
	time_t last_access;
}share_memory;

unsigned int string_hash(char *str)
{
    unsigned int hash = 0;
 
    while (*str)
    {
        hash = (*str++) + (hash << 6) + (hash << 16) - hash;
    }
 
    return (hash & 0x7FFFFFFF);
}


static inline unsigned int get_share_mem_key()
{
	#define MAX_PATH_LEN 256
	pid_t pid = getpid();
	char proc[MAX_PATH_LEN] = {0};
	char program[MAX_PATH_LEN] = {0};
	size_t program_len = 0;
	sprintf(proc, "/proc/%d/exe", pid);
	program_len = readlink(proc, program, sizeof(program));
	if(-1 == program_len)
	{
		return (unsigned int)-1;
	}

	return string_hash(program);
}

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
	
	*shmid = shmget((key_t)SHARE_MEM_KEY, sizeof(share_memory), shmflag);
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


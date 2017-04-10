#include <unistd.h>
#include <signal.h>
#include <sys/param.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include "global/cmdParse.h"
#include "global/shareMem.h"
#include <wait.h>
#include "logger/logger.h"
#include "syscfg/sysConfig.h"
#include "service/sysService.h"

int main(int argc, char* argv[])
{
	int pid;
	int cmd;
	int smhid;
	void* shm;

	share_memory *shm_st;

	cmd = cmdParse(argc, argv);

	if(START == cmd)
	{
		if(0 != (pid = fork()))	//父进程
		{
			return 0;
		}
		else	//第一子进程
		{
			if(pid < 0)
			{
				printf("创建第一子进程失败\n");
				return pid;
			}
		}
		
		//第一子进程执行代码
		//第一子进程成为新的会话组长和进程组长并与控制终端分离
		setsid();

		//创建共享内存
		shm = create_share_memory(&smhid, 1);
		if(!shm)
		{
			printf("创建共享内存失败\n");
			return 0;
		}

		shm_st = (share_memory*)shm;
		if(1 == shm_st->run && shm_st->last_access >= time(0) - 1)
		{
			printf("进程已经在运行\n");
			return 0;
		}
		shm_st->run = 1;
		shm_st->last_access = time(0);

	FORK:
		if((pid = fork()) > 0)	//第一子进程(监控进程)
		{
			while(shm_st->run)
			{
				if(waitpid(pid, NULL, WNOHANG))
				{
					goto FORK;
				}
				shm_st->last_access = time(0);
				usleep(100);
			}

			if(destroy_share_memory(shm, smhid) == -1)
			{
				printf("销毁共享内存失败\n");
			}

			return 0;
		}
		else
		{
			if(pid < 0)
			{
				printf("创建业务进程失败\n");
				return pid;
			}
		}

		CSysService svr;
		//业务进程执行代码
        CLogger::instance()->start_logger(1000, LOG_METHOD_CONSOLE | LOG_METHOD_FILE);
        CLogger::instance()->write_log(LOG_LEVEL_INFO, "业务进程启动...");
		//读取配置参数
		if (!CSysConfig::instance().loadConfig())
		{
			shm_st->run = 0;
		}
		else
		{
			//启动业务
			if (!svr.start())
			{
				CLogger::instance()->write_log(LOG_LEVEL_ERR, "创建系统业务失败!");
				shm_st->run = 0;
			}
		}

		sleep(2);

		while(shm_st->run)
		{
			sleep(1);
		}
		printf("业务进程已退出\n");
	}
	else if(SHUTDOWN == cmd)
	{
        CLogger::instance()->write_log(LOG_LEVEL_INFO, "关闭业务进程...");
		shm = create_share_memory(&smhid,0);
		if(!shm)
		{
			printf("无法找到共享内存,可能业务进程已退出\n");
			return 0;
		}
		shm_st = (share_memory*)shm;

		if(0 == shm_st->run || shm_st->last_access < time(0) - 1)
		{
			printf("业务进程未在运行\n");
			return 0;
		}

        shm_st->run = 0;
        CLogger::instance()->write_log(LOG_LEVEL_INFO, "进程已经退出");
	}
	else
	{
		printf("命令行参数非法\n");
	}

	return 0;
}


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

///设置调试日志标志
void set_log_debug_flag(int* smhid, bool debug)
{
	void* shm = create_share_memory(smhid, 0);
	if (!shm)
	{
		printf("无法找到共享内存,可能业务进程已退出\n");
		return;
	}
	share_memory* shm_st = (share_memory*)shm;

	shm_st->log_debug = debug;
}

///通过更改共享内存标志退出进程
///@param	smhid	共享内存id
void shutdown(int* smhid)
{
	void* shm = create_share_memory(smhid,0);
	if(!shm)
	{
		printf("无法找到共享内存,可能业务进程已退出\n");
		return;
	}
	share_memory* shm_st = (share_memory*)shm;

	if(0 == shm_st->run || shm_st->last_access < time(0) - 1)
	{
		printf("业务进程未在运行\n");
		return;
	}

	shm_st->run = 0;
}

///打印命令行参数说明
void print_usage()
{
	printf("Useage:\n");
	printf("-s	启动图像处理模块\n");
	printf("-u	停止图像处理模块\n");
	printf("-d	启用调试日志\n");
	printf("-i	关闭调试日志\n");
}

///程序入口函数
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
		if((pid = fork()) != 0)	//父进程
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
		shm_st->log_debug = false;
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
				usleep(50000);
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

		//在这里添加业务进程执行代码

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

		while(shm_st->run)
		{
			CLogger::instance()->set_log_debug(shm_st->log_debug);
			sleep(1);
		}
	}
	else if(SHUTDOWN == cmd)
	{
		shutdown(&smhid);
	}
	else if (OPENDEBUG == cmd)
	{
		set_log_debug_flag(&smhid, true);
	}
	else if (CLOSEDEBUG == cmd)
	{
		set_log_debug_flag(&smhid, false);
	}
	else
	{
		print_usage();
	}

	return 0;
}



#include <unistd.h>
#include <signal.h>
#include <sys/param.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include "cmdParse.h"
#include "shareMem.h"
#include <wait.h>

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
		if(pid = fork())	//������
		{
			return 0;
		}
		else	//��һ�ӽ���
		{
			if(pid < 0)
			{
				printf("������һ�ӽ���ʧ��\n");
				return pid;
			}
		}
		
		//��һ�ӽ���ִ�д���
		//��һ�ӽ��̳�Ϊ�µĻỰ�鳤�ͽ����鳤��������ն˷���
		setsid();

		//���������ڴ�
		shm = create_share_memory(&smhid, 1);
		if(!shm)
		{
			printf("���������ڴ�ʧ��\n");
			return 0;
		}

		shm_st = (share_memory*)shm;
		if(1 == shm_st->run && shm_st->last_access >= time(0) - 1)
		{
			printf("�����Ѿ�������\n");
			return 0;
		}
		shm_st->run = 1;
		shm_st->last_access = time(0);

	FORK:
		if((pid = fork()) > 0)	//��һ�ӽ���(��ؽ���)
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
				printf("���ٹ����ڴ�ʧ��\n");
			}

			return 0;
		}
		else
		{
			if(pid < 0)
			{
				printf("����ҵ�����ʧ��\n");
				return pid;
			}
		}

		//ҵ�����ִ�д���
		
		printf("ҵ���������...\n");
		while(shm_st->run)
		{
			sleep(1);
		}
		printf("ҵ��������˳�\n");
	}
	else if(SHUTDOWN == cmd)
	{
		printf("�ر�ҵ�����...\n");
		shm = create_share_memory(&smhid,0);
		if(!shm)
		{
			printf("�޷��ҵ������ڴ�,����ҵ��������˳�\n");
			return 0;
		}
		shm_st = (share_memory*)shm;

		if(0 == shm_st->run || shm_st->last_access < time(0) - 1)
		{
			printf("ҵ�����δ������\n");
			return 0;
		}

		shm_st->run = 0;
	}
	else
	{
		printf("�����в����Ƿ�\n");
	}

	return 0;
}


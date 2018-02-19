#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>

#include <signal.h>
#include <unistd.h>
#include <fcntl.h>

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct
{
	long mtype;
	int data[2];
} msg;

int * shmpids;

int main()
{
	key_t msgqk, smemk;
	int msgqi, smemi, pid, num, count;
	int i, t;
	msg message;
	
	dup2(open("/dev/null", O_RDONLY), 0);
	dup2(creat("servlog", 0666), 1);
	dup2(creat("serverr", 0666), 2);
	//signal(SIGINT, SIG_IGN);
	close(creat("msgq", 0666));
	close(creat("smem", 0666));
	msgqk = ftok("msgq", 's');
	smemk = ftok("smem", 's');
	msgqi = msgget(msgqk, 0666 | IPC_CREAT | IPC_EXCL);
	smemi = shmget(smemk, 1 << 16, 0666 | IPC_CREAT | IPC_EXCL);
	shmpids = calloc(1 << 8, sizeof(int));
	count = 0;
	t = 0;
	
	while (1)
	{
		msgrcv(msgqi, &message, sizeof(msg) - sizeof(long), 1, 0);
		pid = message.data[0];
		num = message.data[1];
		if (num < 0)
		{
			if (count == 256) 
			{
				message.mtype = pid;
				message.data[0] = -1;
				message.data[1] = -3;
				msgsnd(msgqi, &message, sizeof(msg) - sizeof(long), 0);
				printf("-3 %5d %5d", pid, 0);
				continue;
			}
			for (i = 0; i < 256; ++i)
				if (shmpids[i] == 0)
				{
					num = i;
					break;
				}
			shmpids[num] = pid;
			++count;
			message.mtype = pid;
			message.data[0] = smemk;
			message.data[1] = num;
			msgsnd(msgqi, &message, sizeof(msg) - sizeof(long), 0);
			printf(" 1 %5d %5d", pid, num);
			continue;
		}
		if (num >= 0 && num < 256)
		{
			if (shmpids[num] == pid)
			{
				shmpids[num] = 0;
				--count;
				message.mtype = pid;
				message.data[0] = -1;
				message.data[1] = 256;
				msgsnd(msgqi, &message, sizeof(msg) - sizeof(long), 0);
				printf(" 0 %5d %5d", pid, num);
				if (count == 0)
				{
					free(shmpids);
					shmctl(smemi, IPC_RMID, NULL);
					return 0;
				}
				continue;
			}
			if (shmpids[num] == 0)
			{
				shmpids[num] = pid;
				++count;
				message.mtype = pid;
				message.data[0] = smemk;
				message.data[1] = num;
				msgsnd(msgqi, &message, sizeof(msg) - sizeof(long), 0);
				printf(" 2 %5d %5d", pid, num);
				continue;
			}
			else
			{
				message.mtype = pid;
				message.data[0] = -1;
				message.data[1] = -1;
				msgsnd(msgqi, &message, sizeof(msg) - sizeof(long), 0);
				printf("-1 %5d %5d", pid, num);
				continue;
			}
		}
		else
		{
			message.mtype = pid;
			message.data[0] = -1;
			message.data[1] = -2;
			msgsnd(msgqi, &message, sizeof(msg) - sizeof(long), 0);
			printf("-2 %5d %5d", pid, num);
			continue;
		}
		
	}
	/*Unreachable code, return by delete all*/
}
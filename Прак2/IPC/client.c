#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>

#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

#include "client.h"

typedef struct message_
{
	long mtype;
	int data[2];
} msg;

key_t msgqk;
int msgqi, count;
char ** strs;
char * startpos;

int IPCinit(void)
{
	msgqk = ftok("msgq", 's');
	msgqi = msgget(msgqk, 0666);
	strs = calloc(256, sizeof(char *));
	count = 0;
	startpos = NULL;
	return 0;
}

int IPCexit(void)
{
	free(strs);
	return 0;
}

char * GetStr(int * pnum)
{
	msg message;
	int pid, smemi;
	key_t smemk;
	
	if ((*pnum) >= 0 && (*pnum) < 256 && strs[*pnum]) return strs[*pnum];
	pid = getpid();
	message.mtype = 1;
	message.data[0] = pid;
	message.data[1] = *pnum;
	msgsnd(msgqi, &message, sizeof(msg) - sizeof(long), 0);
	msgrcv(msgqi, &message, sizeof(msg) - sizeof(long), pid, 0);
	smemk = message.data[0];
	*pnum = message.data[1];
	if ((*pnum) >=0 && (*pnum) < 256)
	{
		if (count == 0)
		{
			smemi = shmget(smemk, 1 << 16, 0666);
			startpos = shmat(smemi, NULL, 0);
		}
		++count;
		strs[*pnum] = startpos + 256 * (*pnum);
		return strs[*pnum];
	}
	return NULL;
}

int FreeStr(int * pnum)
{
	msg message;
	int pid;
	
	if ((*pnum) >= 0 && (*pnum) < 256) strs[*pnum] = NULL;
	pid = getpid();
	message.mtype = 1;
	message.data[0] = pid;
	message.data[1] = *pnum;
	msgsnd(msgqi, &message, sizeof(msg) - sizeof(long), 0);
	msgrcv(msgqi, &message, sizeof(msg) - sizeof(long), pid, 0);
	*pnum = message.data[1];
	if (*pnum == 256)
	{
		--count;
		if (count == 0)
		{
			shmdt(startpos);
			startpos = NULL;
		}
	}
	return (*pnum) != 256;
}

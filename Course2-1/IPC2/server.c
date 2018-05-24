#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/un.h>

#include <signal.h>
#include <unistd.h>
#include <fcntl.h>

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

#define MAXSOCKS 10
#define SOCKADDR "/home/s02160069/unix/cwtf/IPC2/socket"
#define BUFSIZE 5
#define ATTEMPTS 10

#define FRETURN(CODE, TYPE) { printf("%d\n", (CODE)); if (TYPE) { shutdown(tmp, 2); close(tmp); } shutdown(socketid, 2); close(socketid); return -(CODE); }

typedef struct sockaddr sadd;
typedef struct sockaddr_un sadd_un;
typedef unsigned int uint;

int rungame(int sock)
{
	int count, i, tmp, cows, bulls;
	char num[4];
	char used[6];
	char getsend[4];
	
	memset(used, 0, 6);
	srand(time(NULL));
	for (i = 0; i < 4; ++i)
	{
		num[i] = tmp = rand() % 6;
		if (used[tmp]) --i;
		used[tmp] = 1;
	}
	
	for (count = 0; count < ATTEMPTS; ++count)
	{
		recv(sock, getsend, 4, 0);
		cows = 0;
		bulls = 0;
		for(i = 0; i < 4; ++i)
		{
			if (num[i] == getsend[i] - 1) 
				++bulls;
			else if (used[getsend[i] - 1])
				++cows;
		}
		getsend[0] = bulls;
		getsend[1] = cows;
		send(sock, getsend, 2, 0);
		if (bulls == 4) break;
	}
	return 0;
}

int main()
{
	int socketid, tmp, nothing;
	sadd_un sockinfo;
	char buf[BUFSIZE];
	
	/*dup2(open("/dev/null", O_RDONLY), 0);
	dup2(creat("servlog", 0666), 1);
	dup2(creat("serverr", 0666), 2);*/
	
	nothing = 0;
	sockinfo.sun_family = AF_UNIX;
	strcpy(sockinfo.sun_path, SOCKADDR);
	socketid = socket(AF_UNIX, SOCK_STREAM, 0);
	printf("%d %d\n", tmp = bind(socketid, (sadd *) &sockinfo, sizeof(sockinfo)), socketid);
	if (tmp)
		return 0;
	listen(socketid, MAXSOCKS);
	while(1)
	{
		tmp = accept(socketid, NULL, (uint *) &nothing);
		if (tmp == -1)
			FRETURN(-1, 0)
		if (recv(tmp, buf, 1, 0) == -1)
			FRETURN(-2, 1)
		if (buf[0] == 0)
			FRETURN(0, 1)
		printf("1, %d\n", tmp);
		if (fork() == 0)
		{
			rungame(tmp);
			shutdown(tmp, 2);
			close(tmp);
			return 0;
		}
	}
	/*UNREACHABLE*/
	if (!tmp)
	{
		shutdown(socketid, 2);
		close(socketid);
	}
	return 0;
}
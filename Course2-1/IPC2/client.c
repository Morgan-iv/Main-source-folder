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

typedef struct sockaddr sadd;
typedef struct sockaddr_un sadd_un;
typedef unsigned int uint;

int main()
{
	int socketid, tmp, i;
	sadd_un sockinfo;
	char sendget[4];
	
	sockinfo.sun_family = AF_UNIX;
	strcpy(sockinfo.sun_path, SOCKADDR);
	socketid = socket(AF_UNIX, SOCK_STREAM, 0);
	printf("%d %d\n", tmp = connect(socketid, (sadd *) &sockinfo, sizeof(sockinfo)), socketid);
	if (tmp)
		return 0;
	printf("Do you want to stop server? (1/0) ");
	scanf("%d", &tmp);
	tmp = !tmp;
	send(socketid, &tmp, 1, 0);
	if (!tmp)
	{
		shutdown(socketid, 2);
		close(socketid);
		return 0;
	}
	for(i = 0; i < ATTEMPTS; ++i)
	{
		printf("You have %2d attempt(s) left. Please enter the number:", ATTEMPTS - i);
		scanf("%*c%c%c%c%c", sendget, sendget + 1, sendget + 2, sendget + 3);
		sendget[0] -= '0';
		sendget[1] -= '0';
		sendget[2] -= '0';
		sendget[3] -= '0';
		printf("%d %d %d %d\n", (int) sendget[0], (int) sendget[1], (int) sendget[2], (int) sendget[3]);
		if (sendget[0] < 1 || sendget[0] > 6 ||
			sendget[1] < 1 || sendget[1] > 6 ||
			sendget[2] < 1 || sendget[2] > 6 ||
			sendget[3] < 1 || sendget[3] > 6 ||
			sendget[0] == sendget[1] || sendget[0] == sendget[2] ||
			sendget[0] == sendget[3] || sendget[1] == sendget[2] ||
			sendget[1] == sendget[3] || sendget[2] == sendget[3])
			{
				--i;
				printf("Wrong input! You should put the number that contains digits between 1 and 6 without repeats\n");
				continue;
			}
		send(socketid, sendget, 4, 0);
		recv(socketid, sendget, 2, 0);
		if (sendget[0] == 4)
		{
			printf("Congratulations! You won!\n");
			break;
		}
		printf("You got %d bulls and %d cows\n", sendget[0], sendget[1]);
	}
	shutdown(socketid, 2);
	close(socketid);
	return 0;
}
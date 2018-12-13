#include<string.h>
#include<unistd.h>
#include<stdlib.h>
#include<signal.h>
#include<fcntl.h>
#include<stdio.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<sys/stat.h>

int NDFA;
int BACK;
int IRED;
int ORED;
int ORTP;
int IBFO;
int argn;

unsigned int DFA (char * command, char ** args, int index)
{
	int st = 1;
	int argpos = 0;
	char c;
	NDFA = 0;
	BACK = 0;
	IRED = 0;
	ORED = 0;
	ORTP = 0;
	IBFO = 0;
	unsigned int res = 0;
	while (1)
	{
		if (st == 0)
		{
			argn = argpos;
			args[argpos] = NULL
			return index;
		}
		if (st == 1)
		{
			c = command[index++];
			if (c == '\0')
			{	--index; st = 0; continue;}
			if (c == '&')
			{	BACK = 1; st = 7; continue;}
			if (c == '|')
			{	NDFA = 1; st = 0; continue;}
			if (c == '<')
			{	IBFO = 1; IRED = 1; st = 3; continue;}
			if (c == '>')
			{	ORED = 1; st = 5; continue;}
			if (c == ' ')
				continue;
			args[argpos] = command + (index - 1); st = 2;
			continue;
		}
		if (st == 2)
		{
			c = command[index];
			if (c == '\0')
			{	++argpos; st = 0; continue;}
			if (c == '&')
			{	++argpos; command[index] = '\0'; ++index; BACK = 1; st = 7; continue;}
			if (c == '|')
			{	++argpos; command[index] = '\0'; ++index; NDFA = 1; st = 0; continue;}
			if (c == '<')
			{	++argpos; command[index] = '\0'; ++index; IBFO = 1; IRED = 1; st = 3; continue;}
			if (c == '>')
			{	++argpos; command[index] = '\0'; ++index; ORED = 1; st = 5; continue;}
			if (c == ' ')
			{	++argpos; command[index] = '\0'; ++index; st = 1; continue;}
			++index;
			continue;
		}
		if (st == 3)
		{
			c = command[index++];
			if (c == '\0')
			{	--index; st = 0; continue;}
			if (c == '&')
			{	BACK = 1; st = 7; continue;}
			if (c == '>')
			{	ORED = 1; st = 5; continue;}
			if (c == ' ')
				continue;
			args[argpos] = command + (index - 1); st = 4;
			continue;
		}
		if (st == 4)
		{
			c = command[index];
			if (c == '\0')
			{	++argpos; st = 0; continue;}
			if (c == '&')
			{	++argpos; command[index] = '\0'; ++index; BACK = 1; st = 7; continue;}
			if (c == '>')
			{	++argpos; command[index] = '\0'; ++index; ORED = 1; st = 5; continue;}
			if (c == ' ')
			{	++argpos; command[index] = '\0'; ++index; st = 3; continue;}
			++index;
			continue;
		}
		if (st == 5)
		{
			c = command[index++];
			if (c == '\0')
			{	--index; st = 0; continue;}
			if (c == '&')
			{	BACK = 1; st = 7; continue;}
			if (c == '<')
			{	IRED = 1; st = 3; continue;}
			if (c == '>')
			{	ORTP = 1; continue;}
			if (c == ' ')
				continue;
			args[argpos] = command + (index - 1); st = 6;
			continue;
		}
		if (st == 6)
		{
			c = command[index];
			if (c == '\0')
			{	++argpos; st = 0; continue;}
			if (c == '&')
			{	++argpos; command[index] = '\0'; ++index; BACK = 1; st = 7; continue;}
			if (c == '<')
			{	++argpos; command[index] = '\0'; ++index; IRED = 1; st = 3; continue;}
			if (c == ' ')
			{	++argpos; command[index] = '\0'; ++index; st = 5; continue;}
			++index;
			continue;
		}
		if (st == 7)
		{
			c = command[index++];
			if (c == '\0')
			{	--index; st = 0; continue;}
			if (c == ' ')
				continue;
		}
	}
}

int run (char * command)
{
	int index = 0, comm = 0;
	int fd[2];
	int redio[2];
	unsigned int dfares;
	pid_t child, rcom;
	char ** coms[256];
	redio[0] = 0;
	redio[1] = 1;
	NDFA = 1;
	while(NDFA)
	{
		coms[comm] = malloc(sizeof(char *) * 258);
		dfares = DFA(command, coms[comm++], index);
		index = dfares;
	}
	--comm;
	index = argn;
	if ((child = fork()) == -1)
	{
		perror("forkchild");
		exit(1);
	}
	if (child != 0)
	{
		waitpid(child, NULL, 0);
		for(index = 0; index < comm + 1; ++index)
			free(coms[index]);
		return 0;
	}
	if (IRED)
		redio[0] = open(coms[comm][index - (!!(ORED) + !!(IBFO))], O_RDONLY);
	else if (BACK)
		redio[0] = open("/dev/null", O_RDONLY);
	if (ORED)
		redio[1] = open(coms[comm][index - (1 + (!!(IRED) & !(IBFO)))], O_WRONLY | ((ORTP) ? O_APPEND : O_CREAT | O_TRUNC), (S_IRWXU + S_IRWXG + S_IRWXO));
	else if (BACK)
		redio[1] = open("/dev/null", O_WRONLY);
	coms[comm][index - (!!(ORED) + !!(IRED))] = NULL;
	index = 0;
	while (index < comm)
	{
		pipe(fd);
		if ((rcom = fork()) == -1)
		{
			perror("forkrcom");
			exit(1);
		}
		if (rcom == 0)
		{
			dup2(redio[0], 0);
			dup2(fd[1], 1);
			close(fd[0]);
			if (BACK)
				signal(SIGINT, SIG_IGN);
			execvp(*(coms[index]), coms[index]);
		}
		else
		{
			close(fd[1]);
			redio[0] = fd[0];
			++index;
		}
	}
	if ((rcom = fork()) == -1)
	{
		perror("forkrcomlast");
		exit(1);
	}
	if (rcom == 0)
	{
		dup2(redio[0], 0);
		dup2(redio[1], 1);
		if (BACK)
			signal(SIGINT, SIG_IGN);
		execvp(*(coms[index]), coms[index]);
	}
	else
	{
		if (!(BACK))
			waitpid(rcom, NULL, 0);
		exit(0);
	}
	return 0;
}

int main()
{
	char command[65536];
	while(1)
	{
		fgets(command, 65536, stdin);
		if (command[strlen(command) - 1] == '\n') command[strlen(command) - 1] = '\0';
		if (!strcmp("exit", command)) return 0;
		run(command);
	}
}

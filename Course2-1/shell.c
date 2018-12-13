#include<string.h>
#include<unistd.h>
#include<stdlib.h>
#include<signal.h>
#include<fcntl.h>
#include<stdio.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<sys/stat.h>

#define CHARSEND ++argpos; command[index] = '\0'; ++index

/*execvp(const char * file, char * const argv[])*/

typedef unsigned int uint;

typedef enum returnbit
{
	BITNDFA = 1,  /*0000 0001*/
	BITBACK = 2,  /*0000 0010*/
	BITIRED = 4,  /*0000 0100*/
	BITORED = 8,  /*0000 1000*/
	BITORTP = 16, /*0001 0000*/
	BITIBFO = 36  /*0010 0100*/
} rb;

typedef enum statement
{
	ENDOFCOM = 0,
	ARGSPACE = 1,
	ARGCHARS = 2,
	IFNSPACE = 3,
	IFNCHARS = 4,
	OFNSPACE = 5,
	OFNCHARS = 6,
	BACKCHAR = 7
} state;

uint DFA (char * command, char ** args, int index)
{
	state st = ARGSPACE;
	int argpos = 0;
	uint res = 0;
	while (1) switch (st)
	{
	case ENDOFCOM:
		res ^= (index << 16) ^ (argpos << 6);
		args[argpos] = NULL;
		return res;
	case ARGSPACE:
		switch (command[index++])
		{
		case '\0':
			--index; st = ENDOFCOM; break;
		case '&':
			res ^= BITBACK; st = BACKCHAR; break;
		case '|':
			res ^= BITNDFA; st = ENDOFCOM; break;
		case '<':
			res ^= BITIBFO; st = IFNSPACE; break;
		case '>':
			res ^= BITORED; st = OFNSPACE; break;
		case ' ':
			break;
		default:
			args[argpos] = command + (index - 1); st = ARGCHARS;
		} break;
	case ARGCHARS:
		switch (command[index])
		{
		case '\0':
			++argpos; st = ENDOFCOM; break;
		case '&':
			CHARSEND; res ^= BITBACK; st = BACKCHAR; break;
		case '|':
			CHARSEND; res ^= BITNDFA; st = ENDOFCOM; break;
		case '<':
			CHARSEND; res ^= BITIBFO; st = IFNSPACE; break;
		case '>':
			CHARSEND; res ^= BITORED; st = OFNSPACE; break;
		case ' ':
			CHARSEND; st = ARGSPACE; break;
		default:
			++index;
		} break;
	case IFNSPACE:
		switch (command[index++])
		{
		case '\0':
			--index; st = ENDOFCOM; break;
		case '&':
			res ^= BITBACK; st = BACKCHAR; break;
		case '>':
			res ^= BITORED; st = OFNSPACE; break;
		case ' ':
			break;
		default:
			args[argpos] = command + (index - 1); st = IFNCHARS;
		} break;
	case IFNCHARS:
		switch (command[index])
		{
		case '\0':
			++argpos; st = ENDOFCOM; break;
		case '&':
			CHARSEND; res ^= BITBACK; st = BACKCHAR; break;
		case '>':
			CHARSEND; res ^= BITORED; st = OFNSPACE; break;
		case ' ':
			CHARSEND; st = IFNSPACE; break;
		default:
			++index;
		} break;
	case OFNSPACE:
		switch (command[index++])
		{
		case '\0':
			--index; st = ENDOFCOM; break;
		case '&':
			res ^= BITBACK; st = BACKCHAR; break;
		case '<':
			res ^= BITIRED; st = IFNSPACE; break;
		case '>':
			res ^= BITORTP; break;
		case ' ':
			break;
		default:
			args[argpos] = command + (index - 1); st = OFNCHARS;
		} break;
	case OFNCHARS:
		switch (command[index])
		{
		case '\0':
			++argpos; st = ENDOFCOM; break;
		case '&':
			CHARSEND; res ^= BITBACK; st = BACKCHAR; break;
		case '<':
			CHARSEND; res ^= BITIRED; st = IFNSPACE; break;
		case ' ':
			CHARSEND; st = OFNSPACE; break;
		default:
			++index;
		} break;
	case BACKCHAR:
		switch (command[index++])
		{
		case '\0':
			--index; st = ENDOFCOM; break;
		case ' ':
			break;
		} break;
	}
}

int run (char * command)
{
	int index = 0, comm = 0;
	int fd[2];
	int redio[2];
	uint dfares = 1;
	pid_t child, rcom;
	char ** coms[256];
	redio[0] = 0;
	redio[1] = 1;
	while(dfares & BITNDFA)
	{
		coms[comm] = malloc(sizeof(char *) * 258);
		dfares = DFA(command, coms[comm++], index);
		index = dfares >> 16;
	}
	--comm;
	index = (dfares & ((1 << 16) - 1)) >> 6;
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
	if (dfares & BITIRED)
		redio[0] = open(coms[comm][index - (!!(dfares & BITORED) + !!(dfares & BITIBFO))], O_RDONLY);
	else if (dfares & BITBACK)
		redio[0] = open("/dev/null", O_RDONLY);
	if (dfares & BITORED)
		redio[1] = open(coms[comm][index - (1 + (!!(dfares & BITIRED) & !(dfares & BITIBFO)))], O_WRONLY | ((dfares & BITORTP) ? O_APPEND : O_CREAT | O_TRUNC), (S_IRWXU + S_IRWXG + S_IRWXO));
	else if (dfares & BITBACK)
		redio[1] = open("/dev/null", O_WRONLY);
	coms[comm][index - (!!(dfares & BITORED) + !!(dfares & BITIRED))] = NULL;
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
			if (dfares & BITBACK)
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
		if (dfares & BITBACK)
			signal(SIGINT, SIG_IGN);
		execvp(*(coms[index]), coms[index]);
	}
	else
	{
		if (!(dfares & BITBACK))
			waitpid(rcom, NULL, 0);
		exit(0);
	}
	return 0;
}

int main()
{
	char command[1 << 16];
	while(1)
	{
		fputs("com> ", stdout);
		fgets(command, 1 << 16, stdin);
		if (command[strlen(command) - 1] == '\n') command[strlen(command) - 1] = '\0';
		if (!strcmp("exit", command)) return 0;
		run(command);
	}
}

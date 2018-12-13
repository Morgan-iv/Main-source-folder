// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
// PVS-settings
// PVS-settings end
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<fcntl.h>

typedef long long int64;
typedef unsigned int uint;
typedef unsigned long long uint64;

const uint MA = 1e9 + 7, MB = 1e9 + 9; //-V707
const int numfiles = 256;
const int hashsize = 128;
const int filesize = 2400;
const int block = 4096;

struct num
{
	uint a, b;
};

struct hashkey
{
	int deleted;
	int next;
	int stream;
};

uint64 hashes[filesize];
hashkey sarr[numfiles];
char keystr[block + 1];
char valuestr[block + 1];
char command[7];
int count, seed, P, hfc, pos; //-V707

num * OPlusI(num * x, int y) { x->a = (x->a + y) % MA; x->b = (x->b + y) % MB; return x; }
num * OMultI(num * x, int y) { x->a = ((uint64) x->a * y) % MA; x->b = ((uint64) x->b * y) % MB; return x; }

int IsEqual(const num * x, const num * y) { return x->a == y->a && x->b == y->b; }
uint64 ToUInt64(const num * x) { return (uint64) x->a * MB + x->b + 1; }

int max(int a, int b)
{
	return (a > b) ? a : b;
}

uint64 strhash(const char * str)
{
	num hash;
	int i;

	hash.a = hash.b = seed;
	for (i = 0; str[i] != '\0'; ++i)
	{
		OPlusI(OMultI(&hash, P), str[i]);
	}
	return ToUInt64(&hash);
}

int findk(const char * key, uint64 hash)
{
	int i, j, del;
	char comp[block + 1];

	comp[block] = '\0';
	i = hash & (hashsize - 1);
	while (i != -1)
	{
		del = sarr[i].deleted;
		lseek(sarr[i].stream, 0, SEEK_SET);
		read(sarr[i].stream, hashes, sizeof(uint64) * filesize);
		for (j = 0; j < filesize; ++j)
		{
			if (del + j == filesize) break;
			if (hashes[j] == 0) --del;
			if (hash == hashes[j])
			{
				lseek(sarr[j].stream, sizeof(uint64) * filesize + j * block * 2, SEEK_SET);
				read(sarr[i].stream, comp, sizeof(char) * block);
				if (!strcmp(key, comp))
				{
					pos = j;
					return i;
				}
			}
		}
		i = sarr[i].next;
	}
	return -1;
}

int main()
{
	int i, j, index;
	uint64 g = 0;

	srand(time(NULL));
	seed = rand() & ((1 << 16) - 1);
	P = max(239, rand());
	
	hfc = 0;
	for (i = 0; i < block + 1; ++i)
	{
		keystr[i] = valuestr[i] = 0;
	}
	for (i = 0; i < numfiles; ++i)
	{
		sprintf(keystr, "%d", i);
		sarr[i].stream = open(keystr, O_TRUNC|O_CREAT|O_RDWR, 00666);
		for (j = 0; j < filesize; ++j)
		{
			write(sarr[i].stream, &g, sizeof(uint64) * 1);
		}
		/*for (j = 0; j < filesize; ++j)
		{
			fwrite(keystr, sizeof(char), block, sarr[i].stream);
			fwrite(keystr, sizeof(char), block, sarr[i].stream);
		}*/
		sarr[i].deleted = filesize;
		sarr[i].next = -1;
	}

	scanf("%d", &count);
	for (index = 0; index < count; ++index)
	{
		scanf("%s %s", command, keystr); //-V576
		if (command[0] == 'A')
		{
			scanf("%s", valuestr); //-V576
			if (findk(keystr, g = strhash(keystr)) != -1)
			{
				printf("ERROR\n");
				continue;
			}
			j = g & (hashsize - 1);
			while (sarr[j].next != -1 && !sarr[j].deleted)
				j = sarr[j].next;
			if (sarr[j].deleted == 0)
			{
				sarr[j].next = hfc++;
				j = sarr[j].next;
			}
			read(sarr[j].stream, hashes, sizeof(uint64) * filesize);
			for (i = 0; hashes[i] != 0; ++i);
			//hashes[i] = g;
			lseek(sarr[j].stream, i * block * 2, SEEK_CUR);
			write(sarr[j].stream, keystr, sizeof(char) * block);
			write(sarr[j].stream, valuestr, sizeof(char) * block);
			lseek(sarr[j].stream, sizeof(uint64) * i, SEEK_SET);
			write(sarr[j].stream, &g, sizeof(uint64) * 1);
			sarr[j].deleted--;
			continue;
		}
		
		if (command[0] == 'D')
		{
			if ((i = findk(keystr, g = strhash(keystr))) == -1)
			{
				printf("ERROR\n");
				continue;
			}
			lseek(sarr[i].stream, sizeof(uint64) * pos, SEEK_SET);
			g = 0;
			write(sarr[i].stream, &g, sizeof(uint64));
			sarr[i].deleted++;
			continue;
		}
		
		if (command[0] == 'P')
		{
			if ((i = findk(keystr, g = strhash(keystr))) == -1)
			{
				printf("ERROR\n");
				continue;
			}
			read(sarr[i].stream, valuestr, sizeof(char) * block);
			printf("%s %s\n", keystr, valuestr);
			continue;
		}
		
		if (command[0] == 'U')
		{
			scanf("%s", valuestr); //-V576
			if ((i = findk(keystr, g = strhash(keystr))) == -1)
			{
				printf("ERROR\n");
				continue;
			}
			//fread(keystr, sizeof(char), 5, sarr[i].stream);
			write(sarr[i].stream, valuestr, sizeof(char) * block);
			continue;
		}
	}
	return 0;
}

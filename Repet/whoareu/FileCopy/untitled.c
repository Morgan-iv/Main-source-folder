#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <ftw.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#include <sys/stat.h>
#include <sys/types.h>

size_t fromlen;
size_t destlen;
size_t sufflen;
const char * from = "/home/morgan/work/../work/dir";
const char * dest = "/home/morgan/work/here";
const char * suff = ".c";

int nendswith(const char * str, const char * suffix, size_t lenstr, size_t lensuffix)
{
	if (!str)
		return -1;
	if (!suffix)
		return -2;
    if (lensuffix > lenstr)
        return 2;
    return strncmp(str + lenstr - lensuffix, suffix, lensuffix) > 0;
}

int work(const char * path, const struct stat * sb, int typeflag)
{
	size_t plen;
	char * name = malloc((plen = destlen + strlen(path + fromlen)) + 2);
	strcpy(name, dest);
	strcat(name, path + fromlen);
	if (typeflag == FTW_D)
		mkdir(name, sb->st_mode);
	if (typeflag == FTW_F && !nendswith(name, suff, plen, sufflen))
	{
		char buf;
		long long size = sb->st_size;
		int f = open(path, O_RDONLY);
		int d = creat(name, sb->st_mode);
		for (long long i = 0; i < size; ++i)
		{
			read(f, &buf, 1);
			write(d, &buf, 1);
		}
		close(f);
		close(d);
	}
	free(name);
	return 0;
}

int main()
{
	fromlen = strlen(from);
	destlen = strlen(dest);
	sufflen = strlen(suff);
	ftw(from, work, 100);
	return 0;
}
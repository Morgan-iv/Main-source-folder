#include <sys/types.h>
#include <sys/socket.h>

#include <cstring>

struct record_t
{
    static const size_t maxaulen = 32;
    static const size_t maxjolen = 64;
    static const size_t maxarlen = 128;

    uint64_t ID;
    char author[maxaulen];
    int32_t year;
    int32_t number;
    char journal[maxjolen];
    char article[maxarlen];

    record_t()
    {
        ID = 0;
        author[0] = '\0';
        year = 0;
        number = 0;
        journal[0] = '\0';
        article[0] = '\0';
    }

    record_t(uint64_t id, const char * au, int32_t y, int32_t n, const char * jo, const char * ar) :
        ID(id), year(y), number(n)
    {
        size_t size;
        size = strlen(au) + 1;
        if (size > maxaulen)
            size = maxaulen;
        std::copy(au, au + size, author);
        size = strlen(jo) + 1;
        if (size > maxjolen)
            size = maxjolen;
        std::copy(jo, jo + size, journal);
        size = strlen(ar) + 1;
        if (size > maxarlen)
            size = maxarlen;
        std::copy(ar, ar + size, article);
    }
};

int swapbytes(int a)
{
    char * bytes = (char *)&a;
    char tmp = bytes[0];
    bytes[0] = bytes[3];
    bytes[3] = tmp;
    tmp = bytes[1];
    bytes[1] = bytes[2];
    bytes[2] = tmp;
    return a;
}

uint64_t swapbytes(uint64_t a)
{
    char * bytes = (char *)&a;
    char tmp = bytes[0];
    bytes[0] = bytes[7];
    bytes[7] = tmp;
    tmp = bytes[1];
    bytes[1] = bytes[6];
    bytes[6] = tmp;
    tmp = bytes[2];
    bytes[2] = bytes[5];
    bytes[5] = tmp;
    tmp = bytes[3];
    bytes[3] = bytes[4];
    bytes[4] = tmp;
    return a;
}

int sendall(int s, const char *buf, int len, int flags)
{
    int total = 0;
    int n;

    while (total < len)
    {
        n = send(s, buf + total, len - total, flags);
        if (n == -1)
            break;
        total += n;
    }

    return (n == -1) ? -1 : total;
}

int recvall(int s, char *buf, int len, int flags)
{
    int total = 0;
    int n;

    while (total < len)
    {
        n = recv(s, buf + total, len - total, flags);
        if (n <= 0)
            break;
        total += n;
    }

    return (n <= 0) ? n : total;
}

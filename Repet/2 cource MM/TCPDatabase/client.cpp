#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>

#include <unistd.h>

#include <vector>
#include <string>
#include <cstring>
#include <iostream>

#include "mydb.h"

int tcp_port = 55555;

using response_t = std::vector<record_t>;

//using namespace std;

static char * cp1251_to_utf8(char *out, const char *in)
{
    static const int table[128] = {
        0x82D0,0x83D0,0x9A80E2,0x93D1,0x9E80E2,0xA680E2,0xA080E2,0xA180E2,
        0xAC82E2,0xB080E2,0x89D0,0xB980E2,0x8AD0,0x8CD0,0x8BD0,0x8FD0,
        0x92D1,0x9880E2,0x9980E2,0x9C80E2,0x9D80E2,0xA280E2,0x9380E2,0x9480E2,
        0,0xA284E2,0x99D1,0xBA80E2,0x9AD1,0x9CD1,0x9BD1,0x9FD1,
        0xA0C2,0x8ED0,0x9ED1,0x88D0,0xA4C2,0x90D2,0xA6C2,0xA7C2,
        0x81D0,0xA9C2,0x84D0,0xABC2,0xACC2,0xADC2,0xAEC2,0x87D0,
        0xB0C2,0xB1C2,0x86D0,0x96D1,0x91D2,0xB5C2,0xB6C2,0xB7C2,
        0x91D1,0x9684E2,0x94D1,0xBBC2,0x98D1,0x85D0,0x95D1,0x97D1,
        0x90D0,0x91D0,0x92D0,0x93D0,0x94D0,0x95D0,0x96D0,0x97D0,
        0x98D0,0x99D0,0x9AD0,0x9BD0,0x9CD0,0x9DD0,0x9ED0,0x9FD0,
        0xA0D0,0xA1D0,0xA2D0,0xA3D0,0xA4D0,0xA5D0,0xA6D0,0xA7D0,
        0xA8D0,0xA9D0,0xAAD0,0xABD0,0xACD0,0xADD0,0xAED0,0xAFD0,
        0xB0D0,0xB1D0,0xB2D0,0xB3D0,0xB4D0,0xB5D0,0xB6D0,0xB7D0,
        0xB8D0,0xB9D0,0xBAD0,0xBBD0,0xBCD0,0xBDD0,0xBED0,0xBFD0,
        0x80D1,0x81D1,0x82D1,0x83D1,0x84D1,0x85D1,0x86D1,0x87D1,
        0x88D1,0x89D1,0x8AD1,0x8BD1,0x8CD1,0x8DD1,0x8ED1,0x8FD1
    };
    char * res = out;
    while (*in)
        if (*in & 0x80) {
            int v = table[(int)(0x7f & *in++)];
            if (!v)
                continue;
            *out++ = (char)v;
            *out++ = (char)(v >> 8);
            if (v >>= 16)
                *out++ = (char)v;
        }
        else
            *out++ = *in++;
    *out = 0;
    return res;
}

void print(const record_t & rec, int loc1251)
{
    if (loc1251)
    {
        char buf[512];
        std::cout << rec.ID << std::endl;
        std::cout << cp1251_to_utf8(buf, rec.author) << std::endl;
        std::cout << rec.year << std::endl;
        std::cout << rec.number << std::endl;
        std::cout << cp1251_to_utf8(buf, rec.journal) << std::endl;
        std::cout << cp1251_to_utf8(buf, rec.article) << std::endl;
        std::cout << std::endl;
    }
    else
        std::cout << rec.ID << std::endl
                  << rec.author << std::endl
                  << rec.year << std::endl
                  << rec.number << std::endl
                  << rec.journal << std::endl
                  << rec.article << std::endl
                  << std::endl;
}



int main()
{
    struct sockaddr_in addr;
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0)
    {
        perror("socket");
        exit(1);
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(tcp_port);
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    if(connect(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("connect");
        exit(2);
    }

    int chk;
    recvall(sock, (char *)&chk, sizeof(int), MSG_NOSIGNAL);
    if (chk == 1)
        chk = 0;
    sendall(sock, (char *)&chk, sizeof(int), MSG_NOSIGNAL);

    int comchar = 1;
    int loc1251 = 0;

    while (true)
    {
        if (!std::cin.good())
            break;
        
        std::string q;
        if (comchar)
            std::cout << '>';
        std::getline(std::cin, q);

        if (!q.compare("exit"))
            break;

        if (!q.compare("nochar"))
        {
            comchar = 0;
            continue;
        }

        if (!q.compare("onchar"))
        {
            comchar = 1;
            continue;
        }

        if (!q.compare("no1251"))
        {
            loc1251 = 0;
            continue;
        }

        if (!q.compare("cp1251"))
        {
            loc1251 = 1;
            continue;
        }

        int len = q.length();
        sendall(sock, (char *)&len, sizeof(int), MSG_NOSIGNAL);
        sendall(sock, q.c_str(), len, MSG_NOSIGNAL);
    
        recvall(sock, (char *)&len, sizeof(int), MSG_NOSIGNAL);
        if (chk)
            len = swapbytes(len);
        if (len < 0)
        {
            std::cout << "exception" << len << std::endl << std::endl;
            continue;
        }

        response_t result(len);
        if (len)
            recvall(sock, (char *)result.data(), len * sizeof(record_t), MSG_NOSIGNAL);
        if (chk)
            for (int i = 0; i < len; ++i)
            {
                result[i].ID = swapbytes(result[i].ID);
                result[i].year = swapbytes(result[i].year);
                result[i].number = swapbytes(result[i].number);
            }
        for (size_t i = 0; i < result.size(); ++i)
        {
            print(result[i], loc1251);
        }
    }
    chk = 0;
    sendall(sock, (char *)&chk, sizeof(int), MSG_NOSIGNAL);
    shutdown(sock, 2);
    close(sock);
    return 0;
}

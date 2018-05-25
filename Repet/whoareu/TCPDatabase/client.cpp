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

void print(const record_t & rec)
{
    std::cout << rec.ID << std::endl
              << rec.journal << std::endl
              << rec.year << std::endl
              << rec.number << std::endl
              << rec.author << std::endl
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
            print(result[i]);
        }
    }
    chk = 0;
    sendall(sock, (char *)&chk, sizeof(int), MSG_NOSIGNAL);
    shutdown(sock, 2);
    close(sock);
    return 0;
}
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

using namespace std;

response_t DBQuery(std::string querystr)
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
    
    int len = querystr.length();
    sendall(sock, (char *)&len, sizeof(int), MSG_NOSIGNAL);
    sendall(sock, querystr.c_str(), len, MSG_NOSIGNAL);

    recvall(sock, (char *)&len, sizeof(int), MSG_NOSIGNAL);
    if (chk)
    	len = swapbytes(len);
    if (len < 0)
    {
        cout << "exception" << len << endl << endl;
        shutdown(sock, 2);
        close(sock);
        return response_t(0);
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
    shutdown(sock, 2);
    close(sock);
    return result;
}

void print(const record_t & rec)
{
    cout << rec.ID << endl
         << rec.journal << endl
         << rec.year << endl
         << rec.number << endl
         << rec.author << endl
         << rec.article << endl
         << endl;
}

int main()
{
	//response_t res = DBQuery(std::string("select author=[Au1] number =[42,42]"));
	while (true)
    {
        string q;
        cout << '>';
        std::getline(cin, q);
        if (!q.compare("exit"))
            break;
        response_t res = DBQuery(q);
        for (int i = 0; i < res.size(); ++i)
        {
            print(res[i]);
        }
    }
}
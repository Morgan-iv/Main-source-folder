#include <pthread.h> 
#include <sys/types.h> 
#include <stdio.h> 
#include <map>
#include <string>
#include <boost/regex.hpp>
#include <stdlib.h>
#include <iostream>

#include "fcgi_config.h" 
#include "fcgiapp.h" 


#define THREAD_COUNT 1 
#define SOCKET_PATH "127.0.0.1:9000" 

typedef std::map<std::string, std::string> QueryStr;


namespace ConstTexts
{
    static const char NoQueryPage[] = 
    "Content-type: text/html\r\n\r\n<html>\r\n<title>API Page</title>\r\n</head>\r\n<body>\r\n<h1>Please, use <a href=\"http://saferoad.tk\">our site</a> to get API documentation</h1>\r\n</body>\r\n</html>\r\n";
    static const char BadQuery[] = 
    "Status: 400\r\nContent-type: application/json\r\n\r\n{\r\n\t\"status\" : 400,\r\n\t\"description\" : \"bad query syntax\"\r\n}\r\n";
    static const char JsonHead[] =
    "Status: 200\r\nContent-type: application/json\r\n";
    static const char HTTPDelim[] = 
    "\r\n";
    
}

//хранит дескриптор открытого сокета 
static int socketId; 

static const char BrowserPage[] = 
"Content-type: text/html\r\n\r\n<html>\r\n<title>API Page</title>\r\n</head>\r\n<body>\r\n<h1>Please, use <a href=\"http://saferoad.tk\">our site</a> to get API documentation</h1>\r\n</body>\r\n</html>\r\n";

//parsing HTML with regular ex OH ZALGO HE COMESSSSSSSSSSSSS
static int QueryParser(char * qstr, QueryStr & result)
{
    if (strlen(qstr) == 0) return -1;
    boost::regex pattern("(([^&=]+)\\=([^&]+)).*");
    boost::cmatch mres;
    int pos = 0;
    while (boost::regex_match(&(qstr[pos]), mres, pattern))
    {
        pos += mres[1].length() + 1;
        result[std::string(mres[2])] = std::string(mres[3]);
    }
    if (result.size() == 0) return -2;
    return 0;
}

static void *doit(void *a) 
{ 
    int rc, i; 
    FCGX_Request request; 
    char *query_string; 

    if(FCGX_InitRequest(&request, socketId, 0) != 0) 
    { 
        //ошибка при инициализации структуры запроса 
        printf("Can not init request\n"); 
        return NULL; 
    } 
    printf("Request is inited\n"); 
 
    for(;;) 
    { 
        static pthread_mutex_t accept_mutex = PTHREAD_MUTEX_INITIALIZER; 

        //попробовать получить новый запрос 
        printf("Try to accept new request\n"); 
        pthread_mutex_lock(&accept_mutex); 
        rc = FCGX_Accept_r(&request); 
        pthread_mutex_unlock(&accept_mutex); 

        if(rc < 0) 
        { 
            //ошибка при получении запроса 
            printf("Can not accept new request\n"); 
            break; 
        } 
        printf("request is accepted\n"); 
        
        //FCGX_PutS(BrowserPage, request.out);
        //получить значение переменной 
        query_string = FCGX_GetParam("QUERY_STRING", request.envp); 
        printf("%s\n", query_string);
        QueryStr query;
        i = QueryParser(query_string, query);
        if (i == -1)
        {
            FCGX_PutS(ConstTexts::NoQueryPage, request.out);
            FCGX_Finish_r(&request);
            continue;
        }
        if (i == -2)
        {
            FCGX_PutS(ConstTexts::BadQuery, request.out);
            FCGX_Finish_r(&request);
            continue;
        }
        FCGX_PutS(ConstTexts::JsonHead, request.out);
        FCGX_PutS(ConstTexts::HTTPDelim, request.out);
        FCGX_PutS("{", request.out);
        for (auto it = query.begin(); it != query.end(); ++it)
        {
            if (it != query.begin()) FCGX_PutS(",", request.out);
            FCGX_PutS("\"", request.out);
            FCGX_PutStr(it->first.c_str(), it->first.length(), request.out);
            FCGX_PutS("\":\"", request.out);
            FCGX_PutStr(it->second.c_str(), it->second.length(), request.out);
            FCGX_PutS("\"", request.out);
            //printf("%s : %s\n", it->first.c_str(), it->second.c_str());
        }
        FCGX_PutS("}", request.out);
        
        
        //закрыть текущее соединение 
        FCGX_Finish_r(&request); 
        
        //завершающие действия - запись статистики, логгирование ошибок и т.п. 
    } 
 
    return NULL; 
} 

int main(void) 
{ 
    int i; 
    pthread_t id[THREAD_COUNT]; 

    //инициализация библилиотеки 
    FCGX_Init(); 
    printf("Lib is inited\n"); 
    
    //открываем новый сокет 
    socketId = FCGX_OpenSocket(SOCKET_PATH, 20); 
    if(socketId < 0) 
    { 
        //ошибка при открытии сокета 
        return 1; 
    } 
    printf("Socket is opened\n"); 

    //создаём рабочие потоки 
    for(i = 0; i < THREAD_COUNT; i++) 
    { 
        pthread_create(&id[i], NULL, doit, NULL); 
    } 
    
    //ждем завершения рабочих потоков 
    for(i = 0; i < THREAD_COUNT; i++) 
    { 
        pthread_join(id[i], NULL); 
    } 

    return 0; 
}

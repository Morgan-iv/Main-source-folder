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

#include <cppdb/frontend.h>

#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>

#ifndef SAFEROAD_TEST
#define SAFEROAD_FINAL
#endif

#define THREAD_COUNT 1 

//#define SOCKET_PATH "127.0.0.1:9001" 

typedef std::map<std::string, std::string> QueryStr;


namespace ConstTexts
{
    #ifdef SAFEROAD_TEST
    static const char SocketPath[] =
    "127.0.0.1:9001";
    #endif
    #ifdef SAFEROAD_FINAL
    static const char SocketPath[] =
    "127.0.0.1:9000";
    #endif
    static const char NoQueryPage[] = 
    "Content-type: text/html\r\n\r\n<html>\r\n<title>API Page</title>\r\n</head>\r\n<body>\r\n<h1>Please, use <a href=\"http://saferoad.tk\">our site</a> to get API documentation</h1>\r\n</body>\r\n</html>\r\n";
    static const char BadQueryHead[] =
    "Status: 400\r\nContent-type: application/json\r\n\r\n{\r\n\t\"status\" : 400,\r\n\t\"description\" : \"";
    static const char BadQueryEnd[] =
    "\"\r\n}\r\n";
    static const char BadQuerySyntax[] =
    "bad query syntax";
    static const char BadQueryTSLost[] =
    "can't find timestamp key in request";
    static const char BadQueryNaN[] = 
    "timestamp value is not a number";
    static const char BadQueryOutLLRange[] = 
    "timestamp value is out of long long range";
    static const char BadQueryWTFEx[] = 
    "You just broken the API. Please, contact us";
    static const char JsonHead[] =
    "Status: 200\r\nContent-type: application/json\r\n";
    static const char HTTPDelim[] = 
    "\r\n";
    static const char SQLConnect[] = 
    "odbc:Driver={ODBC Driver 13 for SQL Server};Server=tcp:saferoad-database.database.windows.net,1433;Database=Areas;Uid=morgan@saferoad-database;Pwd={SafeRoad-ARbfX4ZB6D};Encrypt=yes;TrustServerCertificate=no;Connection Timeout=30;";
    static const char SQLQuery[] = 
    "SELECT TOP 3 id, CAST(tstamp AS BIGINT) AS ts, coordx AS latitude, coordy AS longitude, radius, dp, dh FROM Areas WHERE tstamp > CAST(? AS ROWVERSION) ORDER BY ts";
    static const char SQLMaxTimestamp[] = 
    "SELECT CAST(MAX(tstamp) AS BIGINT) AS max FROM Areas";
    static const char SQLMaxID[] = 
    "SELECT MAX(id) AS max FROM Areas";
    static const char * JSONKeys[] =
    {
        "tstampmax",
        "count",
        "data",
        "id",
        "lat",
        "lng",
        "rad",
        "tstampto",
        "idmax",
		"dp",
		"dh"
    };
    static const std::string StrTimestamp("timestamp");
    static const std::string ExMapAt("map::at");
    static const std::string ExStoll("stoll");
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
    
    int id, count, idmax;
    long long tstamp, max, cur;
    double lat, lng, rad, dp, dh;
    
    cppdb::result res;
    cppdb::statement stat;
    
    rapidjson::Value jsonvalue[2];
    
    cppdb::session sql(ConstTexts::SQLConnect);
    std::cout << "Connected to areas table" << std::endl;
    stat = sql << ConstTexts::SQLQuery;
    
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
            FCGX_PutS(ConstTexts::BadQueryHead, request.out);
            FCGX_PutS(ConstTexts::BadQuerySyntax, request.out);
            FCGX_PutS(ConstTexts::BadQueryEnd, request.out);
            FCGX_Finish_r(&request);
            continue;
        }
        
        //TODO: send cause to user and server log
        try
        {
            cur = std::stoll(query.at(ConstTexts::StrTimestamp));
        }
        catch (std::out_of_range e)
        {
            const std::string & exstr = e.what();
            if (exstr == ConstTexts::ExMapAt)
            {
                FCGX_PutS(ConstTexts::BadQueryHead, request.out);
                FCGX_PutS(ConstTexts::BadQueryTSLost, request.out);
                FCGX_PutS(ConstTexts::BadQueryEnd, request.out);
                FCGX_Finish_r(&request);
                continue;
            }
            if (exstr == ConstTexts::ExStoll)
            {
                FCGX_PutS(ConstTexts::BadQueryHead, request.out);
                FCGX_PutS(ConstTexts::BadQueryOutLLRange, request.out);
                FCGX_PutS(ConstTexts::BadQueryEnd, request.out);
                FCGX_Finish_r(&request);
                continue;
            }
            
            FCGX_PutS(ConstTexts::BadQueryHead, request.out);
            FCGX_PutS(ConstTexts::BadQueryWTFEx, request.out);
            FCGX_PutS(ConstTexts::BadQueryEnd, request.out);
            FCGX_Finish_r(&request);
            continue;
        }
        catch (std::invalid_argument e)
        {
            const std::string & exstr = e.what();
            if (exstr == ConstTexts::ExStoll)
            {
                FCGX_PutS(ConstTexts::BadQueryHead, request.out);
                FCGX_PutS(ConstTexts::BadQueryNaN, request.out);
                FCGX_PutS(ConstTexts::BadQueryEnd, request.out);
                FCGX_Finish_r(&request);
                continue;
            }
            
            FCGX_PutS(ConstTexts::BadQueryHead, request.out);
            FCGX_PutS(ConstTexts::BadQueryWTFEx, request.out);
            FCGX_PutS(ConstTexts::BadQueryEnd, request.out);
            FCGX_Finish_r(&request);
            continue;
        }
        catch (...)
        {
            FCGX_PutS(ConstTexts::BadQueryHead, request.out);
            FCGX_PutS(ConstTexts::BadQueryWTFEx, request.out);
            FCGX_PutS(ConstTexts::BadQueryEnd, request.out);
            FCGX_Finish_r(&request);
            continue;
        }
        
        res = sql << ConstTexts::SQLMaxTimestamp;
        res.next();
        res >> max;
        res = sql << ConstTexts::SQLMaxID;
        res.next();
        res >> idmax;
        tstamp = max;
        stat.reset();
        stat << cur;
        res = stat.query();
        count = stat.affected();
        
        rapidjson::Document jsondoc;
        auto & jsondocalloc = jsondoc.GetAllocator();
        
        jsondoc.SetObject();
        jsonvalue[0].SetUint64(max);
        jsondoc.AddMember(rapidjson::StringRef(ConstTexts::JSONKeys[0]), jsonvalue[0], jsondocalloc);
        jsondoc.AddMember(rapidjson::StringRef(ConstTexts::JSONKeys[8]), idmax, jsondocalloc);
        jsondoc.AddMember(rapidjson::StringRef(ConstTexts::JSONKeys[1]), count, jsondocalloc);
        jsonvalue[0].SetArray();
        while (res.next())
        {
            res >> id >> tstamp >> lat >> lng >> rad >> dp >> dh;
            jsonvalue[1].SetObject();
            jsonvalue[1].AddMember(rapidjson::StringRef(ConstTexts::JSONKeys[3]),  id, jsondocalloc);
            jsonvalue[1].AddMember(rapidjson::StringRef(ConstTexts::JSONKeys[4]), lat, jsondocalloc);
            jsonvalue[1].AddMember(rapidjson::StringRef(ConstTexts::JSONKeys[5]), lng, jsondocalloc);
            jsonvalue[1].AddMember(rapidjson::StringRef(ConstTexts::JSONKeys[6]), rad, jsondocalloc);
			jsonvalue[1].AddMember(rapidjson::StringRef(ConstTexts::JSONKeys[9]),  dp, jsondocalloc);
            jsonvalue[1].AddMember(rapidjson::StringRef(ConstTexts::JSONKeys[10]), dh, jsondocalloc);
            jsonvalue[0].PushBack(jsonvalue[1], jsondocalloc);
        }
        jsondoc.AddMember(rapidjson::StringRef(ConstTexts::JSONKeys[2]), jsonvalue[0], jsondocalloc);
        jsonvalue[0].SetUint64(tstamp);
        jsondoc.AddMember(rapidjson::StringRef(ConstTexts::JSONKeys[7]), jsonvalue[0], jsondocalloc);
        
        rapidjson::StringBuffer buffer;
        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
        jsondoc.Accept(writer);
        
        FCGX_PutS(ConstTexts::JsonHead, request.out);
        FCGX_PutS(ConstTexts::HTTPDelim, request.out);
        FCGX_PutS(buffer.GetString(), request.out);
        
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
    socketId = FCGX_OpenSocket(ConstTexts::SocketPath, 20); 
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

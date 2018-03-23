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

typedef std::map<std::string, std::string> QueryMap;
typedef char byte;

struct tsmapelem
{
	long long ts;
	int id;
};

static const int probacolscnt = 20;
static const int sizecrec = sizeof(long long) + sizeof(int) + sizeof(short);
static const int sizeprec = sizeof(long long) + sizeof(float) * probacolscnt;
static const int cmaxdatacount = 10;
static const int pmaxdatacount = 10;


namespace ConstTexts
{
    static const char FileNameFCGIConf[] =
	"fcgi_config.cfg";
	static const char FileNameRawData[] =
	"rawcoords.dat";
	static const char FileNameCoordTable[] =
	"coordtable.dat";
	static const char FileNameProbaTable[] =
	"probatable.dat";
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
	static const char BadQueryApiKey[] =
    "bad API key";
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
        "idmax",         //0
        "coords",        //1
        "rawfilelen",    //2
        "tstampto",      //3
        "tstampmax",     //4
        "count",         //5
        "crecords",      //6
        "id",            //7
        "sht",           //8
		"len",           //9
		"points",        //10
		"probabilities", //11
        "precords",      //12
		"probas",        //13
    };
    static const std::string StrCTStamp("ctstamp");
	static const std::string StrPTStamp("ptstamp");
    static const std::string ExMapAt("map::at");
    static const std::string ExStoll("stoll");
	static const std::string BadQueryTS("bad ctstamp or ptstamp value");
}

//хранит дескриптор открытого сокета 
static int socketId;

int idmax;
int rawdatalen;
static tsmapelem * ctstamp;
static tsmapelem * ptstamp;
static byte * coordsfile;
static byte * coordtable;
static byte * probatable;

static int tmecmp(const void * a, const void * b)
{
	return *((long long *)a) - *((long long *)b);
}

static int binsearch(const tsmapelem * arr, int v, int l, int r)
{
	//if (l == r) return (arr[l].ts == v) l + 1 : l;
	if (l == r) return l;
	int m = (l + r) / 2;
	if (arr[m].ts >= v) r = m;
	else l = m + 1;
	return binsearch(arr, v, l, r);
}

//parsing HTML with regular ex OH ZALGO HE COMESSSSSSSSSSSSS
static int QueryParser(char * qstr, QueryMap & result)
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

int checkAPIKey(const QueryMap & query)
{
	return 1;
}

int getCTStamp(const QueryMap & query)
{
	int CTS;
	try
    {
        CTS = std::stoll(query.at(ConstTexts::StrCTStamp));
    }
    catch (std::out_of_range e)
    {
        const std::string & exstr = e.what();
        if (exstr == ConstTexts::ExMapAt)
			return -1;
        if (exstr == ConstTexts::ExStoll)
			return -2;
        return -255;
    }
    catch (std::invalid_argument e)
    {
        const std::string & exstr = e.what();
        if (exstr == ConstTexts::ExStoll)
			return -3;
        return -255;
    }
    catch (...)
    {
        return -255;
    }
	return CTS;
}

int getPTStamp(const QueryMap & query)
{
	int PTS;
	try
    {
        PTS = std::stoll(query.at(ConstTexts::StrPTStamp));
    }
    catch (std::out_of_range e)
    {
        const std::string & exstr = e.what();
        if (exstr == ConstTexts::ExMapAt)
			return -1;
        if (exstr == ConstTexts::ExStoll)
			return -2;
        return -255;
    }
    catch (std::invalid_argument e)
    {
        const std::string & exstr = e.what();
        if (exstr == ConstTexts::ExStoll)
			return -3;
        return -255;
    }
    catch (...)
    {
        return -255;
    }
	return PTS;
}

std::string getJSONStr(int CTS, int PTS)
{
	rapidjson::Value jsonvalue[4];
	rapidjson::Document jsondoc;
    auto & jsondocalloc = jsondoc.GetAllocator();
	
	int cpos = binsearch(ctstamp, CTS, 0, idmax);
	int ppos = binsearch(ptstamp, PTS, 0, idmax);
	if (cpos < idmax && ctstamp[cpos].ts <= CTS) cpos++;
	if (ppos < idmax && ptstamp[ppos].ts <= PTS) ppos++;
	int ccnt = idmax - cpos;
	int pcnt = idmax - ppos;
	if (ccnt > cmaxdatacount) ccnt = cmaxdatacount;
	if (pcnt > pmaxdatacount) pcnt = pmaxdatacount;
	
	jsondoc.SetObject();
	jsondoc.AddMember(rapidjson::StringRef(ConstTexts::JSONKeys[0]), idmax, jsondocalloc);
	
	jsonvalue[0].SetObject();
	jsonvalue[0].AddMember(rapidjson::StringRef(ConstTexts::JSONKeys[2]), rawdatalen, jsondocalloc);
	jsonvalue[1].SetUint64(ctstamp[cpos + ccnt - 1].ts);
	jsonvalue[0].AddMember(rapidjson::StringRef(ConstTexts::JSONKeys[3]), jsonvalue[1], jsondocalloc);
	jsonvalue[1].SetUint64(ctstamp[idmax - 1].ts);
	jsonvalue[0].AddMember(rapidjson::StringRef(ConstTexts::JSONKeys[4]), jsonvalue[1], jsondocalloc);
	jsonvalue[0].AddMember(rapidjson::StringRef(ConstTexts::JSONKeys[5]), ccnt, jsondocalloc);
	
	jsonvalue[1].SetArray();
	for (int i = 0; i < ccnt; ++i)
	{
		jsonvalue[2].SetObject();
		int sht = *((int   *)(coordtable + (ctstamp[cpos + i].id * sizecrec) + sizeof(long long)));
		int len = *((short *)(coordtable + (ctstamp[cpos + i].id * sizecrec) + sizeof(long long) + sizeof(int)));
		jsonvalue[2].AddMember(rapidjson::StringRef(ConstTexts::JSONKeys[7]), ctstamp[cpos + i].id, jsondocalloc);
		jsonvalue[2].AddMember(rapidjson::StringRef(ConstTexts::JSONKeys[8]), sht, jsondocalloc);
		jsonvalue[2].AddMember(rapidjson::StringRef(ConstTexts::JSONKeys[9]), len, jsondocalloc);
		
		jsonvalue[3].SetArray();
		for (int j = sht; j < sht + len; ++j)
			jsonvalue[3].PushBack(*((float *)(coordsfile + (j * sizeof(float)))), jsondocalloc);
		jsonvalue[2].AddMember(rapidjson::StringRef(ConstTexts::JSONKeys[10]), jsonvalue[3], jsondocalloc);
		
		jsonvalue[1].PushBack(jsonvalue[2], jsondocalloc);
	}
	jsonvalue[0].AddMember(rapidjson::StringRef(ConstTexts::JSONKeys[6]), jsonvalue[1], jsondocalloc);
	
	jsondoc.AddMember(rapidjson::StringRef(ConstTexts::JSONKeys[1]), jsonvalue[0], jsondocalloc);
	
	jsonvalue[0].SetObject();
	jsonvalue[1].SetUint64(ptstamp[ppos + pcnt - 1].ts);
	jsonvalue[0].AddMember(rapidjson::StringRef(ConstTexts::JSONKeys[3]), jsonvalue[1], jsondocalloc);
	jsonvalue[1].SetUint64(ptstamp[idmax - 1].ts);
	jsonvalue[0].AddMember(rapidjson::StringRef(ConstTexts::JSONKeys[4]), jsonvalue[1], jsondocalloc);
	jsonvalue[0].AddMember(rapidjson::StringRef(ConstTexts::JSONKeys[5]), pcnt, jsondocalloc);
	
	jsonvalue[1].SetArray();
	for (int i = 0; i < pcnt; ++i)
	{
		jsonvalue[2].SetObject();
		jsonvalue[2].AddMember(rapidjson::StringRef(ConstTexts::JSONKeys[7]), ptstamp[ppos + i].id, jsondocalloc);
		
		jsonvalue[3].SetArray();
		for (int j = 0; j < probacolscnt; ++j)
			jsonvalue[3].PushBack(((float *)(probatable + (ptstamp[ppos + i].id * sizeprec) + sizeof(long long)))[j], jsondocalloc);
		jsonvalue[2].AddMember(rapidjson::StringRef(ConstTexts::JSONKeys[13]), jsonvalue[3], jsondocalloc);
		
		jsonvalue[1].PushBack(jsonvalue[2], jsondocalloc);
	}
	jsonvalue[0].AddMember(rapidjson::StringRef(ConstTexts::JSONKeys[12]), jsonvalue[1], jsondocalloc);
	
	jsondoc.AddMember(rapidjson::StringRef(ConstTexts::JSONKeys[11]), jsonvalue[0], jsondocalloc);
	
	rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    jsondoc.Accept(writer);
	
	return std::string(buffer.GetString());
}

std::string getResponseByKeyMap(const QueryMap & query)
{
	std::string result = "";
	
	int typeofAPIKey = checkAPIKey(query);
	if (typeofAPIKey < 0)
	{
		result += ConstTexts::BadQueryHead;
		result += ConstTexts::BadQueryApiKey;
		result += ConstTexts::BadQueryEnd;
		return result;
	}
	
	int cts, pts;
	cts = getCTStamp(query);
	pts = getPTStamp(query);
	if (cts < 0 || pts < 0)
	{
		result += ConstTexts::BadQueryHead;
		result += ConstTexts::BadQueryTS;
		result += ConstTexts::BadQueryEnd;
		return result;
	}
	
	result += ConstTexts::JsonHead;
    result += ConstTexts::HTTPDelim;
	
	result += getJSONStr(cts, pts);
	
	return result;
}

static void *doit(void *a) 
{ 
    int rc, i; 
    FCGX_Request request; 
    char *query_string;
    
    int id, count, idmax;
    long long tstamp, max, cur;
    //double lat, lng, rad, dp, dh;
    /*
    cppdb::result res;
    cppdb::statement stat;
	*/
	
    
    //rapidjson::Value jsonvalue[2];
    /*
    cppdb::session sql(ConstTexts::SQLConnect);
    std::cout << "Connected to areas table" << std::endl;
    stat = sql << ConstTexts::SQLQuery;
    */
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
        
        QueryMap query;
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
		
		std::string response = getResponseByKeyMap(query);
        
		FCGX_PutStr(response.c_str(), response.length(), request.out);
		FCGX_Finish_r(&request);
		
		/*
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
        */
        //завершающие действия - запись статистики, логгирование ошибок и т.п. 
    } 
 
    return NULL; 
} 

int main(void) 
{ 
    int i; 
    pthread_t id[THREAD_COUNT]; 
	
	FILE * conf = fopen(ConstTexts::FileNameFCGIConf, "r");
	fscanf(conf, "%d %d", &idmax, &rawdatalen);
	fclose(conf);
	
	coordsfile = (byte *) malloc(sizeof(float) * rawdatalen);
	coordtable = (byte *) malloc(sizecrec * idmax);
	probatable = (byte *) malloc(sizeprec * idmax);
	ctstamp = (tsmapelem *) malloc(sizeof(tsmapelem) * idmax);
	ptstamp = (tsmapelem *) malloc(sizeof(tsmapelem) * idmax);
	
	FILE * raw = fopen(ConstTexts::FileNameRawData, "rb");
	fread((void *) coordsfile, sizeof(float), rawdatalen, raw);
	fclose(raw);
	
	FILE * coord = fopen(ConstTexts::FileNameCoordTable, "rb");
	fread((void *) coordtable, sizecrec, idmax, coord);
	fclose(coord);
	
	FILE * proba = fopen(ConstTexts::FileNameProbaTable, "rb");
	fread((void *) probatable, sizeprec, idmax, proba);
	fclose(proba);
	
	for (int i = 0; i < idmax; ++i)
	{
		ctstamp[i].ts = *((long long *)(coordtable + (i * sizecrec)));
		ctstamp[i].id = i;
	}
	
	for (int i = 0; i < idmax; ++i)
	{
		ptstamp[i].ts = *((long long *)(probatable + (i * sizeprec)));
		ptstamp[i].id = i;
	}
	
	qsort(ctstamp, idmax, sizeof(tsmapelem), tmecmp);
	qsort(ptstamp, idmax, sizeof(tsmapelem), tmecmp);
	
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


#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
//#include <sys/stat.h>
//#include <sys/ipc.h>
//#include <sys/un.h>

//#include <signal.h>
#include <unistd.h>
//#include <fcntl.h>

#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <map>
#include <set>
#include <unordered_map>
#include <thread>
#include <vector>
#include <atomic>
#include <mutex>
#include <boost/regex.hpp>
#include <algorithm>
#include <iostream>

#include "mydb.h"


// TODO: recv/send check flags/signals
//


class sharedMtx
{

	private:
                    //cветофор 
	std::mutex _read;
	std::mutex _write;
	std::atomic<int> _cnt;

	public:
                     //Method of svetofor
	void lock()
	{
		_write.lock();
		_read.lock();
	}

	void unlock()
	{
		_read.unlock();
		_write.unlock();
	}

	void lock_shared()
	{
		_write.lock();
		++_cnt;
		_read.try_lock();
		_write.unlock();
	}

	void unlock_shared()
	{
		if (!--_cnt)
			_read.unlock();
	}
};
          
using strmap_t = std::unordered_multimap<std::string, uint64_t>;
using intmap_t = std::multimap<int32_t, uint64_t>;
using keymap_t = std::map<uint64_t, size_t>;       // structyrs otobragheny 
using recvec_t = std::vector<record_t>;
using keysearch_t = std::set<uint64_t>;

using strmap_vtype = std::pair<std::string, uint64_t>;
using intmap_vtype = std::pair<int32_t, uint64_t>;
using keymap_vtype = std::pair<uint64_t, size_t>;
using response_t = std::pair<record_t *, int32_t>;

const size_t read_size = 4096;
const size_t block_size = 1 * read_size;
const int32_t max_com_len = 4096;
uint max_thread;

const int select_pos_author = 1;
const int select_pos_year1 = 2;
const int select_pos_year2 = 3;
const int select_pos_number1 = 4;
const int select_pos_number2 = 5;

const int remove_pos_ID = 1;

const int update_pos_ID = 1;
const int update_pos_author = 2;
const int update_pos_year = 3;
const int update_pos_number = 4;
const int update_pos_journal = 5;
const int update_pos_article = 6;

const int insert_pos_author = 1;
const int insert_pos_year = 2;
const int insert_pos_number = 3;
const int insert_pos_journal = 4;
const int insert_pos_article = 5;

const char fnamemeta[] = "filemeta";
const char fnamedata[] = "filedata";
const char fnameathrmap[] = "fileathrmap";
const char fnamekeysmap[] = "filekeysmap";
const char fnameyearmap[] = "fileyearmap";
const char fnamenumbmap[] = "filenumbmap";

const boost::regex re_select(" *select *(?>author *= *\\[(.{1,31}?)\\] *)?(?:year *= *\\[ *(\\d+) *, *(\\d+) *\\] *)?(?:number *= *\\[ *(\\d+) *, *(\\d+) *\\] *)?");
const boost::regex re_update(" *update *ID *= *\\[ *(\\d+) *\\] *(?>author *= *\\[(.{1,31}?)\\] *)?(?:year *= *\\[ *(\\d+) *\\] *)?(?:number *= *\\[ *(\\d+) *\\] *)?(?>journal *= *\\[(.{1,63}?)\\] *)?(?>article *= *\\[(.{1,127}?)\\] *)?");
const boost::regex re_remove(" *remove *ID *= *\\[ *(\\d+) *\\] *");
const boost::regex re_insert(" *insert *(?>author *= *\\[(.{1,31}?)\\] *)(?:year *= *\\[ *(\\d+) *\\] *)(?:number *= *\\[ *(\\d+) *\\] *)(?>journal *= *\\[(.{1,63}?)\\] *)(?>article *= *\\[(.{1,127}?)\\] *)");
  
  //  vadelyaem pamyat i cosdaem object


recvec_t data;      // vector iz zapisey
strmap_t athrmap;   // karta po avtoram
keymap_t keysmap;   // karta po ID -> zAPIS
intmap_t yearmap;   // 
intmap_t numbmap;

std::set<size_t> clean_records;  //vocantnye  position
std::atomic<uint64_t> max_ID;

sharedMtx rwmutex;                 
std::vector<uint> thread_ids;
std::mutex thread_run_mutex;          //  // sinization  potocov (16  potocov vsego)
std::mutex thread_edit_mutex;

short tcp_port;      

void dump(void)  //  write v files iz "data"
{
	//compress();
	printf("Dump start\n");
	FILE * fmeta = fopen(fnamemeta, "w");
	FILE * fdata = fopen(fnamedata, "w");
	FILE * fathrmap = fopen(fnameathrmap, "w");
	FILE * fkeysmap = fopen(fnamekeysmap, "w");
	FILE * fyearmap = fopen(fnameyearmap, "w");
	FILE * fnumbmap = fopen(fnamenumbmap, "w");

	size_t s = data.size();
	fprintf(fmeta, "%ld %ld %ld %ld %ld\n",       s, keysmap.size(), athrmap.size(), yearmap.size(), numbmap.size());
	fprintf(fmeta, "%ld %ld\n", (uint64_t) max_ID, clean_records.size());
	fclose(fmeta); 

	fwrite(data.data(), sizeof(record_t), s, fdata);   // otobraghenye  so vsevi "slovami"
	
	for (size_t pos : clean_records)
		fwrite(&pos, sizeof(size_t), 1, fdata);
	fclose(fdata);

	for (auto & elem : keysmap)
	{
		fwrite(&elem.first, sizeof(elem.first), 1, fkeysmap);
		fwrite(&elem.second, sizeof(elem.second), 1, fkeysmap);
	}
	for (auto & elem : athrmap)
	{
		char l = elem.first.length();
		fwrite(&l, 1, 1, fathrmap);
		fwrite(elem.first.c_str(), l, 1, fathrmap);
		fwrite(&elem.second, sizeof(elem.second), 1, fathrmap);
	}
	for (auto & elem : yearmap)
	{
		fwrite(&elem.first, sizeof(elem.first), 1, fyearmap);
		fwrite(&elem.second, sizeof(elem.second), 1, fyearmap);
	}
	for (auto & elem : numbmap)
	{
		fwrite(&elem.first, sizeof(elem.first), 1, fnumbmap);
		fwrite(&elem.second, sizeof(elem.second), 1, fnumbmap);
	}

	fclose(fkeysmap);
	fclose(fathrmap);
	fclose(fyearmap);
	fclose(fnumbmap);
	printf("Dump end\n");
}

void load(void)  //  read v "data"  IZ filov
{
	printf("Load start\n");
	FILE * fmeta = fopen(fnamemeta, "r");
	FILE * fdata = fopen(fnamedata, "r");
	FILE * fathrmap = fopen(fnameathrmap, "r");
	FILE * fkeysmap = fopen(fnamekeysmap, "r");
	FILE * fyearmap = fopen(fnameyearmap, "r");
	FILE * fnumbmap = fopen(fnamenumbmap, "r");

	uint64_t s, k, a, y, n, max;
	size_t cl, pos;
	fscanf(fmeta, "%ld %ld %ld %ld %ld\n", &s, &k, &a, &y, &n);
	fscanf(fmeta, "%ld %ld\n", &max, &cl);
	max_ID = max;
	fclose(fmeta);
	data.resize(s);
	fread(data.data(), sizeof(record_t), s, fdata);
	clean_records.clear();
	for (size_t i = 0; i < cl; ++i)
	{
		fread(&pos, sizeof(size_t), 1, fdata);
		clean_records.insert(pos);
	}
	fclose(fdata);
	
	keysmap.clear();
	for (uint64_t i = 0; i < k; ++i)
	{
		keymap_vtype elem;
		fread(&elem.first, sizeof(elem.first), 1, fkeysmap);
		fread(&elem.second, sizeof(elem.second), 1, fkeysmap);
		keysmap.insert(elem);
	}
	athrmap.clear();
	for (uint64_t i = 0; i < a; ++i)
	{
		char l;
		char buff[40];
		strmap_vtype elem;
		fread(&l, 1, 1, fathrmap);
		fread(buff, l, 1, fathrmap);
		buff[(int32_t)l] = '\0';
		elem.first = std::string(buff);
		fread(&elem.second, sizeof(elem.second), 1, fathrmap);
		athrmap.insert(elem);
	}
	yearmap.clear();
	for (uint64_t i = 0; i < y; ++i)
	{
		intmap_vtype elem;
		fread(&elem.first, sizeof(elem.first), 1, fyearmap);
		fread(&elem.second, sizeof(elem.second), 1, fyearmap);
		yearmap.insert(elem);
	}
	numbmap.clear();
	for (uint64_t i = 0; i < n; ++i)
	{
		intmap_vtype elem;
		fread(&elem.first, sizeof(elem.first), 1, fnumbmap);
		fread(&elem.second, sizeof(elem.second), 1, fnumbmap);
		numbmap.insert(elem);
	}

	fclose(fkeysmap);
	fclose(fathrmap);
	fclose(fyearmap);
	fclose(fnumbmap);
	printf("Load end\n");
}

void select(uint64_t key, record_t * buf)  //read zapis po "keys"  i  "map" mesto v "data"
{
	*buf = data[keysmap[key]];
}

void remove(uint64_t key)   // Delet zapis po "key"
{
	size_t pos = keysmap[key];
	record_t & rec = data[pos];
	auto arange = athrmap.equal_range(std::string(rec.author));
	for (auto it = arange.first; it != arange.second; ++it)
		if (it->second == key)
		{
			athrmap.erase(it);
			break;
		}
	auto yrange = yearmap.equal_range(rec.year);
	for (auto it = yrange.first; it != yrange.second; ++it)
		if (it->second == key)
		{
			yearmap.erase(it);
			break;
		}
	auto nrange = numbmap.equal_range(rec.number);
	for (auto it = nrange.first; it != nrange.second; ++it)
		if (it->second == key)
		{
			numbmap.erase(it);
			break;
		}
	keysmap.erase(key);
	clean_records.insert(pos);
}

void update(uint64_t key, const record_t & record) // corect zapis
{
	size_t pos = keysmap[key];
	record_t & rec = data[pos];
	auto arange = athrmap.equal_range(std::string(rec.author));
	for (auto it = arange.first; it != arange.second; ++it)
		if (it->second == key)
		{
			athrmap.erase(it);
			break;
		}
	auto yrange = yearmap.equal_range(rec.year);
	for (auto it = yrange.first; it != yrange.second; ++it)
		if (it->second == key)
		{
			yearmap.erase(it);
			break;
		}
	auto nrange = numbmap.equal_range(rec.number);
	for (auto it = nrange.first; it != nrange.second; ++it)
		if (it->second == key)
		{
			numbmap.erase(it);
			break;
		}
	rec = record;
	rec.ID = key;
	athrmap.insert({std::move(std::string(record.author)), key});
	yearmap.insert({record.year, key});
	numbmap.insert({record.number, key});
}

void insert(uint64_t key, const record_t & record)  // write zapis
{
	size_t pos;
	if (!clean_records.empty())
	{
		pos = *clean_records.begin();
		clean_records.erase(clean_records.begin());
		data[pos] = record;
	}
	else
	{
		pos = data.size();
		data.push_back(record);
	}
	data[pos].ID = key;
	keysmap[key] = pos;
	athrmap.insert({std::move(std::string(record.author)), key});
	yearmap.insert({record.year, key});
	numbmap.insert({record.number, key});
}

keysearch_t searcha(const std::string & author)
{
    keysearch_t res;
    auto p = athrmap.equal_range(author);
    for (auto it = p.first; it != p.second; ++it)
    {
        res.insert(it->second);
    }
    return res;
}

keysearch_t searchy(int a, int b)
{
    if (a > b)
    	std::swap(a, b);
    keysearch_t res;
    auto left = yearmap.equal_range(a);
    auto right = yearmap.equal_range(b);
    for (auto it = left.first; it != right.second; ++it)
    {
        res.insert(it->second);
    }
    return res;
}

keysearch_t searchn(int a, int b)
{
	if (a > b)
    	std::swap(a, b);
    keysearch_t res;
    auto left = numbmap.equal_range(a);
    auto right = numbmap.equal_range(b);
    for (auto it = left.first; it != right.second; ++it)
    {
        res.insert(it->second);
    }
    return res;
}

response_t execute_select(const boost::cmatch & match_res)
{
	rwmutex.lock_shared();
	std::vector<keysearch_t> sets;
    if (match_res[select_pos_author].matched)
    {
    	sets.push_back(searcha((std::string)match_res[select_pos_author]));
    }
    if (match_res[select_pos_year1].matched)
    {
    	int left, right;
    	try
    	{
    		left = std::stoi((std::string)match_res[select_pos_year1]);
    		right = std::stoi((std::string)match_res[select_pos_year2]);
    	}
    	catch (const std::out_of_range & ex)
    	{
    		rwmutex.unlock_shared();
    		return std::make_pair<record_t *, int32_t> (nullptr, -2);
    	}
    	sets.push_back(searchy(left, right));
    }
    if (match_res[select_pos_number1].matched)
    {
    	int left, right;
    	try
    	{
    		left = std::stoi((std::string)match_res[select_pos_number1]);
    		right = std::stoi((std::string)match_res[select_pos_number2]);
    	}
    	catch (const std::out_of_range & ex)
    	{
    		rwmutex.unlock_shared();
    		return std::make_pair<record_t *, int32_t> (nullptr, -3);
    	}
    	sets.push_back(searchn(left, right));
    }
    if (sets.size() == 0)
    {
    	record_t * result = (record_t *) malloc(sizeof(record_t) * keysmap.size());
    	int sz = data.size();
    	for (int i = 0, j = 0; i < sz; ++i, ++j)
    	{
    		if (clean_records.find(i) == clean_records.end())
    			result[j] = data[i];
    		else
    			--j;
    	}
    	sz = keysmap.size();
    	rwmutex.unlock_shared();
    	return std::make_pair<record_t *, int32_t> (std::move(result), std::move(sz));
    }
    if (sets.size() == 1)
    {
    	int sz = sets[0].size();
    	record_t * result = (record_t *) malloc(sizeof(record_t) * sz);
    	auto it = sets[0].begin();
    	for (int i = 0; i < sz; ++i, ++it)
    	{
    		result[i] = data[keysmap[*it]];
    	}
    	rwmutex.unlock_shared();
    	return std::make_pair<record_t *, int32_t> (std::move(result), std::move(sz));
    }
    if (sets.size() > 1)
    {
    	keysearch_t tmp;
    	for (size_t i = 1; i < sets.size(); ++i)
    	{
    		tmp.clear();
    		set_intersection(sets[0].begin(), sets[0].end(),
    						 sets[i].begin(), sets[i].end(),
                  			 std::inserter(tmp, tmp.begin()));
    		tmp.swap(sets[0]);
    	}
    	int sz = sets[0].size();
    	record_t * result = (record_t *) malloc(sizeof(record_t) * sz);
    	auto it = sets[0].begin();
    	for (int i = 0; i < sz; ++i, ++it)
    	{
    		result[i] = data[keysmap[*it]];
    	}
    	rwmutex.unlock_shared();
    	return std::make_pair<record_t *, int32_t> (std::move(result), std::move(sz));
    }
    // Unreachable 
    rwmutex.unlock_shared();
    return std::make_pair<record_t *, int32_t> (nullptr, -4);
}

response_t execute_remove(const boost::cmatch & match_res)
{
	rwmutex.lock();
	int64_t ID;
   	try
   	{
   		ID = std::stoll((std::string)match_res[remove_pos_ID]);
   	}
   	catch (const std::out_of_range & ex)
   	{
   		rwmutex.unlock();
   		return std::make_pair<record_t *, int32_t> (nullptr, -2);
   	}
   	if (keysmap.find(ID) != keysmap.end())
   		remove(ID);
   	rwmutex.unlock();
   	return std::make_pair<record_t *, int32_t> (nullptr, 0);
}

response_t execute_update(const boost::cmatch & match_res)
{
	rwmutex.lock();
	int64_t ID;
   	try
   	{
   		ID = std::stoll((std::string)match_res[update_pos_ID]);
   	}
   	catch (const std::out_of_range & ex)
   	{
   		rwmutex.unlock();
   		return std::make_pair<record_t *, int32_t> (nullptr, -2);
   	}
   	auto it = keysmap.find(ID);
   	if (it == keysmap.end())
   	{
   		rwmutex.unlock();
   		return std::make_pair<record_t *, int32_t> (nullptr, 0);
   	}

   	record_t record = data[it->second];
   	if (match_res[update_pos_author].matched)
    {
    	std::copy(match_res[update_pos_author].first,
    			  match_res[update_pos_author].second,
    			  record.author);
    	record.author[match_res[update_pos_author].length()] = '\0';
    }
    if (match_res[update_pos_year].matched)
    {
    	int year;
   		try
   		{
   			year = std::stoi((std::string)match_res[update_pos_year]);
   		}
   		catch (const std::out_of_range & ex)
   		{
   			rwmutex.unlock();
   			return std::make_pair<record_t *, int32_t> (nullptr, -2);
   		}
   		record.year = year;
    }
    if (match_res[update_pos_number].matched)
    {
    	int number;
   		try
   		{
   			number = std::stoi((std::string)match_res[update_pos_number]);
   		}
   		catch (const std::out_of_range & ex)
   		{
   			rwmutex.unlock();
   			return std::make_pair<record_t *, int32_t> (nullptr, -2);
   		}
   		record.number = number;
    }
    if (match_res[update_pos_journal].matched)
    {
    	std::copy(match_res[update_pos_journal].first,
    			  match_res[update_pos_journal].second,
    			  record.journal);
    	record.journal[match_res[update_pos_journal].length()] = '\0';
    }
    if (match_res[update_pos_article].matched)
    {
    	std::copy(match_res[update_pos_article].first,
    			  match_res[update_pos_article].second,
    			  record.article);
    	record.article[match_res[update_pos_article].length()] = '\0';
    }
    update(ID, record);
    rwmutex.unlock();
    return std::make_pair<record_t *, int32_t> (nullptr, 0);
}

response_t execute_insert(const boost::cmatch & match_res)
{
	record_t record;
    std::copy(match_res[insert_pos_author].first,
    		  match_res[insert_pos_author].second,
    		  record.author);
    record.author[match_res[insert_pos_author].length()] = '\0';
    int year, number;
   	try
   	{
   		year = std::stoi((std::string)match_res[insert_pos_year]);
   		number = std::stoi((std::string)match_res[insert_pos_number]);
   	}
   	catch (const std::out_of_range & ex)
   	{
   		rwmutex.unlock();
   		return std::make_pair<record_t *, int32_t> (nullptr, -2);
   	}
   	record.year = year;
   	record.number = number;
    std::copy(match_res[insert_pos_journal].first,
    		  match_res[insert_pos_journal].second,
    		  record.journal);
    record.journal[match_res[insert_pos_journal].length()] = '\0';
    std::copy(match_res[insert_pos_article].first,
    		  match_res[insert_pos_article].second,
    		  record.article);
    record.article[match_res[insert_pos_article].length()] = '\0';
    int64_t ID = ++max_ID;
    rwmutex.lock();
    insert(ID, record);
    rwmutex.unlock();
    return std::make_pair<record_t *, int32_t> (nullptr, 0);
}

response_t execute_query(char * query)
{
	
	boost::cmatch mres;
	if (boost::regex_match(query, mres, re_select))
		return execute_select(mres);
	if (boost::regex_match(query, mres, re_remove))
		return execute_remove(mres);
	if (boost::regex_match(query, mres, re_update))
		return execute_update(mres);
	if (boost::regex_match(query, mres, re_insert))
		return execute_insert(mres);
    return std::make_pair<record_t *, int32_t> (nullptr, -1);
    // Do C++ programs dream of template memes?
}

void thread_fun(int tid, int sockid)   //  poluchaet  ID potoka (1 / 16) i  ID soceta s kotorm budet rabotat
{

    int chk = 1;
    char com[max_com_len];
    if (sendall(sockid, (char *)&chk, sizeof(int), MSG_NOSIGNAL) < 0)
    {
    	sprintf(com, "send one tid = %2d", tid);
    	perror(com);
    	shutdown(sockid, 2);
    	close(sockid);
	
		thread_edit_mutex.lock();
		thread_ids.push_back(tid);
		thread_run_mutex.unlock();
		thread_edit_mutex.unlock();
    	return;
    }

    if (recvall(sockid, (char *)&chk, sizeof(int), MSG_NOSIGNAL) < 0)
    {
    	sprintf(com, "recv one tid = %2d", tid);
    	perror(com);
    	shutdown(sockid, 2);
    	close(sockid);
	
		thread_edit_mutex.lock();
		thread_ids.push_back(tid);
		thread_run_mutex.unlock();
		thread_edit_mutex.unlock();
    	return;
    }
    
    
    while (true)
    {
    	int32_t len;
        if (recvall(sockid, (char *)&len, sizeof(int), MSG_NOSIGNAL) < 0) //without null-terminator at the end of command
        {
        	sprintf(com, "recv len tid = %2d", tid);
    		perror(com);
    		break;
        } 
        if (len == 0)
        	break;
    	if (chk)
    		len = swapbytes(len);
    	if (len > max_com_len)
    		len = max_com_len;
    	if (recvall(sockid, com, len, MSG_NOSIGNAL) < 0)
    	{
    		sprintf(com, "recv com tid = %2d", tid);
    		perror(com);
    		break;
    	}
    	com[len] = '\0';
    	response_t response = execute_query(com);
    	if (sendall(sockid, (char *)&response.second, sizeof(int), MSG_NOSIGNAL) < 0)
    	{
    		sprintf(com, "send len tid = %2d", tid);
    		perror(com);
    		free(response.first);
    		response.first = nullptr;
    		break;
    	}
    	if (response.second >= 0)
    		if (sendall(sockid, (char *)response.first, response.second * sizeof(record_t), MSG_NOSIGNAL) < 0)
    		{
    			sprintf(com, "send res tid = %2d", tid);
    			perror(com);
    			free(response.first);
    			response.first = nullptr;
    			break;
    		}

    	free(response.first);
    	response.first = nullptr;
#ifdef MYAPP_VERSION_1
    	break;
#endif
    }

    shutdown(sockid, 2);
    close(sockid);

	thread_edit_mutex.lock();
	thread_ids.push_back(tid);
	thread_run_mutex.unlock();
	thread_edit_mutex.unlock();
}

int init(void)
{
	return 0;
}

std::vector<record_t> generate(int count, int art, int aut, int jou, int yearmin, int yearmax, int numbmin, int numbmax)
{
	srand(time(NULL));
	std::vector<record_t> res;
	std::string au("Author");
	std::string ar("Article");
	std::string jo("Journal");
	int r;
	for (int i = 0; i < count; ++i)
	{
		r = rand() % art;
		std::string gart = ar + std::to_string(r);
		r = rand() % jou;
		std::string gjou = jo + std::to_string(r);
		r = rand() % aut;
		std::string gaut = au + std::to_string(r);
		int gy = rand() % (yearmax - yearmin) + yearmin;
		int gn = rand() % (numbmax - numbmin) + numbmin;
		
		record_t rec(i + 1, gjou.c_str(), gy, gn, gaut.c_str(), gart.c_str());
		res.push_back(rec);
	}
	return res;
}

void ggen (void)
{
	std::vector<record_t> gen = generate(1, 200000, 10000, 1000, 1900, 2000, 1, 1000);

	for (int i = 0; i < 1; ++i)
	{
		insert(gen[i].ID, gen[i]);
		if (!(i % 1000))
			std::cout << i << std::endl;
	}
	max_ID = 1;
	dump();
}

int main(int argc, char const *argv[])
{
	if (!(max_thread = std::thread::hardware_concurrency()))
		max_thread = 16;
	tcp_port = 55555;
	for (size_t i = 0; i < max_thread; ++i)
	{
		thread_ids.push_back(i);
	}

	//ggen(); return 0;

	load();

	int a = 5;
	printf("%d\n", a);

	int sock, listener;
    struct sockaddr_in addr;

	listener = socket(AF_INET, SOCK_STREAM, 0);
    if(listener < 0)
    {
        perror("socket");
        exit(1);
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(tcp_port);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if(bind(listener, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("bind");
        exit(5);
    }

    listen(listener, 10);

    while (true)
    {
    	sock = accept(listener, NULL, NULL);
    	if(sock < 0)
        {
            perror("accept");
            exit(3);
        }

        thread_run_mutex.lock();
        thread_edit_mutex.lock();
        uint cur_id = thread_ids.back();
        thread_ids.pop_back();
        if (thread_ids.size() > 0)
        	thread_run_mutex.unlock();
        thread_edit_mutex.unlock();
        std::thread(thread_fun, cur_id, sock).detach();
    }

	return 0;
}

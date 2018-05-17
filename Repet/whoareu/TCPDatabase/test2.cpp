#include <iostream>
#include <string>
#include <cstdlib>
#include <set>
#include <vector>
#include <cstdio>
#include <cstring>
#include <boost/regex.hpp>
#include <typeinfo>
//#include 
#include "mydb.h"

using namespace std;



int main(int argc, char const *argv[])
{
	//record_t rec;

	//char str[] = "select   ";

	string q;
	std::getline(cin, q);
	cout << q;

	cout << RAND_MAX << endl;


	cout << "asjkdh" << endl;

	std::vector<record_t> v;

	v.push_back({1, "j1", 1999, 42, "Au1", "Ar1"});
	v.push_back({2, "j1", 1999, 33, "Au2", "Ar2"});
	v.push_back({3, "j2", 1986, 55, "Au3", "Ar3"});
	v.push_back({4, "j2", 1955, 42, "Au5", "Ar4"});
	v.push_back({5, "j3", 1986, 11, "Au2", "Ar5"});
	v.push_back({6, "j4", 1999, 22, "Au1", "Ar1"});

	record_t & rec =  v[3];

/*
	boost::regex pattern(" *select *(?>author *= *\\[(.+?)\\] *)?(?:year *= *\\[ *(\\d+) *, *(\\d+) *\\] *)?(?:number *= *\\[ *(\\d+) *, *(\\d+) *\\] *)?");
    boost::cmatch mres;
    cout << boost::regex_match(str, mres, pattern) << endl;

    void * ptr = malloc(0);
    cout << ptr << endl;
    free(ptr);

	cout << mres[0].matched <<  ' ' << mres[0] << endl
		 << mres[1].matched <<  ' ' << mres[1] << endl
		 << mres[2].matched <<  ' ' << mres[2] << endl
		 << mres[3].matched <<  ' ' << mres[3] << endl
		 << mres[4].matched <<  ' ' << mres[4] << endl
		 << mres[5].matched <<  ' ' << mres[5] << endl
		 ;

	char kek[100];

	strcpy(kek, ((std::string)(mres[1])).c_str());
	*/

	cout << v[3].ID << endl
		 << v[3].journal << endl
		 << v[3].year << endl
		 << v[3].number << endl
		 << v[3].author << endl
		 << v[3].article << endl
		 ;

	rec = v[2];

	cout << v[3].ID << endl
		 << v[3].journal << endl
		 << v[3].year << endl
		 << v[3].number << endl
		 << v[3].author << endl
		 << v[3].article << endl
		 ;

	return 0;
}
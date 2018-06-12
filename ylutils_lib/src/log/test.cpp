/*
 * =====================================================================================
 * 
 *        Filename:  test.cpp
 * 
 *     Description:  
 * 
 *         Version:  1.0
 *         Created:  2006年06月12日 13时36分50秒 CST
 *        Revision:  none
 *        Compiler:  g++
 * 
 *          Author:  张明辉 (), minghui.zhang@yeelion.com
 *         Company:  亿览科技
 * 
 * =====================================================================================
 */

#include <iostream>
#include <fstream>
#include <set>
#include <ext/hash_set>
#include <map>
#include <string>
#include <iterator>
#include "LocalLog.h"
#include <unistd.h>

using namespace std;
using namespace __gnu_cxx;

namespace __gnu_cxx
{
	template<> struct hash< std::string >
	{
		size_t operator()(const std::string& str) const
		{
			return hash<const char* >()(str.c_str());
		}
	};
}



/*
enum ELogLevel {
	LOG_WARNING,
	LOG_ERROR,
	LOG_INFO,
	LOG_DEBUG
} ;

*/
void *fun(void *arg)
{
	while (0)
	{
		//printf("%u:%ld:%sover over%s\n", pthread_self(), getpid(), (char *)arg, (char *)arg);
		//break;
	}

	return NULL;
}

/*
int main()
{
	ELogLevel l1 = LOG_WARNING;
	ELogLevel l2 = LOG_ERROR;

	//cout << ((l1<l2)?"l1<l2":"l2<l1") << endl;

	//cout << "getpid() : " << getpid() << endl;
	//cout << "pthread_self() : " << pthread_self() << endl;

	pthread_t pid0, pid1, pid2;
	char arg_pid0[32] = "pid0";
	char arg_pid1[32] = "pid1";
	char arg_pid2[32] = "pid2";
	pthread_create(&pid0, NULL, fun, arg_pid0);
	pthread_create(&pid1, NULL, fun, arg_pid1);
	pthread_create(&pid2, NULL, fun, arg_pid2);

	pthread_join(pid0, NULL);
	pthread_join(pid1, NULL);
	pthread_join(pid2, NULL);

	hash_set<string> s;
	ifstream fin("list");
	string line;
	//while (getline(fin, line))
		s.insert(line);

	fin.close();

	cout << "size() : " << s.size() << endl;
	copy(s.begin(), s.end(), ostream_iterator<string>(cout, "\n"));
//	for (map<long long, string>::iterator it = m.begin(); it != m.end(); it++)
//		cout << it->first << ": " << it->second << endl;

	cout << rand() << ":" << rand() << endl;
	return 0;
}
*/
	LocalLog loger;

void *testlog(void *arg)
{
	while (1) {
		loger.appQueryLog("hello");
		usleep(20000);
	}
	return NULL;
}

int main()
{
	loger.openAllStream();
	pthread_t pid[25];
	for (int i = 0; i < 25; i++)
	{
		pthread_create(&pid[i], NULL, testlog, NULL);
	}
	for (int i = 0; i < 25; i++)
	{
		pthread_join(pid[i], NULL);
	}

	return 0;
}

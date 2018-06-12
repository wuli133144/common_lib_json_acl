//
// 数据库连接池
//
#ifndef _MYSQL_CONN_POOL_H_
#define _MYSQL_CONN_POOL_H_

#include "MysqlDB.h"
#include <queue>
#include <pthread.h>

using namespace std;

class MysqlConnPool 
{
public:
	MysqlConnPool() throw(MysqlDBException);
	~MysqlConnPool();

	// 设置数据库的地址
	void setMysqlConfig(const char* ip, int port, const char* user, const char* passwd, const char* db_name);

	// 初始化数据库连接池
	void initConnPool(u_int size) throw(MysqlDBException);
	
	void initConnPoolCharSet(string charsetname) ;
	// 从连接池中取出一个连接
	MysqlDB* get();

	// 向连接池放回一个连接
	void put(MysqlDB* conn);

private:
	MysqlDB*  pMysqlArray;
	queue<MysqlDB*> mysqlQueue;
	pthread_mutex_t mQLock;
	pthread_cond_t mQCond;
	u_int mSize;

	string		mHost;
	u_int		mPort;
	string		mUser;
	string		mPasswd;
	string		mDb;
};

#endif

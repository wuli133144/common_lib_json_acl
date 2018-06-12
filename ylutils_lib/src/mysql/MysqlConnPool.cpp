#include "MysqlConnPool.h"

MysqlConnPool::MysqlConnPool() throw(MysqlDBException):
	pMysqlArray(NULL),
	mSize(0),
	mHost(""),
	mPort(0),
	mUser(""),
	mPasswd(""),
	mDb("")
{
	if(pthread_mutex_init(&mQLock,NULL) != 0)
		throw MysqlDBException("MysqlConnPool::initConnPool: init mutex failed!");

	if(pthread_cond_init(&mQCond,NULL) != 0)
		throw MysqlDBException("MysqlConnPool::initConnPool: init cond failed!");
}

MysqlConnPool::~MysqlConnPool()
{
	if(pMysqlArray)
		delete [] pMysqlArray;	
	
	pthread_mutex_destroy(&mQLock);
	pthread_cond_destroy(&mQCond);
}

void MysqlConnPool::setMysqlConfig(const char* ip, int port, const char* user, const char* passwd, const char* db_name)
{
	mHost = ip;
	mPort = port;
	mUser = user;
	mPasswd = passwd;
	mDb = db_name;
}

void MysqlConnPool::initConnPool(u_int size) throw(MysqlDBException)
{
	mSize = size;
	pMysqlArray = new MysqlDB[size];
	if(!pMysqlArray)
		throw MysqlDBException("MysqlConnPool::initConnPool: allocate mysql array failed!");

	try {
		for(u_int i=0; i<size; i++) {
			pMysqlArray[i].connect(mHost.c_str(), mPort, mUser.c_str(), mPasswd.c_str(), mDb.c_str());
			mysqlQueue.push(&pMysqlArray[i]);
		}
	}catch (MysqlDBException & e) {
		throw e;
	}
}

void MysqlConnPool::initConnPoolCharSet(string charsetname) 
{
	if(pMysqlArray)
	{
		for(u_int i=0; i<mSize; i++) {
			pMysqlArray[i].setCharSet("utf8");
		}
	}
}

MysqlDB* MysqlConnPool::get()
{
	pthread_mutex_lock(&mQLock);
	while(mysqlQueue.empty())
		pthread_cond_wait(&mQCond, &mQLock);
	MysqlDB* mysql = mysqlQueue.front();
	mysqlQueue.pop();
	pthread_mutex_unlock(&mQLock);

	return mysql;
}

void MysqlConnPool::put(MysqlDB* conn)
{
	pthread_mutex_lock(&mQLock);
	mysqlQueue.push(conn);
	pthread_mutex_unlock(&mQLock);
	pthread_cond_signal(&mQCond);
}

#ifdef _TEST

#include <iostream>

int main(int argc, char* argv[])
{
	try {
		MysqlConnPool mysql_pool;
		mysql_pool.setMysqlConfig("localhost", 3306, "root", "", "file_dispatch");
		mysql_pool.initConnPool(2);

		MysqlDB* mysql = mysql_pool.get();
		MYSQL_ROW MyRow;

		string sql = "select pfid, pfname from pfid_info limit 10";
		mysql->execute(sql);
		mysql->getResult();
		while((MyRow = mysql->nextRow())) {
			int pfid = atoi(MyRow[0]);
			string pfname = MyRow[1];
			cout<<pfid<<"\t"<<pfname<<endl;
		}

		mysql_pool.put(mysql);
	} catch (exception &E) {
		cout << E.what() << endl;
		return -1;
	}
	return 0;
}

#endif

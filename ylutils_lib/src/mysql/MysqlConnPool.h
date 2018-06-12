//
// ���ݿ����ӳ�
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

	// �������ݿ�ĵ�ַ
	void setMysqlConfig(const char* ip, int port, const char* user, const char* passwd, const char* db_name);

	// ��ʼ�����ݿ����ӳ�
	void initConnPool(u_int size) throw(MysqlDBException);
	
	void initConnPoolCharSet(string charsetname) ;
	// �����ӳ���ȡ��һ������
	MysqlDB* get();

	// �����ӳطŻ�һ������
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

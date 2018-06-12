//
// 一个连接启动一个线程的服务模型
//

#ifndef _MTService_H_
#define _MTService_H_

#include <string>
#include <pthread.h>
#include "sync/ThreadCondLock.h"
#include "ServerSocket.h"
#include "AcceptSocket.h"

using namespace std;

class MTServiceException : public std::exception
{
	public :
		MTServiceException(const string &msg) throw(): mMsg(msg) {}
		virtual ~MTServiceException() throw() {}
		virtual const char *what() const throw() { return mMsg.c_str(); }
	private :
		string mMsg;
};

typedef void (*Func)(AcceptSocket* mSocket, void* arg); 

class MTService
{
public:
	MTService(const char* ip, int port) throw(MTServiceException);
	~MTService();

	// 每建立一个连接之后的业务处理函数为dealer，dealer函数中不需要处理mSocket的关闭问题
	void start(Func dealer, void* obj) throw(MTServiceException);

private:
	string mListenIp;
	int mListenPort;

	ServerSocket    mServerSocket;
	int             mAcceptSocketfd;
	int             mShareCount; 
	pthread_mutex_t mLock; 
	pthread_cond_t mCond;

	static void* mThreadFunc(void* arg);
};

#endif

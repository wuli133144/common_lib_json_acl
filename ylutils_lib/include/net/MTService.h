//
// һ����������һ���̵߳ķ���ģ��
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

	// ÿ����һ������֮���ҵ������Ϊdealer��dealer�����в���Ҫ����mSocket�Ĺر�����
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

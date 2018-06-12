#include "MTService.h"
#include <sstream>
#include <signal.h>

MTService::MTService(const char* ip, int port) throw(MTServiceException)
	:
	mListenIp(ip), mListenPort(port)
{
	if(pthread_mutex_init(&mLock,NULL) != 0)
		throw MTServiceException("MTService: init mutex failed!");

	if(pthread_cond_init(&mCond,NULL) != 0)
		throw MTServiceException("MTService: init cond failed!");
}

MTService::~MTService()
{
	pthread_mutex_destroy(&mLock);
	pthread_cond_destroy(&mCond);
}

struct ThrArg
{
	MTService* pSvr;
	Func worker;
	void* obj;
};

void MTService::start(Func dealer, void* obj) throw(MTServiceException)
{
        if(signal(SIGPIPE, SIG_IGN) == SIG_ERR)
                throw MTServiceException("signal() error. throw exception.");

        try {
                mServerSocket.setNoDelay();
                mServerSocket.setReuseAddress(true);

                //!< �󶨵�����˿���.
                mServerSocket.bind(mListenIp.c_str(), mListenPort, 100);

                //!< ��ʼ���տͻ�����.
                while (1) {
                        mAcceptSocketfd = mServerSocket.accept();
			mShareCount = 0;

			ThrArg arg;
			arg.pSvr = this;
			arg.worker = dealer;
			arg.obj = obj;

			pthread_t tid;
			if(pthread_create(&tid, NULL, mThreadFunc, &arg) != 0) {
				ostringstream oss;
				oss << "MTService::dispatch() : pthread_create() error : " << strerror(errno);
				throw MTServiceException(oss.str());
			}

			//! ���̵߳Ⱥ����߳��������Ա�֤���߳���ȷ��ȡ��accept_socket_fd.
			pthread_mutex_lock(&mLock);
			while(mShareCount == 0)
				pthread_cond_wait(&mCond, &mLock);
			mShareCount--;
			pthread_mutex_unlock(&mLock);
                }
        } catch (exception &E) {
                ostringstream oss;
                oss << "MTService::start() catch fatal eception : " << E.what() << " EXIT !!!";
                throw MTServiceException(oss.str());
        }
}

void* MTService::mThreadFunc(void* arg)
{
	pthread_detach(pthread_self());

	//!< ���Ƚ�mAcceptSocketfd��ȡ.
	ThrArg* _arg = (ThrArg*)arg;
	MTService *__boss = _arg->pSvr;
	int accept_socket = __boss->mAcceptSocketfd;

	pthread_mutex_t* __lock = &__boss->mLock;
	pthread_cond_t* __cond = &__boss->mCond;
	int& __sharecond = __boss->mShareCount;

	//!< �����źŸ����߳������������.
	pthread_mutex_lock(__lock);
	if(__sharecond == 0)
		pthread_cond_signal(__cond);
	__sharecond++;
	pthread_mutex_unlock(__lock);

	// ����ҵ���߼�
	AcceptSocket mSocket(accept_socket);
	_arg->worker(&mSocket, _arg->obj);

	return 0;	
}

#ifdef _TEST

#include <iostream>

static void worker(AcceptSocket* mSocket, void* arg)
{
	try {
		while(1) {
			int len;
			char buf[1024];
			mSocket->setReadTimeout(0);
			mSocket->safeRead((char *)&len, sizeof(int));
			mSocket->safeRead(buf, len);
			buf[len] = 0;

			cout<<"remote: "<<mSocket->getRemoteIP()<<endl;

			cout<<len<<"\t"<<buf<<endl;
		} 
	}catch (exception &E) {
		cout<<E.what()<<endl;
	}
}

int main(int argc, char* argv[])
{
	try {
		MTService svr("124.228.252.20", 8091);
		svr.start(worker, NULL);
	}catch(MTServiceException& e) {
		cout<<e.what()<<endl;
	}

	return 0;
}

#endif

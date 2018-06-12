/*
 * =====================================================================================
 * 
 *        Filename:  Thread.cpp
 * 
 *     Description:  
 * 
 *         Version:  1.0
 *         Created:  2006年06月13日 19时51分09秒 CST
 *        Revision:  none
 *        Compiler:  g++
 * 
 *          Author:  张明辉 (), minghui.zhang@yeelion.com
 *         Company:  亿览科技
 * 
 * =====================================================================================
 */

#include "Thread.h"

void *Thread::_threadCaller(void *arg)
{
	Thread *thread = reinterpret_cast<Thread *>(arg);
	if (thread != NULL)
	{
		thread->run();
	}

	return NULL;
}

Thread::Thread()
{
}

Thread::~Thread()
{
}

int Thread::join()
{
	return pthread_join(mThreadID, NULL);
}

bool Thread::start()
{
	pthread_create(&mThreadID, NULL, _threadCaller, this);
	return true;
}

#ifdef _TEST

#include <unistd.h>

class MyThread : public Thread
{
public :
	MyThread() : Thread() {}

	virtual ~MyThread() {}

	void run()
	{
		int i = 9;
		if (pthread_self() == 1084225904*1L)
		{
			cerr << "i++" << endl;
			i++;
		}
		fprintf(stderr, "i am thread %ld \ti = %d\n", pthread_self(), i);
		return;
	}
};

int main()
{
	MyThread *thread = new MyThread[10]();

	for (int i = 0; i < 10; i++)
	{
		thread[i].start();
	}

	for (int i = 0; i < 10; i++)
	{
		thread[i].join();
	}

	delete []thread;

	return 0;
}

#endif //_TEST

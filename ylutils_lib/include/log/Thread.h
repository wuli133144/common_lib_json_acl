/*
 * =====================================================================================
 * 
 *        Filename:  Thread.h
 * 
 *     Description:  �߳̿�.
 * 
 *         Version:  1.0
 *         Created:  2006��06��13�� 19ʱ47��19�� CST
 *        Revision:  none
 *        Compiler:  g++
 * 
 *          Author:  ������ ()
 *         Company:  �����Ƽ�
 *           Email:  minghui.zhang@yeelion.com
 * 
 * =====================================================================================
 */


#ifndef THREAD_H_ZMH_2006_06_13
#define THREAD_H_ZMH_2006_06_13

#include <iostream>
#include <string>

#include <pthread.h>

using namespace std;

class Thread
{
public :
	Thread();

	virtual ~Thread();

	virtual void run() = 0;

	int join();

	bool start();

protected :

private :
	static void *_threadCaller(void *arg);

private :
	pthread_t	mThreadID;
};
#endif //THREAD_H_ZMH_2006_06_13


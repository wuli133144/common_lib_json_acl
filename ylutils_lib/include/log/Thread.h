/*
 * =====================================================================================
 * 
 *        Filename:  Thread.h
 * 
 *     Description:  线程库.
 * 
 *         Version:  1.0
 *         Created:  2006年06月13日 19时47分19秒 CST
 *        Revision:  none
 *        Compiler:  g++
 * 
 *          Author:  张明辉 ()
 *         Company:  亿览科技
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


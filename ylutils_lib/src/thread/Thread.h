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
/** @addtogroup thread
 *  线程函数库
 *  
 *  @{
 */

#ifndef THREAD_H_ZMH_2006_06_13
#define THREAD_H_ZMH_2006_06_13

#include <iostream>
#include <string>

#include <pthread.h>

using namespace std;
/*! \brief 线程虚父类
 *! \class Thread Thread.h thread/Thread.h   
  *  \version 1.0 
  \code
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

  \endcode
 */
class Thread
{
public :
	//!  \brief constructor
	Thread();
	//! \brief  destructor
	virtual ~Thread();
	//! \brief  run
	virtual void run() = 0;
	//!  \brief join
	int join();
	//!  \brief start
	bool start();

protected :

private :
	static void *_threadCaller(void *arg);

private :
	pthread_t	mThreadID;
};
#endif //THREAD_H_ZMH_2006_06_13

/** @} */ //
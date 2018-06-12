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
/** @addtogroup thread
 *  �̺߳�����
 *  
 *  @{
 */

#ifndef THREAD_H_ZMH_2006_06_13
#define THREAD_H_ZMH_2006_06_13

#include <iostream>
#include <string>

#include <pthread.h>

using namespace std;
/*! \brief �߳��鸸��
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
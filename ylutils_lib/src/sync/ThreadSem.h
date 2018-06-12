/*
 * =====================================================================================
 * 
 *        Filename:  ThreadSem.h
 * 
 *     Description:  封装sem_x, sem_x更适合用来线程间的同步. 所以在这个版本里舍弃了进程间
 *                   同步的选项.
 *         Version:  1.0
 *         Created:  2006年06月16日 12时15分52秒 CST
 *        Revision:  none
 *        Compiler:  g++
 * 
 *          Author:  张明辉 ()
 *         Company:  亿览科技
 *           Email:  minghui.zhang@yeelion.com
 * 
 * =====================================================================================
 */
/** @defgroup sync
 *  
 *  
 *  @{
 */

#ifndef THREADSEM_H_ZMH_2006_06_16
#define THREADSEM_H_ZMH_2006_06_16

#include <iostream>
#include <string>
#include <semaphore.h>
#include <errno.h>

#include "log/SingleLog.h"

using namespace std;
/*! 封装sem_x, sem_x更适合用来线程间的同步. 所以在这个版本里舍弃了进程间
 *                   同步的选项.
 *! \class ThreadSem ThreadSem.h sync/ThreadSem.h     
 \code
 int main()
{
	pthread_t pid;
	pthread_create(&pid, NULL, thread_product, NULL);
	pthread_join(pid, NULL);
	return 0;
}
 \endcode
 */
class ThreadSem
{
public :
	//! constructor
	ThreadSem(uint sem_value);
	//! destructor
	virtual ~ThreadSem();
	//! try wait
	int trywait();
	//! wait
	int wait();
	//! post
	int post();
	//! getValue
	int getValue(int &sem_value);
protected :

private :
	sem_t		mSem;
};
#endif //THREADSEM_H_ZMH_2006_06_16
/** @} */ //

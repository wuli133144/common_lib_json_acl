/*
 * =====================================================================================
 * 
 *        Filename:  ThreadCondLock.h
 * 
 *     Description:  �߳����ķ�װ.
 * 
 *         Version:  1.0
 *         Created:  2006��06��15�� 16ʱ48��06�� CST
 *        Revision:  none
 *        Compiler:  g++
 * 
 *          Author:  ������ ()
 *         Company:  �����Ƽ�
 *           Email:  minghui.zhang@yeelion.com
 * 
 * =====================================================================================
 */
/** @addtogroup sync
 *  
 *  
 *  @{
 */

#ifndef THREADLOCK_H_ZMH_2006_06_15
#define THREADLOCK_H_ZMH_2006_06_15

#include <iostream>
#include <string>

#include "log/SingleLog.h"

using namespace std;
/*! 
 *   ThreadCondLockException
 */
class ThreadCondLockException : std::exception
{
public :
	ThreadCondLockException(const string &msg) throw() : mMsg(msg) {}

	virtual ~ThreadCondLockException() throw()  {}

	virtual const char *what() const throw() { return mMsg.c_str(); }

private :
	string mMsg;
};
/*! �߳����ķ�װ.
*! \class ThreadCondLock ThreadCondLock.h sync/ThreadCondLock.h     
\code
int main()
{
	pthread_t pid0, pid1, pid2, pid3;

	pthread_create(&pid0, NULL, thread1, NULL);
	pthread_create(&pid1, NULL, thread1, NULL);
	pthread_create(&pid2, NULL, thread2, NULL);
	pthread_create(&pid3, NULL, thread2, NULL);

	pthread_join(pid0, NULL);
	pthread_join(pid1, NULL);
	pthread_join(pid2, NULL);
	pthread_join(pid3, NULL);

	return 0;
}
\endcode
 */
class ThreadCondLock
{
public :
	//! construct
	ThreadCondLock(const pthread_mutexattr_t *attr = NULL);
	//! destruct
	virtual ~ThreadCondLock();
	//! trylock
	int trylock();
	//! lock
	int lock();
	//! timedlock
	int timedlock(const struct timespec *restrict_abstime);
	//! unlock()
	int unlock();
	//! signal
	int signal();
	//! broadcast
	int broadcast();
	//! wait
	int wait();
	// timedwait
	int timedwait(const struct timespec *restrict_abstime);

protected :

private :
	pthread_mutexattr_t		mMutexAttr;
	pthread_mutex_t			mMutex;
	pthread_cond_t			mCond;
};
#endif //THREADLOCK_H_ZMH_2006_06_15
/** @} */ //

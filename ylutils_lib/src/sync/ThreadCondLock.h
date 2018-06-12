/*
 * =====================================================================================
 * 
 *        Filename:  ThreadCondLock.h
 * 
 *     Description:  线程锁的封装.
 * 
 *         Version:  1.0
 *         Created:  2006年06月15日 16时48分06秒 CST
 *        Revision:  none
 *        Compiler:  g++
 * 
 *          Author:  张明辉 ()
 *         Company:  亿览科技
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
/*! 线程锁的封装.
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

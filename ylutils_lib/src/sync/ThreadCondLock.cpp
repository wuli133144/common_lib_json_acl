/*
 * =====================================================================================
 * 
 *        Filename:  ThreadCondLock.cpp
 * 
 *     Description:  
 * 
 *         Version:  1.0
 *         Created:  2006年06月15日 17时19分28秒 CST
 *        Revision:  none
 *        Compiler:  g++
 * 
 *          Author:  张明辉 (), minghui.zhang@yeelion.com
 *         Company:  亿览科技
 * 
 * =====================================================================================
 */

#include "ThreadCondLock.h"


ThreadCondLock::ThreadCondLock(const pthread_mutexattr_t *attr)
{
	pthread_mutexattr_init(&mMutexAttr);
	int ret = 0;
	if ((attr != NULL) && (ret=pthread_mutexattr_settype(&mMutexAttr, *(int *)attr)) != 0)
	{
		SingleLog::getInstance()->writeLog(ELOGWARNING, "pthread_mutexattr_settype(mutexattr, %d) : %s", *(int *)attr, strerror(ret));
	}

	//! init the mutex && cond, they nerver return error code.
	pthread_mutex_init(&mMutex, &mMutexAttr);

	pthread_cond_init(&mCond, NULL);
}

ThreadCondLock::~ThreadCondLock()
{
	int ret = 0;
	if ((ret=pthread_mutex_destroy(&mMutex)) != 0)
	{
		SingleLog::getInstance()->writeLog(ELOGWARNING, "pthread_mutex_destroy(mutex) : %s", strerror(ret));
	}

	if ((ret=pthread_cond_destroy(&mCond)) != 0)
	{
		SingleLog::getInstance()->writeLog(ELOGWARNING, "pthread_cond_destroy(cond) : %s", strerror(ret));
	}
}

int ThreadCondLock::trylock()
{
	int ret = 0;;

	if ((ret=pthread_mutex_trylock(&mMutex)) != 0)
	{
		SingleLog::getInstance()->writeLog(ELOGWARNING, "pthread_mutex_trylock(mutex) : %s", strerror(ret));
	}

	return ret;
}

int ThreadCondLock::lock()
{
	int ret = 0;;

	if ((ret=pthread_mutex_lock(&mMutex)) != 0)
	{
		SingleLog::getInstance()->writeLog(ELOGWARNING, "pthread_mutex_lock(mutex) : %s", strerror(ret));
	}

	return ret;
}

int ThreadCondLock::timedlock(const struct timespec *restrict_abstime)
{
	int ret = 0;;

	if ((ret=pthread_mutex_timedlock(&mMutex, restrict_abstime)) != 0)
	{
		SingleLog::getInstance()->writeLog(ELOGWARNING, "pthread_mutex_timedlock(mutex) : %s", strerror(ret));
	}

	return ret;
}

int ThreadCondLock::unlock()
{
	int ret = 0;;

	if ((ret=pthread_mutex_unlock(&mMutex)) != 0)
	{
		SingleLog::getInstance()->writeLog(ELOGWARNING, "pthread_mutex_unlock(mutex) : %s", strerror(ret));
	}

	return ret;
}

int ThreadCondLock::signal()
{
	int ret = 0;;

	if ((ret=pthread_cond_signal(&mCond)) != 0)
	{
		SingleLog::getInstance()->writeLog(ELOGWARNING, "pthread_cond_signal(cond) : %s", strerror(ret));
	}

	return ret;
}

int ThreadCondLock::wait()
{
	int ret = 0;;

	if ((ret=pthread_cond_wait(&mCond, &mMutex)) != 0)
	{
		SingleLog::getInstance()->writeLog(ELOGWARNING, "pthread_cond_wait(cond, mutex) : %s", strerror(ret));
	}

	return ret;
}

int ThreadCondLock::timedwait(const struct timespec *restrict_abstime)
{
	int ret = 0;;

	if ((ret=pthread_cond_timedwait(&mCond, &mMutex, restrict_abstime)) != 0)
	{
		SingleLog::getInstance()->writeLog(ELOGWARNING, "pthread_cond_timedwait(cond, mutex, restrict_abstime) : %s", strerror(ret));
	}

	return ret;
}

int ThreadCondLock::broadcast()
{
	int ret = 0;;

	if ((ret=pthread_cond_broadcast(&mCond)) != 0)
	{
		SingleLog::getInstance()->writeLog(ELOGWARNING, "pthread_cond_broadcast(cond) : %s", strerror(ret));
	}

	return ret;
}


#ifdef _TEST

//! global
static ThreadCondLock lock;
static vector<int> v;

//! get an integer from v.
void *thread1(void *arg)
{
	while (1)
	{
		lock.lock();
		while (v.empty())
			lock.wait();
		cerr << pthread_self() << "get an integer : " << v.back() << endl;
		v.pop_back();
		lock.unlock();
	}

	return NULL;
}

//! put an integer into v.
void *thread2(void *arg)
{
	int count = 0;
	while (1)
	{
		lock.lock();
		if (v.empty())
			lock.signal();
		cerr << pthread_self() << "put an integer : " << count << endl;
		v.push_back(count++);
		lock.unlock();

		sleep(1);
		cout << "\n\n##############################\n";
	}

	return NULL;
}

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
#endif //_TEST

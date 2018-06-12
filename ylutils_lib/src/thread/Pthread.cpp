/*
 * =====================================================================================
 * 
 *        Filename:  Pthread.cpp
 * 
 *     Description:  
 * 
 *         Version:  1.0
 *         Created:  2006年06月20日 16时04分19秒 CST
 *        Revision:  none
 *        Compiler:  g++
 * 
 *          Author:  张明辉 (), minghui.zhang@yeelion.com
 *         Company:  亿览科技
 * 
 * =====================================================================================
 */

#include "Pthread.h"


int YeePthreadUtil::Pthread_create(pthread_t  *thread, pthread_attr_t *attr, void * (*start_routine)(void *), void *arg)
{
	int ret = 0;
	if ((ret=pthread_create(thread, attr, start_routine, arg)) != 0)
	{
		SingleLog::getInstance()->writeLog(ELOGWARNING, "Pthread_create()::pthread_create() errno : %d\terrmsg : %s", ret, strerror(ret));
	}

	return ret;
}

pthread_t YeePthreadUtil::Pthread_self(void)
{
	return pthread_self();
}

int YeePthreadUtil::Pthread_join(pthread_t th, void **thread_return)
{
	int ret = 0;
	if ((ret=pthread_join(th, thread_return)) != 0)
	{
		SingleLog::getInstance()->writeLog(ELOGWARNING, "Pthread_join() : pthread_join()  errno : %d\terrmsg : %s", ret, strerror(ret));
	}

	return ret;
}

int YeePthreadUtil::Pthread_detach(pthread_t th)
{
	int ret = 0;
	if ((ret=pthread_detach(th)) != 0)
	{
		SingleLog::getInstance()->writeLog(ELOGWARNING, "Pthread_detach() : pthread_detach()  errno : %d\terrmsg : %s", ret, strerror(ret));
	}

	return ret;
}

void YeePthreadUtil::Pthread_exit(void *retval)
{
	pthread_exit(retval);
	return;
}

int YeePthreadUtil::Pthread_sigmask(int how, const sigset_t *newmask, sigset_t *oldmask)
{
	int ret = 0;
	if ((ret=pthread_sigmask(how, newmask, oldmask)) != 0)
	{
		SingleLog::getInstance()->writeLog(ELOGWARNING, "Pthread_sigmask() : pthread_sigmask()  errno : %d\terrmsg : %s", ret, strerror(ret));
	}

	return ret;
}

int YeePthreadUtil::Pthread_kill(pthread_t thread, int signo)
{
	int ret = 0;
	if ((ret=pthread_kill(thread, signo)) != 0)
	{
		SingleLog::getInstance()->writeLog(ELOGWARNING, "Pthread_kill() : pthread_kill()  errno : %d\terrmsg : %s", ret, strerror(ret));
	}

	return ret;
}

int YeePthreadUtil::Sigwait(const sigset_t *set, int *sig)
{
	int ret = 0;
	if ((ret=sigwait(set, sig)) != 0)
	{
		SingleLog::getInstance()->writeLog(ELOGWARNING, "Sigwait() : sigwait()  errno : %d\terrmsg : %s", ret, strerror(ret));
	}

	return ret;
}

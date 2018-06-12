/*
 * =====================================================================================
 * 
 *        Filename:  ThreadSem.cpp
 * 
 *     Description:  
 * 
 *         Version:  1.0
 *         Created:  2006年06月16日 12时18分41秒 CST
 *        Revision:  none
 *        Compiler:  g++
 * 
 *          Author:  张明辉 (), minghui.zhang@yeelion.com
 *         Company:  亿览科技
 * 
 * =====================================================================================
 */

#include "ThreadSem.h"

ThreadSem::ThreadSem(uint sem_value)
{
	int ret = 0;
	if ((ret=sem_init(&mSem, 0, sem_value)) != 0)
	{
		SingleLog::getInstance()->writeLog(ELOGWARNING, "ThreadSem::ThreadSem(%u) : sem_init(sem, 0, %u) : %s", sem_value, sem_value, strerror(ret));
	}
}

ThreadSem::~ThreadSem()
{
	int ret = 0;
	if ((ret=sem_destroy(&mSem)) != 0)
	{
		SingleLog::getInstance()->writeLog(ELOGWARNING, "ThreadSem::~ThreadSem() : sem_destroy(sem) : %s",strerror(ret));
	}
}

int ThreadSem::trywait()
{
	int ret = 0;
	if ((ret=sem_trywait(&mSem)) != 0)
	{
		SingleLog::getInstance()->writeLog(ELOGWARNING, "ThreadSem::trywati() : sem_trywait(sem) : %s",strerror(ret));
	}

	return ret;
}

int ThreadSem::wait()
{
	int ret = 0;
	if ((ret=sem_wait(&mSem)) != 0)
	{
		SingleLog::getInstance()->writeLog(ELOGWARNING, "ThreadSem::wait() : sem_wait(sem) : %s",strerror(ret));
	}

	return ret;
}

int ThreadSem::post()
{
	int ret = 0;
	if ((ret=sem_post(&mSem)) != 0)
	{
		SingleLog::getInstance()->writeLog(ELOGWARNING, "ThreadSem::post() : sem_post(sem) : %s",strerror(ret));
	}

	return ret;
}

int ThreadSem::getValue(int &sem_value)
{
	int ret = 0;
	if ((ret=sem_getvalue(&mSem, &sem_value)) != 0)
	{
		SingleLog::getInstance()->writeLog(ELOGWARNING, "ThreadSem::getValue() : sem_getvalue(sem, value) : %s",strerror(ret));
	}

	return ret;
}

#ifdef _TEST

void *thread_product(void *arg)
{
	return NULL;
}

void *thread_customer(void *arg)
{
	return NULL;
}

int main()
{
	pthread_t pid;
	pthread_create(&pid, NULL, thread_product, NULL);
	pthread_join(pid, NULL);
	return 0;
}
#endif //_TEST

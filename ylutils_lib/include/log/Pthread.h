/*
 * =====================================================================================
 * 
 *        Filename:  Pthread.h
 * 
 *     Description:  带日志的线程函数, 线程间同步的工具在sync里进行了封装.
 * 
 *         Version:  1.0
 *         Created:  2006年06月19日 14时56分20秒 CST
 *        Revision:  none
 *        Compiler:  g++
 * 
 *          Author:  张明辉 ()
 *         Company:  亿览科技
 *           Email:  minghui.zhang@yeelion.com
 * 
 * =====================================================================================
 */


#ifndef PTHREAD_H_ZMH_2006_06_19
#define PTHREAD_H_ZMH_2006_06_19

#include <pthread.h>
#include <signal.h>

#include "log/SingleLog.h"

//! see pthread_create.
int Pthread_create(pthread_t  *thread, pthread_attr_t *attr, void * (*start_routine)(void *), void *arg);

//! see pthread_self.
pthread_t Pthread_self(void);

//! see pthread_join
int Pthread_join(pthread_t th, void **thread_return);

//! see pthread_detach.
int Pthread_detach(pthread_t th);

//! see pthread_exit().
void Pthread_exit(void *retval);



//! see pthread_X about signal.
int Pthread_sigmask(int how, const sigset_t *newmask, sigset_t *oldmask);
int Pthread_kill(pthread_t thread, int signo);
int Sigwait(const sigset_t *set, int *sig);


#endif //PTHREAD_H_ZMH_2006_06_19


/*
 * =====================================================================================
 * 
 *        Filename:  Pthread.h
 * 
 *     Description:  ����־���̺߳���, �̼߳�ͬ���Ĺ�����sync������˷�װ.
 * 
 *         Version:  1.0
 *         Created:  2006��06��19�� 14ʱ56��20�� CST
 *        Revision:  none
 *        Compiler:  g++
 * 
 *          Author:  ������ ()
 *         Company:  �����Ƽ�
 *           Email:  minghui.zhang@yeelion.com
 * 
 * =====================================================================================
 */
/** @defgroup thread
 *  �̺߳�����
 *  
 *  @{
 */

#ifndef PTHREAD_H_ZMH_2006_06_19
#define PTHREAD_H_ZMH_2006_06_19

#include <pthread.h>
#include <signal.h>

#include "log/SingleLog.h"
/*!\brief ��װ�˴���־���̺߳���, �̼߳�ͬ���Ĺ�����sync������˷�װ.
 *  	google posix thread lib for more information
 *! \class YeePthreadUtil YeePthreadUtil.h thread/YeePthreadUtil.h   
 */
class YeePthreadUtil{
public:
	//! see pthread_create.
static	int Pthread_create(pthread_t  *thread, pthread_attr_t *attr, void * (*start_routine)(void *), void *arg);
	
	//! see pthread_self.
static	pthread_t Pthread_self(void);
	
	//! see pthread_join
static	int Pthread_join(pthread_t th, void **thread_return);
	
	//! see pthread_detach.
static	int Pthread_detach(pthread_t th);
	
	//! see pthread_exit().
static	void Pthread_exit(void *retval);
	
	
	
	//! see pthread_X about signal.
static	int Pthread_sigmask(int how, const sigset_t *newmask, sigset_t *oldmask);
	//! see pthread_X about signal.
static	int Pthread_kill(pthread_t thread, int signo);
	//! see pthread_X about signal.
static	int Sigwait(const sigset_t *set, int *sig);
	
};
#endif //PTHREAD_H_ZMH_2006_06_19
/** @} */ //

/*
 * =====================================================================================
 * 
 *        Filename:  ThreadSem.h
 * 
 *     Description:  ��װsem_x, sem_x���ʺ������̼߳��ͬ��. ����������汾�������˽��̼�
 *                   ͬ����ѡ��.
 *         Version:  1.0
 *         Created:  2006��06��16�� 12ʱ15��52�� CST
 *        Revision:  none
 *        Compiler:  g++
 * 
 *          Author:  ������ ()
 *         Company:  �����Ƽ�
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
/*! ��װsem_x, sem_x���ʺ������̼߳��ͬ��. ����������汾�������˽��̼�
 *                   ͬ����ѡ��.
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

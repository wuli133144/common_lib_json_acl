/*
 * =====================================================================================
 * 
 *        Filename:  SafeVectorBySem.h
 * 
 *     Description:  pthread_cond_signal有时侯会有信号丢失的可能. sem_x是共享内存实现的.
 *                   所以在某种程度上会比较安全.
 * 
 *         Version:  1.0
 *         Created:  2006年06月16日 18时07分49秒 CST
 *        Revision:  none
 *        Compiler:  g++
 * 
 *          Author:  张明辉 ()
 *         Company:  亿览科技
 *           Email:  minghui.zhang@yeelion.com
 * 
 * =====================================================================================
 */
/** @addtogroup container
 *   容器库
 *  
 *  @{
 */

#ifndef SAFEVECTORBYSEM_H_ZMH_2006_06_16
#define SAFEVECTORBYSEM_H_ZMH_2006_06_16

#include <iostream>
#include <string>

#include "SafeContainerBySem.h"
#include "Vector.h"

using namespace std;

/*
template <class Class>
class SafeVectorBySem
{
public :
	SafeVectorBySem();

	SafeVectorBySem(size_t n);

	SafeVectorBySem(size_t n, const Class &object);

	SafeVectorBySem(const vector<Class> &other);

	virtual ~SafeVectorBySem();

	void push_back(const Class &object);

	void pop_back(Class &object);

	void clear();
protected :

private :
	ThreadSem	mSizeSem;		//!< 产品个数信号.
	ThreadSem	mMutexSem;		//!< 二值信号锁.
	vector<Class>	mVector;		//!< 容器.
};

template <class Class>
SafeVectorBySem<Class>::SafeVectorBySem()
	:
	mSizeSem(0),
	mMutexSem(1)
{
}

template <class Class>
SafeVectorBySem<Class>::SafeVectorBySem(size_t n)
	:
	mSizeSem(0),
	mMutexSem(1),
	mVector(n)
{
}

template <class Class>
SafeVectorBySem<Class>::SafeVectorBySem(size_t n, const Class &object)
	:
	mSizeSem(0),
	mMutexSem(1),
	mVector(n, object)
{
}

template <class Class>
SafeVectorBySem<Class>::SafeVectorBySem(const vector<Class> &other)
	:
	mSizeSem(other.size()),
	mMutexSem(1),
	mVector(other)
{
}

template <class Class>
SafeVectorBySem<Class>::~SafeVectorBySem()
{
}

template <class Class>
void SafeVectorBySem<Class>::push_back(const Class &object)
{
	mMutexSem.wait();
	mVector.push_back(object);
	mSizeSem.post();
	mMutexSem.post();

	return;
}

template <class Class>
void SafeVectorBySem<Class>::pop_back(Class &object)
{
	mSizeSem.wait();
	mMutexSem.wait();
	object = mVector.back();
	mVector.pop_back();
	mMutexSem.post();

	return;
}
*/

typedef SafeContainerBySem<Vector, int> SafeVectorBySem;

#endif //SAFEVECTORBYSEM_H_ZMH_2006_06_16
/** @} */ //

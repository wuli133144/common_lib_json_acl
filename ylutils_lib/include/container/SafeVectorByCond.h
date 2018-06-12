/*
 * =====================================================================================
 * 
 *        Filename:  SafeVectorByCond.h
 * 
 *     Description:  �̰߳�ȫ��vector. ��֧�ֳ��õķ���.
 * 
 *         Version:  1.0
 *         Created:  2006��06��14�� 16ʱ50��23�� CST
 *        Revision:  none
 *        Compiler:  g++
 * 
 *          Author:  ������ ()
 *         Company:  �����Ƽ�
 *           Email:  minghui.zhang@yeelion.com
 * 
 * =====================================================================================
 */
/** @addtogroup container
 *   ������
 *  
 *  @{
 */

#ifndef SAFEVECTOR_H_ZMH_2006_06_14
#define SAFEVECTOR_H_ZMH_2006_06_14

#include <iostream>
#include <string>
#include <vector>

#include "SafeContainerByCond.h"
#include "Vector.h"

using namespace std;

/**
 * ����ʵ�ֵĲ���ͨ�õİ汾.
template <class Class>
class SafeVectorByCond
{
public :
	SafeVectorByCond();
	
	SafeVectorByCond(size_t n);

	SafeVectorByCond(size_t n, const Class &object);

	virtual ~SafeVectorByCond();

	void push_back(const Class &object);

	void pop_back(Class &object);

	bool pop_back_timeout(Class &object, long secs);

	void clear();

	void resize(size_t n, const Class &object = Class());

protected :

private :
	ThreadCondLock		mCondLock;
	vector<Class>		mVector;
};

template <class Class>
SafeVectorByCond<Class>::SafeVectorByCond()
	:
	mCondLock(NULL)
{
}

template <class Class>
SafeVectorByCond<Class>::SafeVectorByCond(size_t n)
	:
	mCondLock(NULL),
	mVector(n)
{
}

template <class Class>
SafeVectorByCond<Class>::SafeVectorByCond(size_t n, const Class &object)
	:
	mCondLock(NULL),
	mVector(n, object)
{
}

template <class Class>
SafeVectorByCond<Class>::~SafeVectorByCond()
{
}

template <class Class>
void SafeVectorByCond<Class>::push_back(const Class &object)
{
	mCondLock.lock();
	if (mVector.empty())
		mCondLock.signal();
	mVector.push_back(object);
	mCondLock.unlock();

	return;
}

template <class Class>
void SafeVectorByCond<Class>::pop_back(Class &object)
{
	mCondLock.lock();
	while (mVector.empty())
		mCondLock.wait();
	object = mVector.back();
	mVector.pop_back();
	mCondLock.unlock();

	return;
}

template <class Class>
bool SafeVectorByCond<Class>::pop_back_timeout(Class &object, long secs)
{
	struct timespec ts;
	ts.tv_sec = time(NULL) + secs;
	ts.tv_nsec = 0;

	mCondLock.lock();
	while (mVector.empty())
	{
		if (mCondLock.timedwait(&ts) == ETIMEDOUT)
		{
			mCondLock.unlock();
			return false;
		}
	}
	object = mVector.back();
	mVector.pop_back();
	mCondLock.unlock();

	return true;
}

template <class Class>
void SafeVectorByCond<Class>::clear()
{
	mCondLock.lock();
	mVector.clear();
	mCondLock.unlock();

	return;
}

template <class Class>
void SafeVectorByCond<Class>::resize(size_t n, const Class &object)
{
	mCondLock.lock();
	if (mVector.empty() && (n!=0))
		mCondLock.signal();
	mVector.resize(n, object);
	mCondLock.unlock();

	return;
}
*/

typedef SafeContainerByCond<Vector, int> SafeVectorByCond;

#endif //SAFEVECTOR_H_ZMH_2006_06_14
/** @} */ //

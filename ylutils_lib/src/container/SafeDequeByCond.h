/*
 * =====================================================================================
 * 
 *        Filename:  SafeDequeByCond.h
 * 
 *     Description:  线程安全的双端队列.
 * 
 *         Version:  1.0
 *         Created:  2006年06月14日 18时49分52秒 CST
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

#ifndef SAFEDEQUE_H_ZMH_2006_06_14
#define SAFEDEQUE_H_ZMH_2006_06_14

#include <iostream>
#include <string>
#include <deque>

#include "SafeContainerByCond.h"
#include "Deque.h"

using namespace std;

/**
 * 早期不最通用的版本.
template <class Class>
class SafeDequeByCond
{
public :
	SafeDequeByCond();

	SafeDequeByCond(size_t n);

	SafeDequeByCond(size_t n, const Class &object);

	SafeDequeByCond(const deque<Class> &other);

	SafeDequeByCond &operator=(const deque<Class> &other);

	virtual ~SafeDequeByCond();

	//! methods
	void push_back(const Class &object);

	void push_front(const Class &object);

	void pop_back(Class &object);

	void pop_front(Class &object);

	bool pop_back(Class &object, long secs);

	bool pop_front(Class &object, long secs);

	void clear();

	void resize(size_t n, const Class &object = Class());
protected :

private :
	ThreadCondLock		mCondLock;
	deque<Class>		mDeque;
};

template <class Class>
SafeDequeByCond<Class>::SafeDequeByCond()
	:
	mCondLock(NULL)
{
}

template <class Class>
SafeDequeByCond<Class>::SafeDequeByCond(size_t n)
	:
	mCondLock(NULL),
	mDeque(n)
{
}

template <class Class>
SafeDequeByCond<Class>::SafeDequeByCond(size_t n, const Class &object)
	:
	mCondLock(NULL),
	mDeque(n, object)
{
}

template <class Class>
SafeDequeByCond<Class>::SafeDequeByCond(const deque<Class> &other)
	:
	mCondLock(NULL),
	mDeque(other)
{
}

template <class Class>
SafeDequeByCond<Class> &SafeDequeByCond<Class>::operator=(const deque<Class> &other)
{
	mCondLock.lock();
	if (mDeque.empty() && !other.empty())
		mCondLock.signal();
	mDeque = other;
	mCondLock.unlock();

	return *this;
}

template <class Class>
SafeDequeByCond<Class>::~SafeDequeByCond()
{
}

template <class Class>
void SafeDequeByCond<Class>::push_back(const Class &object)
{
	mCondLock.lock();
	if (mDeque.empty())
		mCondLock.signal();
	mDeque.push_back(object);
	mCondLock.unlock();

	return;
}

template <class Class>
void SafeDequeByCond<Class>::push_front(const Class &object)
{
	mCondLock.lock();
	if (mDeque.empty())
		mCondLock.signal();
	mDeque.push_front(object);
	mCondLock.unlock();

	return;
}

template <class Class>
void SafeDequeByCond<Class>::pop_back(Class &object)
{
	mCondLock.lock();
	while (mDeque.empty())
		mCondLock.wait();
	object = mDeque.back();
	mDeque.pop_back();
	mCondLock.unlock();

	return;
}

template <class Class>
void SafeDequeByCond<Class>::pop_front(Class &object)
{
	mCondLock.lock();
	while (mDeque.empty())
		mCondLock.wait();
	object = mDeque.front();
	mDeque.pop_front();
	mCondLock.unlock();

	return;
}

template <class Class>
bool SafeDequeByCond<Class>::pop_back(Class &object, long secs)
{
	struct timespec ts;
	ts.tv_sec = time(NULL) + secs;
	ts.tv_nsec = 0;

	mCondLock.lock();
	while (mDeque.empty())
	{
		if (mCondLock.timedwait(&ts) == ETIMEDOUT)
			return false;
	}
	object = mDeque.back();
	mDeque.pop_back();
	mCondLock.unlock();

	return true;
}

template <class Class>
bool SafeDequeByCond<Class>::pop_front(Class &object, long secs)
{
	struct timespec ts;
	ts.tv_sec = time(NULL) + secs;
	ts.tv_nsec = 0;

	mCondLock.lock();
	while (mDeque.empty())
	{
		if (mCondLock.timedwait(&ts) == ETIMEDOUT)
			return false;
	}
	object = mDeque.front();
	mDeque.pop_front();
	mCondLock.unlock();

	return true;
}

template <class Class>
void SafeDequeByCond<Class>::clear()
{
	mCondLock.lock();
	mDeque.clear();
	mCondLock.unlock();

	return;
}

template <class Class>
void SafeDequeByCond<Class>::resize(size_t n, const Class &object)
{
	mCondLock.lock();
	if (mDeque.empty())
		mCondLock.signal();
	mDeque.resize(n, object);
	mCondLock.unlock();

	return;
}
*/

typedef SafeContainerByCond<Deque, int> SafeDequeByCond;
#endif //SAFEDEQUE_H_ZMH_2006_06_14
/** @} */ //

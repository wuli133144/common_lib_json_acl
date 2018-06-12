/*
 * =====================================================================================
 * 
 *        Filename:  SafeDequeBySem.h
 * 
 *     Description:  
 * 
 *         Version:  1.0
 *         Created:  2006��06��16�� 18ʱ57��38�� CST
 *        Revision:  none
 *        Compiler:  g++
 * 
 *          Author:  ������ ()
 *         Company:  �����Ƽ�
 *           Email:  minghui.zhang@yeelion.com
 * 
 * =====================================================================================
 */


#ifndef SAFEDEQUEBYSEM_H_ZMH_2006_06_16
#define SAFEDEQUEBYSEM_H_ZMH_2006_06_16

#include <iostream>
#include <string>
#include <deque>

#include "SafeContainerBySem.h"
#include "Deque.h"

using namespace std;

/*
template <class Class>
class SafeDequeBySem
{
public :
	SafeDequeBySem();

	SafeDequeBySem(size_t n);

	SafeDequeBySem(size_t n, const Class &object);

	SafeDequeBySem(const deque<Class> &other);

	SafeDequeBySem &operator=(const deque<Class> &other);

	//! methods.
	void push_back(const Class &object);

	void push_front(const Class &object);

	void pop_back(Class &object);

	void pop_front(Class &object);

	void clear();

	void resize(size_t n, const Class &object = Class());

protected :

private :
	ThreadSem	mSizeSem;
	ThreadSem	mMutexSem;
	deque<Class>	mDeque;
};

template <class Class>
SafeDequeBySem<Class>::SafeDequeBySem()
	:
	mSizeSem(0),
	mMutexSem(1)
{
}

template <class Class>
SafeDequeBySem<Class>::SafeDequeBySem(size_t n)
	:
	mSizeSem(0),
	mMutexSem(1),
	mDeque(n)
{
}

template <class Class>
SafeDequeBySem<Class>::SafeDequeBySem(size_t n, const Class &object)
	:
	mSizeSem(0),
	mMutexSem(1),
	mDeque(n, object)
{
}

template <class Class>
SafeDequeBySem<Class>::SafeDequeBySem(const deque<Class> &other)
	:
	mSizeSem(other.size()),
	mMutexSem(1),
	mDeque(other)
{
}

template <class Class>
SafeDequeBySem<Class> &SafeDequeBySem<Class>::operator=(const deque<Class> &other)
{
	mMutexSem.wait();
	int value = 0;
	mSizeSem.getValue(value);
	while (value--)
	{
		mSizeSem.wait();
		mDeque.pop_back();
	}
	//mDeque.clear();
	mDeque = other;
	value = mDeque.size();
	while (value--)
		mSizeSem.post();
	mMutexSem.post();

	return *this;
}

template <class Class>
void SafeDequeBySem<Class>::push_back(const Class &object)
{
	mMutexSem.wait();
	mDeque.push_back(object);
	mSizeSem.post();
	mMutexSem.post();

	return;
}

template <class Class>
void SafeDequeBySem<Class>::push_front(const Class &object)
{
	mMutexSem.wait();
	mDeque.push_front(object);
	mSizeSem.post();
	mMutexSem.post();

	return;
}

template <class Class>
void SafeDequeBySem<Class>::pop_back(Class &object)
{
	mSizeSem.wait();
	mMutexSem.wait();
	object = mDeque.back();
	mDeque.pop_back();
	mMutexSem.post();

	return;
}

template <class Class>
void SafeDequeBySem<Class>::pop_front(Class &object)
{
	mSizeSem.wait();
	mMutexSem.wait();
	object = mDeque.front();
	mDeque.pop_front();
	mMutexSem.post();

	return;
}

template <class Class>
void SafeDequeBySem<Class>::clear()
{
	mMutexSem.wait();
	//! ��ʱ���ܼ򵥵�clear. ��Ϊ�п��ܸո����������õ���һ��mSizeSem���ǻ�û�еõ�mMutexSem.
	int value = 0;
	mSizeSem.getValue(value);
	while (value--)
	{
		mSizeSem.wait();
		mDeque.pop_back();
	}
	mMutexSem.post();
	
	return;
}

template <class Class>
void SafeDequeBySem<Class>::resize(size_t n, const Class &object)
{
	//! 
	return;
}
*/

typedef SafeContainerBySem<Deque, int> SafeDequeBySem;
#endif //SAFEDEQUEBYSEM_H_ZMH_2006_06_16


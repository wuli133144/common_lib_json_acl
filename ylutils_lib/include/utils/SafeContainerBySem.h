/*
 * =====================================================================================
 * 
 *        Filename:  SafeContainerBySem.h
 * 
 *     Description:  
 * 
 *         Version:  1.0
 *         Created:  2006年06月17日 13时42分19秒 CST
 *        Revision:  none
 *        Compiler:  g++
 * 
 *          Author:  张明辉 ()
 *         Company:  亿览科技
 *           Email:  minghui.zhang@yeelion.com
 * 
 * =====================================================================================
 */


#ifndef SAFECONTAINERBYSEM_H_ZMH_2006_06_17
#define SAFECONTAINERBYSEM_H_ZMH_2006_06_17

#include <iostream>
#include <string>

#include "SafeContainer.h"
#include "sync/ThreadSem.h"

using namespace std;

template <template<class ItemType1> class ContainerType, class ItemType>
class SafeContainerBySem : public SafeContainer<ContainerType, ItemType>
{
public :
	SafeContainerBySem();

	virtual ~SafeContainerBySem();

	virtual void put(const ItemType &item);

	virtual void get(ItemType &item);

	virtual void push_back(const ItemType &item);

	virtual void push_front(const ItemType &item);

	virtual void pop_back(ItemType &item);

	virtual void pop_front(ItemType &item);

	virtual void clear();

private :
	ThreadSem		mSizeSem;
	ThreadSem		mMutexSem;
};

template <template<class ItemType1> class ContainerType, class ItemType>
SafeContainerBySem<ContainerType, ItemType>::SafeContainerBySem()
	:
	SafeContainer<ContainerType, ItemType>(),
	mSizeSem(0),
	mMutexSem(1)
{
}

template <template<class ItemType1> class ContainerType, class ItemType>
SafeContainerBySem<ContainerType, ItemType>::~SafeContainerBySem()
{
}

template <template<class ItemType1> class ContainerType, class ItemType>
void SafeContainerBySem<ContainerType, ItemType>::put(const ItemType &item)
{
	mMutexSem.wait();
	this->mContainer.put(item);
	mSizeSem.post();
	mMutexSem.post();

	return;
}

template <template<class ItemType1> class ContainerType, class ItemType>
void SafeContainerBySem<ContainerType, ItemType>::get(ItemType &item)
{
	mSizeSem.wait();
	mMutexSem.wait();
	this->mContainer.get(item);
	mMutexSem.post();

	return;
}

template <template<class ItemType1> class ContainerType, class ItemType>
void SafeContainerBySem<ContainerType, ItemType>::push_back(const ItemType &item)
{
	mMutexSem.wait();
	this->mContainer.push_back(item);
	mSizeSem.post();
	mMutexSem.post();

	return;
}

template <template<class ItemType1> class ContainerType, class ItemType>
void SafeContainerBySem<ContainerType, ItemType>::pop_back(ItemType &item)
{
	mSizeSem.wait();
	mMutexSem.wait();
	this->mContainer.pop_back(item);
	mMutexSem.post();

	return;
}

template <template<class ItemType1> class ContainerType, class ItemType>
void SafeContainerBySem<ContainerType, ItemType>::push_front(const ItemType &item)
{
	mMutexSem.wait();
	this->mContainer.push_front(item);
	mSizeSem.post();
	mMutexSem.post();

	return;
}

template <template<class ItemType1> class ContainerType, class ItemType>
void SafeContainerBySem<ContainerType, ItemType>::pop_front(ItemType &item)
{
	mSizeSem.wait();
	mMutexSem.wait();
	this->mContainer.pop_front(item);
	mMutexSem.post();

	return;
}

template <template<class ItemType1> class ContainerType, class ItemType>
void SafeContainerBySem<ContainerType, ItemType>::clear()
{
	mMutexSem.wait();
	int value = 0;
	mSizeSem.getValue(value);
	ItemType item;
	while (value--)
	{
		mSizeSem.wait();
		this->mContainer.pop_back(item);
	}
	mMutexSem.post();

	return;
}
#endif //SAFECONTAINERBYSEM_H_ZMH_2006_06_17


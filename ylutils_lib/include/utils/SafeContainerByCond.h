/*
 * =====================================================================================
 * 
 *        Filename:  SafeContainerByCond.h
 * 
 *     Description:  stl�������ܶ�,һһʵ���̰߳�ȫ��ģ��ȽϷ���, ����ͨ�õ�ģ���Ϊ����
 * 
 *         Version:  1.0
 *         Created:  2006��06��16�� 21ʱ18��23�� CST
 *        Revision:  none
 *        Compiler:  g++
 * 
 *          Author:  ������ ()
 *         Company:  �����Ƽ�
 *           Email:  minghui.zhang@yeelion.com
 * 
 * =====================================================================================
 */


#ifndef SAFECONTAINERBYCOND_H_ZMH_2006_06_16
#define SAFECONTAINERBYCOND_H_ZMH_2006_06_16

#include <iostream>
#include <string>
#include <vector>

#include "SafeContainer.h"
#include "sync/ThreadCondLock.h"

using namespace std;


/*
template <template<class ItemType1> class ContainerType, class ItemType>
class SafeContainerByCond
{
public :
	SafeContainerByCond();

	virtual ~SafeContainerByCond();

	void get(ItemType &item);

	void put(const ItemType &item);

private :
	ContainerType<ItemType>		mContainer;
};

template <template<class ItemType1> class ContainerType, class ItemType>
SafeContainerByCond<ContainerType,ItemType>::SafeContainerByCond()
{
}

template <template<class ItemType1> class ContainerType, class ItemType>
SafeContainerByCond<ContainerType,ItemType>::~SafeContainerByCond()
{
}

template <template<class ItemType1> class ContainerType, class ItemType>
void SafeContainerByCond<ContainerType,ItemType>::get(ItemType &item)
{
	mContainer.get(item);	
	return;
}

template <template<class ItemType1> class ContainerType, class ItemType>
void SafeContainerByCond<ContainerType,ItemType>::put(const ItemType &item)
{
	mContainer.put(item);
	return;
}
*/

template <template<class ItemType1> class ContainerType, class ItemType>
class SafeContainerByCond : public SafeContainer<ContainerType, ItemType>
{
public :
	SafeContainerByCond();

	virtual ~SafeContainerByCond();

	virtual void put(const ItemType &item);

	virtual void get(ItemType &item);

	virtual void push_back(const ItemType &item);

	virtual void push_front(const ItemType &item);

	virtual void pop_back(ItemType &item);

	virtual void pop_front(ItemType &item);

	virtual void clear();

private :
	ThreadCondLock		mCondLock;
};

//! ʵ��.
template <template<class ItemType1> class ContainerType, class ItemType>
SafeContainerByCond<ContainerType, ItemType>::SafeContainerByCond()
	:
	SafeContainer<ContainerType, ItemType>(),
	mCondLock(NULL)
{
}

template <template<class ItemType1> class ContainerType, class ItemType>
SafeContainerByCond<ContainerType, ItemType>::~SafeContainerByCond()
{
}

template <template<class ItemType1> class ContainerType, class ItemType>
void SafeContainerByCond<ContainerType, ItemType>::put(const ItemType &item)
{
	mCondLock.lock();
	if (this->mContainer.empty())
		mCondLock.signal();
	this->mContainer.put(item);
	mCondLock.unlock();

	return;
}

template <template<class ItemType1> class ContainerType, class ItemType>
void SafeContainerByCond<ContainerType, ItemType>::get(ItemType &item)
{
	mCondLock.lock();
	while (this->mContainer.empty())
		mCondLock.wait();
	this->mContainer.get(item);
	mCondLock.unlock();

	return;
}

template <template<class ItemType1> class ContainerType, class ItemType>
void SafeContainerByCond<ContainerType, ItemType>::push_back(const ItemType &item)
{
	mCondLock.lock();
	if (this->mContainer.empty())
		mCondLock.signal();
	this->mContainer.push_back(item);
	mCondLock.unlock();

	return;
}

template <template<class ItemType1> class ContainerType, class ItemType>
void SafeContainerByCond<ContainerType, ItemType>::pop_back(ItemType &item)
{
	mCondLock.lock();
	while (this->mContainer.empty())
		mCondLock.wait();
	this->mContainer.pop_back(item);
	mCondLock.unlock();

	return;
}

template <template<class ItemType1> class ContainerType, class ItemType>
void SafeContainerByCond<ContainerType, ItemType>::push_front(const ItemType &item)
{
	mCondLock.lock();
	if (this->mContainer.empty())
		mCondLock.signal();
	this->mContainer.push_front(item);
	mCondLock.unlock();

	return;
}

template <template<class ItemType1> class ContainerType, class ItemType>
void SafeContainerByCond<ContainerType, ItemType>::pop_front(ItemType &item)
{
	mCondLock.lock();
	while (this->mContainer.empty())
		mCondLock.wait();
	this->mContainer.pop_front(item);
	mCondLock.unlock();

	return;
}

template <template<class ItemType1> class ContainerType, class ItemType>
void SafeContainerByCond<ContainerType, ItemType>::clear()
{
	mCondLock.lock();
	this->mContainer.clear();
	mCondLock.unlock();

	return;
}

#endif //SAFECONTAINERBYCOND_H_ZMH_2006_06_16


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

/** @addtogroup container
 *   ������
 *  
 *  @{
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
/*! 
 * \brief  stl�������ܶ�,һһʵ���̰߳�ȫ��ģ��ȽϷ���, ����ͨ�õ�ģ���Ϊ����
 *  \class SafeContainer SafeContainerByCond.h container/SafeContainerByCond.h    
 */
template <template<class ItemType1> class ContainerType, class ItemType>
class SafeContainerByCond : public SafeContainer<ContainerType, ItemType>
{
public :
	//! constructor
	SafeContainerByCond();
	//! destructor
	virtual ~SafeContainerByCond();
	//!  put
	virtual void put(const ItemType &item);
	//! get
	virtual void get(ItemType &item);
	//!  push_back
	virtual void push_back(const ItemType &item);
	//! push_front
	virtual void push_front(const ItemType &item);
	//! pop_back
	virtual void pop_back(ItemType &item);
	//! pop_front
	virtual void pop_front(ItemType &item);
	//! clear
	virtual void clear();
//! size
	virtual size_t size() const;
//! empty
	virtual bool empty() const;

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

template <template<class ItemType1> class ContainerType, class ItemType>
size_t SafeContainerByCond<ContainerType, ItemType>::size() const
{
	return this->mContainer.size();
}

template <template<class ItemType1> class ContainerType, class ItemType>
bool SafeContainerByCond<ContainerType, ItemType>::empty() const
{
	return this->mContainer.empty();
}


#endif //SAFECONTAINERBYCOND_H_ZMH_2006_06_16
/** @} */ //

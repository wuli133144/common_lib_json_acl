/*
 * =====================================================================================
 * 
 *        Filename:  SafeContainer.h
 * 
 *     Description:  线程同步的容器的基类.
 * 
 *         Version:  1.0
 *         Created:  2006年06月17日 10时54分27秒 CST
 *        Revision:  none
 *        Compiler:  g++
 * 
 *          Author:  张明辉 ()
 *         Company:  亿览科技
 *           Email:  minghui.zhang@yeelion.com
 * 
 * =====================================================================================
 */


#ifndef SAFECONTAINER_H_ZMH_2006_06_17
#define SAFECONTAINER_H_ZMH_2006_06_17

#include <iostream>
#include <string>

using namespace std;

template <template <class ItemType1> class ContainerType, class ItemType>
class SafeContainer
{
public :
	SafeContainer();

	virtual ~SafeContainer();

	virtual void put(const ItemType &item) = 0;

	virtual void get(ItemType &item) = 0;

	virtual void push_back(const ItemType &item) = 0;

	virtual void push_front(const ItemType &item) = 0;

	virtual void pop_back(ItemType &item) = 0;

	virtual void pop_front(ItemType &item) = 0;

	virtual void clear() = 0;

protected :
	ContainerType<ItemType>		mContainer;

private :
};


template <template <class ItemType1> class ContainerType, class ItemType>
SafeContainer<ContainerType, ItemType>::SafeContainer()
{
}

template <template <class ItemType1> class ContainerType, class ItemType>
SafeContainer<ContainerType, ItemType>::~SafeContainer()
{
}
#endif //SAFECONTAINER_H_ZMH_2006_06_17


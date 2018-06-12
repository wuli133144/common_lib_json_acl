/*
 * =====================================================================================
 * 
 *        Filename:  Deque.h
 * 
 *     Description:  
 * 
 *         Version:  1.0
 *         Created:  2006年06月17日 14时17分35秒 CST
 *        Revision:  none
 *        Compiler:  g++
 * 
 *          Author:  张明辉 ()
 *         Company:  亿览科技
 *           Email:  minghui.zhang@yeelion.com
 * 
 * =====================================================================================
 */


#ifndef DEQUE_H_ZMH_2006_06_17
#define DEQUE_H_ZMH_2006_06_17

#include <iostream>
#include <string>
#include <deque>

using namespace std;

template <class ItemType>
class Deque : public deque<ItemType>
{
public :
	Deque();

	virtual ~Deque();

	void put(const ItemType &item);

	void get(ItemType &item);

	void push_back(const ItemType &item);

	void push_front(const ItemType &item);

	void pop_back(ItemType &item);
	
	void pop_front(ItemType &item);
};

template <class ItemType>
Deque<ItemType>::Deque()
{
}

template <class ItemType>
Deque<ItemType>::~Deque()
{
}

template <class ItemType>
void Deque<ItemType>::put(const ItemType &item)
{
	//! default push_back().
	deque<ItemType>::push_back(item);

	return;
}

template <class ItemType>
void Deque<ItemType>::get(ItemType &item)
{
	item = this->back();
	deque<ItemType>::pop_back();

	return;
}

template <class ItemType>
void Deque<ItemType>::push_back(const ItemType &item)
{
	deque<ItemType>::push_back(item);

	return;
}

template <class ItemType>
void Deque<ItemType>::pop_back(ItemType &item)
{
	item = this->back();
	deque<ItemType>::pop_back();

	return;
}

template <class ItemType>
void Deque<ItemType>::push_front(const ItemType &item)
{
	deque<ItemType>::push_front(item);

	return;
}

template <class ItemType>
void Deque<ItemType>::pop_front(ItemType &item)
{
	item = this->front();
	deque<ItemType>::pop_front();

	return;
}
#endif //DEQUE_H_ZMH_2006_06_17


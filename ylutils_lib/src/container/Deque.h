/*
 * =====================================================================================
 * 
 *        Filename:  Deque.h
 * 
 *     Description:  
 * 
 *         Version:  1.0
 *         Created:  2006��06��17�� 14ʱ17��35�� CST
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

#ifndef DEQUE_H_ZMH_2006_06_17
#define DEQUE_H_ZMH_2006_06_17

#include <iostream>
#include <string>
#include <deque>

using namespace std;
/*! 
 *\brief   ͨ�ö���ģ����
 *  \class Deque Deque.h container/Deque.h    
 */
template <class ItemType>
class Deque : public deque<ItemType>
{
public :
	//! constructor
	Deque();
	//! destructor
	virtual ~Deque();
	//! push_back
	void put(const ItemType &item);
	//! get
	void get(ItemType &item);
	//! push_back
	void push_back(const ItemType &item);
	//! push_front
	void push_front(const ItemType &item);
	//!  pop_back
	void pop_back(ItemType &item);
	//! pop_front
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
	item = this->front();
	deque<ItemType>::pop_front();

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
/** @} */ //

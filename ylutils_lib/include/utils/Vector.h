/*
 * =====================================================================================
 * 
 *        Filename:  Vector.h
 * 
 *     Description:  ר����SafeVector��Vector�ӿ�
 * 
 *         Version:  1.0
 *         Created:  2006��06��17�� 14ʱ15��12�� CST
 *        Revision:  none
 *        Compiler:  g++
 * 
 *          Author:  ������ ()
 *         Company:  �����Ƽ�
 *           Email:  minghui.zhang@yeelion.com
 * 
 * =====================================================================================
 */


#ifndef VECTOR_H_ZMH_2006_06_17
#define VECTOR_H_ZMH_2006_06_17

#include <iostream>
#include <string>
#include <vector>

using namespace std;

template <class ItemType>
class Vector : public vector<ItemType>
{
public :
	Vector();

	virtual ~Vector();

	void put(const ItemType &item);

	void get(ItemType &item);

	void push_back(const ItemType &item); 

	void pop_back(ItemType &item);
	

	//! ����vector,��֧��front����. ���Բ�Ҫ����SafeVectorByCond�����front�ķ���.
	void pop_front(ItemType &item) {
		return;
	}
	//! vector not support X_front methods.
	void push_front(const ItemType &item) {
		return;
	}


};

template <class ItemType>
Vector<ItemType>::Vector()
	:
	vector<ItemType>()
{
}

template <class ItemType>
Vector<ItemType>::~Vector()
{
}

template <class ItemType>
void Vector<ItemType>::put(const ItemType &item)
{
	vector<ItemType>::push_back(item);
	return;
}

template <class ItemType>
void Vector<ItemType>::get(ItemType &item)
{
	item = this->back();
	vector<ItemType>::pop_back();
	return;
}

template <class ItemType>
void Vector<ItemType>::push_back(const ItemType &item)
{
	vector<ItemType>::push_back(item);
	return;
}

template <class ItemType>
void Vector<ItemType>::pop_back(ItemType &item)
{
	item = this->back();
	vector<ItemType>::pop_back();
	return;
}

#endif //VECTOR_H_ZMH_2006_06_17


/*
 * =====================================================================================
 * 
 *        Filename:  Vector.h
 * 
 *     Description:  专用于SafeVector的Vector接口
 * 
 *         Version:  1.0
 *         Created:  2006年06月17日 14时15分12秒 CST
 *        Revision:  none
 *        Compiler:  g++
 * 
 *          Author:  张明辉 ()
 *         Company:  亿览科技
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
	

	//! 对于vector,不支持front操作. 所以不要调用SafeVectorByCond的相关front的方法.
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


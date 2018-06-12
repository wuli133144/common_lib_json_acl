/*
 * =====================================================================================
 * 
 *        Filename:  SimpleVector.h
 * 
 *     Description:  �𽫸�vector�з�ֹ����ָ��Ľṹ. �������ǳ������ܿ���.
 * 
 *         Version:  1.0
 *         Created:  2006��06��30�� 15ʱ59��20�� CST
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

#ifndef SIMPLEVECTOR_H_ZMH_2006_06_30
#define SIMPLEVECTOR_H_ZMH_2006_06_30

#include <iostream>
#include <string>

using namespace std;
/*!
*@brief  ��������
* �𽫸�vector�з�ֹ����ָ��Ľṹ. �������ǳ������ܿ���.
 *  \class SimpleVector SimpleVector.h container/SimpleVector.h    
*/
template <class ItemType>
class SimpleVector
{
public :
	//! ����Ч�ʿ���, ��ʱ��ֻ����ռ�������г�ʼ��.
	SimpleVector(size_t size = 0);

	/*
	 * @brief    Constructor.
	 *
	 * @param    size    �趨�����Ĵ�С.
	 * @param    value   Ĭ��ֵ.
	 */
	SimpleVector(size_t size, const ItemType &value);

	//! copy Constructor.
	SimpleVector(const SimpleVector<ItemType> &other);

	//! = operator.
	SimpleVector<ItemType> &operator=(const SimpleVector<ItemType> &other);

	//! Destructor.
	virtual ~SimpleVector();

	//! ��ȡ��n��Ԫ��.
	ItemType &operator[](size_t n);

	//! ��ȡ��n��Ԫ��.
	const ItemType &operator[](size_t n) const;
	
	//! ���������Ĵ�С.
	void resize(size_t size);

	//! ���������кϷ������ݸ���.
	size_t size() const;

	//! �����ռ��С.
	size_t capacity() const;

	//! ���.
	void clear();

	//! �Ƿ�Ϊ��.
	bool empty() const;

	const ItemType *begin() const;

	const ItemType *end() const;

	ItemType *begin();

	ItemType *end();

protected :

private :
	u_int		mSize;
	u_int		mCapacity;
	ItemType	*mData;
};

template <class ItemType>
SimpleVector<ItemType>::SimpleVector(size_t size)
	:
	mSize(size),
	mCapacity(size),
	mData(NULL)
{
	if (mSize != 0)
		mData = new ItemType[mSize];
}

template <class ItemType>
SimpleVector<ItemType>::SimpleVector(size_t size, const ItemType &value)
	:
	mSize(size),
	mCapacity(size),
	mData(NULL)
{
	if (mCapacity != 0)
	{
		mData = new ItemType[mCapacity];
		for (size_t i = 0; i < mCapacity; i++)
		{
			mData[i] = value;
		} 
	}
}

template <class ItemType>
SimpleVector<ItemType>::SimpleVector(const SimpleVector<ItemType> &other)
	:
	mSize(0),
	mCapacity(0),
	mData(NULL)
{
	if (other.mCapacity != 0 && other.mData != NULL)
	{
		mData = new ItemType[other.mCapacity];
		memcpy(mData, other.mData, other.mCapacity*sizeof(ItemType));
		mSize = other.mSize;
		mCapacity = other.mCapacity;
	}
}

template <class ItemType>
SimpleVector<ItemType> &SimpleVector<ItemType>::operator=(const SimpleVector<ItemType> &other)
{
	if (this == &other)
		return *this;

	if (mData != NULL)
	{
		delete []mData;
		mData = NULL;
		mCapacity = 0;
		mSize = 0;
	}

	if (other.mCapacity != 0 && other.mData != NULL)
	{
		mData = new ItemType[other.mCapacity];
		memcpy(mData, other.mData, other.mCapacity*sizeof(ItemType));
		mSize = other.size();
		mCapacity = other.capacity();
	}

	return *this;
}

template <class ItemType>
SimpleVector<ItemType>::~SimpleVector()
{
	if (mData != NULL)
		delete []mData;
}

template <class ItemType>
inline ItemType &SimpleVector<ItemType>::operator[](size_t n)
{
	return mData[n];
}

template <class ItemType>
inline const ItemType &SimpleVector<ItemType>::operator[](size_t n) const
{
	return mData[n];
}


template <class ItemType>
void SimpleVector<ItemType>::resize(size_t n)
{
	if (n > mCapacity)
	{
		ItemType *ptmp = new ItemType[n];

		if (mSize != 0)
		{
			memcpy(ptmp, mData, sizeof(ItemType)*mCapacity);
		}
		if (mData != NULL)
			delete []mData;
		mData = ptmp;
		mCapacity = n;
	}

	mSize = n;

	return;
}

template <class ItemType>
inline size_t SimpleVector<ItemType>::size() const
{
	return mSize;
}

template <class ItemType>
inline size_t SimpleVector<ItemType>::capacity() const
{
	return mCapacity;
}

template <class ItemType>
void SimpleVector<ItemType>::clear()
{
	mSize = 0;
	mCapacity = 0;
	if (mData != NULL)
	{
		delete []mData;
		mData = NULL;
	}

	return;
}

template <class ItemType>
bool SimpleVector<ItemType>::empty() const
{
	return mSize == 0;
}

template <class ItemType>
const ItemType *SimpleVector<ItemType>::begin() const
{
	return mData;
}

template <class ItemType>
const ItemType *SimpleVector<ItemType>::end() const
{
	return mData + mSize;
}

template <class ItemType>
ItemType *SimpleVector<ItemType>::begin()
{
	return mData;
}

template <class ItemType>
ItemType *SimpleVector<ItemType>::end()
{
	return mData + mSize;
}
#endif //SIMPLEVECTOR_H_ZMH_2006_06_30
/** @} */ //

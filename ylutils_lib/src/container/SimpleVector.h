/*
 * =====================================================================================
 * 
 *        Filename:  SimpleVector.h
 * 
 *     Description:  勿将该vector中防止含有指针的结构. 这样做是出于性能考虑.
 * 
 *         Version:  1.0
 *         Created:  2006年06月30日 15时59分20秒 CST
 *        Revision:  none
 *        Compiler:  g++
 * 
 *          Author:  张明辉 ()
 *         Company:  亿览科技
 *           Email:  minghui.zhang@yeelion.com
 * 
 * =====================================================================================
 */
/** @addtogroup container
 *   容器库
 *  
 *  @{
 */

#ifndef SIMPLEVECTOR_H_ZMH_2006_06_30
#define SIMPLEVECTOR_H_ZMH_2006_06_30

#include <iostream>
#include <string>

using namespace std;
/*!
*@brief  简单容器类
* 勿将该vector中防止含有指针的结构. 这样做是出于性能考虑.
 *  \class SimpleVector SimpleVector.h container/SimpleVector.h    
*/
template <class ItemType>
class SimpleVector
{
public :
	//! 出于效率考虑, 有时侯只申请空间而不进行初始化.
	SimpleVector(size_t size = 0);

	/*
	 * @brief    Constructor.
	 *
	 * @param    size    设定容器的大小.
	 * @param    value   默认值.
	 */
	SimpleVector(size_t size, const ItemType &value);

	//! copy Constructor.
	SimpleVector(const SimpleVector<ItemType> &other);

	//! = operator.
	SimpleVector<ItemType> &operator=(const SimpleVector<ItemType> &other);

	//! Destructor.
	virtual ~SimpleVector();

	//! 获取第n个元素.
	ItemType &operator[](size_t n);

	//! 获取第n个元素.
	const ItemType &operator[](size_t n) const;
	
	//! 调整容器的大小.
	void resize(size_t size);

	//! 返回容器中合法的数据个数.
	size_t size() const;

	//! 容器空间大小.
	size_t capacity() const;

	//! 清空.
	void clear();

	//! 是否为空.
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

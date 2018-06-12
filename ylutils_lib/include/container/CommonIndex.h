/*
 * =====================================================================================
 * 
 *        Filename:  CommonIndex.h
 * 
 *     Description:  通用的索引模板.
 * 
 *         Version:  1.0
 *         Created:  2007年04月10日 20时58分43秒 CST
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

#ifndef COMMONINDEX_H_ZMH_2007_04_10
#define COMMONINDEX_H_ZMH_2007_04_10

#include <iostream>
#include <sstream>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <map>
#include <unistd.h>
#include <stdlib.h>
#include "exception/IOException.h"

using namespace std;

#define MAX_ITEM_NUM	100000000

#define u_long64 u_long

template <class ItemType>
bool loadData(const string &index, ItemType **pItems, u_long64 &item_num, u_long64 max_item_num) throw(IOException)
{
	if (*pItems != NULL)
	{
		delete []*pItems;
		item_num = 0;
	}

	ostringstream omsg;
	omsg << "loadData(" << index << ", **items, &item_num) : ";

	int fd = open(index.c_str(), O_RDONLY);
	if (fd == -1)
	{
		omsg << "open file error  : " << strerror(errno);
		goto _error;
	}

	struct stat _stat;
	fstat(fd, &_stat);

	if (read(fd, (char *)&item_num, sizeof(item_num)) != sizeof(item_num))
	{
		omsg << "read item-num error : " << strerror(errno);
		goto _error;
	}

	if (item_num > max_item_num)
	{
		omsg << "item_num " << item_num << " beyond max_item_num " << max_item_num << ": ";
		goto _error;
	}

	//!< 判断文件是否严格合法.
	if (sizeof(item_num)+item_num*sizeof(ItemType) != (size_t)_stat.st_size)
	{
		omsg << sizeof(item_num) << "+" << item_num << "*" << sizeof(ItemType) << " != " << _stat.st_size;
		goto _error;
	}

	*pItems = new ItemType[item_num];
	read(fd, (char *)*pItems, item_num*sizeof(ItemType));
	close(fd);
	return true;

_error :
	if (fd != -1)
		close(fd);
	throw IOException(omsg.str());
	return false;	//! never 2 here.
}

template <class ItemType>
void dumpData(const string &index, const ItemType *pItems, u_long64 item_num) throw(IOException)
{
	ostringstream omsg;
	omsg << "dumpData(" << index << ", **items, &item_num) : ";

	ofstream findex(index.c_str(), ios_base::binary);
	if (!findex)
	{
		omsg << "open file error  : " << strerror(errno);
		throw IOException(omsg.str());
	}

	findex.write((char *)&item_num, sizeof(item_num));
	findex.write((char *)pItems, item_num * sizeof(ItemType));
	findex.flush();
	findex.close();

	return;
}

typedef struct Index1 {
	u_int	off;
	u_int	cnt;
} T_Index1;
/*! 
 *   \brief 通用索引模板类
 *  \class CommonIndex CommonIndex.h container/CommonIndex.h    
 */
template <class Index2Type>
class CommonIndex
{
public :
	//! Constructor.
	CommonIndex(); //{}

	//! Destructor.
	virtual ~CommonIndex(); //{}

	//! 载入索引文件.
	void load(const string &index1, const string &index2) throw (IOException);
	//! 导出索引文件.
	void dump(const string &index1, const string &index2) throw (IOException);
	//! 重置索引空间大小
	void resize(u_long64 size1, u_long64 size2);
	//!  下标操作
	pair<Index2Type *, u_int> operator[](size_t n);
	//! 取值操作
	const Index2Type *get(u_int &cnt, size_t n);
	//! 取值操作
	u_int	get(Index2Type *index2, size_t n);
	
	template <class i2type>
	friend ostream &operator<<(ostream &os, CommonIndex<i2type> &index);

	//! idx2
	Index2Type *idx2() {
		return mIndex2;
	}
	//! idx1
	T_Index1 *idx1() {
		return mIndex1;
	}
	//! size1
	u_long64 size1() {
		return mSize1;
	}
	//! size2
	u_long64 size2() {
		return mSize2;
	}
protected :
	friend class IndexFilter;
private :
	T_Index1	*mIndex1;
	Index2Type	*mIndex2;
	u_long64	mSize1;
	u_long64	mSize2;
};

template <class i2type>
ostream &operator<<(ostream &os, CommonIndex<i2type> &index)
{
	os << "索引信息 : 一级:" << index.mSize1 << "\t二级:" << index.mSize2 << endl;
	for (u_long64 i = 0; i < index.mSize1; i++)
	{
		i2type *pos = index.mIndex2 + index.mIndex1[i].off;
		if (index.mIndex1[i].cnt > 0)
		{
			os << "<" << i << " : ";
			for (u_int j = 0; j < index.mIndex1[i].cnt; j++)
				os << pos[j] << " ";
			os << ">\n";
		}
	}
	return os;
}

template <class Index2Type>
CommonIndex<Index2Type>::CommonIndex()
	:
	mIndex1(NULL),
	mIndex2(NULL),
	mSize1(0),
	mSize2(0)
{
}

template <class Index2Type>
CommonIndex<Index2Type>::~CommonIndex()
{
	if (mIndex1 != NULL)
		delete []mIndex1;
	if (mIndex2 != NULL)
		delete []mIndex2;
}

template <class Index2Type>
void CommonIndex<Index2Type>::load(const string &index1, const string &index2) throw (IOException)
{
	loadData(index1, &mIndex1, mSize1, MAX_ITEM_NUM);
	loadData(index2, &mIndex2, mSize2, MAX_ITEM_NUM);
	return;
}

template <class Index2Type>
void CommonIndex<Index2Type>::dump(const string &index1, const string &index2) throw (IOException)
{
	dumpData(index1, mIndex1, mSize1);
	dumpData(index2, mIndex2, mSize2);
	return;
}

template <class Index2Type>
pair<Index2Type *, u_int> CommonIndex<Index2Type>::operator[](size_t n)
{
	if (n >= mSize1)
		return make_pair(NULL, 0);

	return make_pair(mIndex2 + mIndex1[n].off, mIndex1[n].cnt);
}

template <class Index2Type>
const Index2Type *CommonIndex<Index2Type>::get(u_int &cnt, size_t n)
{
	if (n >= mSize1)
		return NULL;
	cnt = mIndex1[n].cnt;
	return mIndex2 + mIndex1[n].off;
}

template <class Index2Type>
u_int	CommonIndex<Index2Type>::get(Index2Type *index2, size_t n)
{
	if (n >= mSize1)
	{
		index2 = NULL;
		return 0;
	}

	index2 = mIndex2 + mIndex1[n].off;
	return mIndex1[n].cnt;
}

template <class Index2Type>
void CommonIndex<Index2Type>::resize(u_long64 size1, u_long64 size2)
{
	if (mIndex1 != NULL)
		delete []mIndex1;
	mIndex1 = NULL;
	mSize1 = 0;
	if (size1 > 0)
	{
		mIndex1 = new T_Index1[size1];
		mSize1 = size1;
	}

	if (mIndex2 != NULL)
		delete []mIndex2;
	mIndex2 = NULL;	
	mSize2 = 0;
	if (size2 > 0)
	{
		mIndex2 = new Index2Type[size2];
		mSize2 = size2;
	}

	return;
}
#endif //COMMONINDEX_H_ZMH_2007_04_10
/** @} */ //

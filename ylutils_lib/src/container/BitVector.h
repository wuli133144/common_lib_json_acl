/*
 * =====================================================================================
 * 
 *        Filename:  BitVector.h
 * 
 *     Description:  
 * 
 *         Version:  1.0
 *         Created:  2007年04月05日 18时42分24秒 CST
 *        Revision:  none
 *        Compiler:  g++
 * 
 *          Author:  张明辉 ()
 *         Company:  亿览科技
 *           Email:  minghui.zhang@yeelion.com
 * 
 * =====================================================================================
 */
/** @defgroup container
 *   容器库
 *  
 *  @{
 */

#ifndef BITVECTOR_H_ZMH_2007_04_05
#define BITVECTOR_H_ZMH_2007_04_05

#include <iostream>
#include <fstream>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include "exception/IOException.h"

using namespace std;
/*! \brief  位容器
\code
#include "BitVector.h"


int main()
{
	BitVector v(100000000);
	cout << "v.size() : " << v.size() << endl;

	for (unsigned i = 0; i < v.size(); i++)
	{
		if (v[i])
			cout << "bad" << endl;
	}

	int i, value;
	cout << "bit vector > ";
	while (cin >> i >> value) 
	{
		if (i >= (int)v.size())
		{
			cout << "out of range " << endl << "bit vector > ";
			continue;
		}
		cout << "cur val : " << v[i] << endl;
		v.set(value, i);
		cout << "set val : " << v[i] << endl;
		cout << "bit vector > ";
	}

	v.dump("hello.bit");

	cout << endl;
	return 0;
}

\endcode
 */
class BitVector
{
public :
	//! Constructor.
	BitVector(unsigned size = 0) {
		mSize = size;
		//mCapacity = size;
		mBuffer = NULL;
		if (mSize != 0)
		{
			mBuffer = new unsigned char[size/8+1];
		}
	}
	//! 装载文件
	void load(const string &file) throw(IOException) {
		if (mBuffer != NULL)
			delete []mBuffer;
		mBuffer = NULL;

		int fd = open(file.c_str(), O_RDONLY);
		if (fd < 0)
			throw IOException(string("BitVector::load(\"") + file + "\") : " + strerror(errno));
		if (read(fd, (char *)&mSize, sizeof(mSize)) != sizeof(mSize))
			throw IOException(string("BitVector::load(\"") + file + "\") : format error when read size");

		mBuffer = new unsigned char[mSize/8 + 1];
		if (read(fd, mBuffer, mSize/8+1) != (mSize/8+1))
			throw IOException(string("BitVector::load(\"") + file + "\") : format error when read content");
		close(fd);

		return;
	}
	//! 输出到文件
	void dump(const string &file) throw(IOException) {
		ofstream fout(file.c_str(), ios_base::binary);
		if (!fout)
			throw IOException(string("BitVector::dump(\"") + file + "\") : " + strerror(errno));
		fout.write((char *)&mSize, sizeof(mSize));
		fout.write((const char *)mBuffer, mSize/8+1);
		fout.close();
		return;
	}
	//! 重置容器空间
	void resize(unsigned size) {
		mSize = size;
		if (mSize != 0)
		{
			mBuffer = new unsigned char[size/8+1];
		}
	}
	//! destructor
	virtual ~BitVector() {
		if (mBuffer != NULL)
			delete []mBuffer;
	}
	//! 返回容器容量
	size_t size() {
		return mSize;
	}
	//! 取某位bit
	bool get(unsigned n) {
		return mBuffer[n>>3] & (128>>(n&07)) ? 1:0;
	}
	//! 设置某位bit
	void set(bool val, unsigned n) {
		if (val)
		{
			mBuffer[n/8] |= (128>>n%8);
		}
		else
		{
			register unsigned i = mBuffer[n/8];
			register unsigned m = n%8;
			mBuffer[n/8] = ((127>>m)&i) | (i>>(8-m))<<(8-m);
		}
		return;
	}
	//! 取某位bit
	bool operator[](unsigned n) {
		return get(n);
	}

protected :

private :
	unsigned char	*mBuffer;
	unsigned  mSize;
	//unsigned  mCapacity;
};
#endif //BITVECTOR_H_ZMH_2007_04_05
/** @} */ //

/*
 * =====================================================================================
 * 
 *        Filename:  BitVector.cpp
 * 
 *     Description:  
 * 
 *         Version:  1.0
 *         Created:  2007年04月05日 19时22分36秒 CST
 *        Revision:  none
 *        Compiler:  g++
 * 
 *          Author:  张明辉 (), minghui.zhang@yeelion.com
 *         Company:  亿览科技
 * 
 * =====================================================================================
 */

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

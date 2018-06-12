/*
 * =====================================================================================
 * 
 *        Filename:  big5.cpp
 * 
 *     Description:  
 * 
 *         Version:  1.0
 *         Created:  2006年12月27日 15时03分26秒 CST
 *        Revision:  none
 *        Compiler:  g++
 * 
 *          Author:  张明辉 (), minghui.zhang@yeelion.com
 *         Company:  亿览科技
 * 
 * =====================================================================================
 */

#include <iostream>
#include <fstream>
#include <string>

using namespace std;
size_t a;

int main()
{
	ofstream fbig5("big5.code");
	for (int i = 0xA1; i <= 0xF9; i++)
	{
		for (int j = 0x40; j <= 0x7E; j++)
		{
			fbig5 << (unsigned char)i << (unsigned char)j << endl;
		}

		for (int j = 0xA1; j <= 0xFE; j++)
		{
			fbig5 << (unsigned char)i << (unsigned char)j << endl;
		}
	}
	fbig5.close();

	return 0;
}




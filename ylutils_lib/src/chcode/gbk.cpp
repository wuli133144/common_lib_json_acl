/*
 * =====================================================================================
 * 
 *        Filename:  gbk.cpp
 * 
 *     Description:  
 * 
 *         Version:  1.0
 *         Created:  2006年12月26日 18时26分41秒 CST
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

int main()
{
	ofstream fgbk("gbk.code");
	for (int i = 0x81; i <= 0xFE; i++)
	{
		for (int j = 0x40; j <= 0x7E; j++)
		{
			fgbk << (unsigned char)i << (unsigned char)j << endl;
		}

		for (int j = 0x80; j <= 0xFE; j++)
		{
			fgbk << (unsigned char)i << (unsigned char)j << endl;
		}
	}
	fgbk.close();

	return 0;
}

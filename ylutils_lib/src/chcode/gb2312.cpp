/*
 * =====================================================================================
 * 
 *        Filename:  gb2312.cpp
 * 
 *     Description:  
 * 
 *         Version:  1.0
 *         Created:  2006年12月27日 13时11分14秒 CST
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
	ofstream fgb2312("gb2312.code");
	for (int i = 0xA1; i <= 0xF7; i++)
	{
		for (int j = 0xA1; j <= 0xFE; j++)
		{
			fgb2312 << (unsigned char)i << (unsigned char)j << endl;
		}
	}
	fgb2312.close();

	return 0;
}

/*
 * =====================================================================================
 * 
 *        Filename:  big5.cpp
 * 
 *     Description:  
 * 
 *         Version:  1.0
 *         Created:  2006��12��27�� 15ʱ03��26�� CST
 *        Revision:  none
 *        Compiler:  g++
 * 
 *          Author:  ������ (), minghui.zhang@yeelion.com
 *         Company:  �����Ƽ�
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




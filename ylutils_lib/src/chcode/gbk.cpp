/*
 * =====================================================================================
 * 
 *        Filename:  gbk.cpp
 * 
 *     Description:  
 * 
 *         Version:  1.0
 *         Created:  2006��12��26�� 18ʱ26��41�� CST
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

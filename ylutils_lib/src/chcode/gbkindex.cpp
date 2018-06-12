/*
 * =====================================================================================
 * 
 *        Filename:  gbkindex.cpp
 * 
 *     Description:  
 * 
 *         Version:  1.0
 *         Created:  2007年01月04日 10时49分27秒 CST
 *        Revision:  none
 *        Compiler:  g++
 * 
 *          Author:  张明辉 (), minghui.zhang@yeelion.com
 *         Company:  亿览科技
 * 
 * =====================================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

#define INDEXLEN 0XFFFF

string no = "□";

int main()
{
	ifstream fgbk("gbk.code");
	ifstream fgb2312("gbk2gb.code.txt");

	u_short *index = new u_short[INDEXLEN];
	string gbk, gb2312;
	while (getline(fgbk, gbk) && getline(fgb2312, gb2312))
	{
		if (gb2312 == no)
			continue;

		index[*(u_short *)gbk.c_str()] = *(u_short *)gb2312.c_str();
	}

	fgbk.close();
	fgb2312.close();

	ofstream fgbkgbh("gbkgb.h");
	char buf[12];
	for (size_t i = 0; i < INDEXLEN; i++)
	{
		sprintf(buf, "\t0X%04x,\n", index[i]);
		fgbkgbh << buf;
	}
	delete []index;
	fgbkgbh.close();

	return 0;
}

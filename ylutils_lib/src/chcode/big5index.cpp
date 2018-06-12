/*
 * =====================================================================================
 * 
 *        Filename:  big5index.cpp
 * 
 *     Description:  
 * 
 *         Version:  1.0
 *         Created:  2006年12月27日 18时31分50秒 CST
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
	ifstream fbig5("big5.code");
	ifstream fgb2312("gbkgb.code");

	u_short *index = new u_short[INDEXLEN];
	string big5, gb2312;
	while (getline(fbig5, big5) && getline(fgb2312, gb2312))
	{
		if (gb2312 == no)
			continue;

		index[*(u_short *)big5.c_str()] = *(u_short *)gb2312.c_str();
	}

	fbig5.close();
	fgb2312.close();

	ofstream fbig5gbh("big5gb.h");
	char buf[12];
	for (size_t i = 0; i < INDEXLEN; i++)
	{
		sprintf(buf, "\t0X%04x,\n", index[i]);
		fbig5gbh << buf;
	}
	delete []index;
	fbig5gbh.close();

	return 0;
}


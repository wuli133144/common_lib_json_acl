/*
 * =====================================================================================
 * 
 *        Filename:  conv.cpp
 * 
 *     Description:  
 * 
 *         Version:  1.0
 *         Created:  2006年07月14日 17时04分34秒 CST
 *        Revision:  none
 *        Compiler:  g++
 * 
 *          Author:  张明辉 (), minghui.zhang@yeelion.com
 *         Company:  亿览科技
 * 
 * =====================================================================================
 */

#include "conv.h"
#include <errno.h>

CodeConv::CodeConv(const char *to, const char *from)
{
	mCd = iconv_open(to, from);
	if (mCd == (iconv_t)-1)
	{
		cout << "Error : " << strerror(errno) << endl; 
		exit(1);
	}
}

CodeConv::~CodeConv()
{
	if (mCd != (iconv_t)-1)
	{
		iconv_close(mCd);
	}
}

size_t CodeConv::conv(char *out, size_t outsize, char *input, size_t inputsize)
{
	if (mCd == (iconv_t)-1)
		return -1;

	char *_out = out;
	char *_input = input;
	size_t outleft = outsize;
	size_t inleft = inputsize;
	size_t ret = iconv(mCd, &_input, &inleft, &_out, &outleft);

	return outsize-outleft;
}


#ifdef _TEST

#include <fstream>

int main()
{
	CodeConv conv("gb2312", "big5");

	char in[3];
	char out[3];
	in[2] = out[2] = '\0';

	ifstream fin("big5.code");
	ofstream fout("big5gb.code");

	string line;
	while (getline(fin, line))
	{
		if (line.length() != 2)
		{
			cout << "error format" << endl;
			return 0;
		}
		sprintf(in, "%s", line.c_str());
		size_t ret = conv.conv(out, 2, in, 2);
		if (ret == (size_t)-1)
		switch (errno)
		{
		case E2BIG :
			cout << "E2BIG" << endl;
			break;
		case EILSEQ :
			cout << "EILSEQ" << endl;
			break;
		case EINVAL :
			cout << "EINVAL" << endl;
			break;
		}

		if (ret != 2)
		{
			fout << in << "\t0"<< endl;
		}
		else
		{
			fout << out << "\t1" << endl;
		}
	}

	fin.close();
	fout.close();

	return 0;
}
#endif //_TEST

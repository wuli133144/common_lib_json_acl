/*
 * =====================================================================================
 * 
 *        Filename:  ChCode.cpp
 * 
 *     Description:  
 * 
 *         Version:  1.0
 *         Created:  2006年12月28日 11时35分17秒 CST
 *        Revision:  none
 *        Compiler:  g++
 * 
 *          Author:  张明辉 (), minghui.zhang@yeelion.com
 *         Company:  亿览科技
 * 
 * =====================================================================================
 */

#include "ChCode.h"
#include <fstream>

u_short ChCode::mBig5GB2312[CODEMAPLEN] = 
{
#include "big5gb.h"
};

u_short ChCode::mBig5GBK[CODEMAPLEN] =
{
//#include "big5gbk.h"
};

u_short ChCode::mGBKGB2312[CODEMAPLEN] =
{
#include "gbkgb.h"
};

ChCode::ChCode(E_CodeType to, E_CodeType from)
	:
	mTo(to),
	mFrom(from)
{
}

ChCode::~ChCode()
{
}

/**
 * 根据gb常用字和big5常用字的数目来判断.
 */
E_CodeType ChCode::codeDetect(const char *src)
{
	const u_char *p = (const u_char *)src;
	int big5num = 0;
	int gbknum = 0;
	while (*p != '\0')
	{
		if (isCommonBig5(*p, *(p+1)))
			big5num++;
		if (isCommonGBK(*p, *(p+1)))
			gbknum++;
		if (isGBK(*p, *(p+1)) || isBig5(*p, *(p+1)))
			p++;
		p++;
	}

	cout << "big5num : " << big5num << endl;
	cout << "gbknum : " << gbknum << endl;
	//if (big5num*2 > (const char *)p-src) return E_BIG5;
	if (big5num == gbknum && gbknum == 0) return E_GBK;
	if (big5num >= gbknum) return E_BIG5;

	return E_GBK;
}

void ChCode::set(E_CodeType to, E_CodeType from)
{
	mTo = to;
	mFrom = from;
	return;
}

int ChCode::conv(char *to, int tolen, const char *from, int fromlen)
{
	return 0;
}

char *ChCode::big52gb(char *buf)
{
	if (buf == NULL) return buf;
	char *p = buf;
	while (*p != '\0')
	{
		if (isBig5(*p, *(p+1)))
		{	//! big5.
			if (mBig5GB2312[*(u_short *)p] != 0)
				*(u_short *)p = mBig5GB2312[*(u_short *)p];
			p += 2;
		}
		else
		{	//! ascii.
			p += 1;
		}
	}

	return buf;
}

string &ChCode::big52gb(string &buf)
{
	this->big52gb((char *)&buf[0]);
	return buf;
}

char *ChCode::gbk2gb(char *buf)
{
	if (buf == NULL) return buf;
	char *p = buf;
	while (*p != '\0')
	{
		if (isGBK(*p, *(p+1)))
		{	//! big5.
			if (mGBKGB2312[*(u_short *)p] != 0)
				*(u_short *)p = mGBKGB2312[*(u_short *)p];
			p += 2;
		}
		else
		{	//! ascii.
			p += 1;
		}
	}

	return buf;
}

string &ChCode::gbk2gb(string &buf)
{
	this->gbk2gb((char *)&buf[0]);
	return buf;
}
//! \code
#ifdef _TEST

int main(int argc, char *argv[])
{
	char buf[1024];
	bzero(buf, sizeof(buf));
	ifstream f(argc==2?argv[1]:"test.txt");
	ChCode code(E_GB2312, E_BIG5);

	string line;
	while (getline(f, line))
	{
		sprintf(buf, "%s", line.c_str());
		switch (code.codeDetect(buf))
		{
			case E_BIG5 :
				cout << "BIG5 CODE" << endl;
				cout << "RAW : " << buf << endl;
				code.big52gb(buf);
				cout << "NOW : " << buf << endl;
				break;
			case E_GBK :
				cout << "GBK CODE" << endl;
				cout << "RAW : " << buf << endl;
				code.gbk2gb(buf);
				cout << "NOW : " << buf << endl;
			//sprintf(buf, "%s", line.c_str());
			//code.big52gb(buf);
			//cout << "NOW : " << buf << endl;
				break;
			default :
				break;
		}
		cout << endl;
	}

	f.close();

	return 0;
}
#endif //_TEST
//! \endcode
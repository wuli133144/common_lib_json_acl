/*
 * =====================================================================================
 * 
 *        Filename:  conv.h
 * 
 *     Description:  编码转换器.
 * 
 *         Version:  1.0
 *         Created:  2006年07月14日 16时51分56秒 CST
 *        Revision:  none
 *        Compiler:  g++
 * 
 *          Author:  张明辉 ()
 *         Company:  亿览科技
 *           Email:  minghui.zhang@yeelion.com
 * 
 * =====================================================================================
 */
 /*!
 * \file conv.h
 * \brief 编码转换器.
 *
 * 编码转换器.
 * \author  张明辉
 * \date 2006年07月14日 16时51分56秒 CST
 */
 
/** @addtogroup chcode
 *  
 *  编码转换
 *  @{
 */



#ifndef CONV_H_ZMH_2006_07_14
#define CONV_H_ZMH_2006_07_14

#include <iostream>
#include <string>
#include <iconv.h>

using namespace std;
/*! \brief 编码转换类
 *  封装iconv函数，进行编码转换
 *! \class CodeConv conv.h chcode/conv.h 
 *  \version 1.0    
 \code
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
 \endcode
 */
class CodeConv
{
public :
	//! Construcor.
	CodeConv(const char *to, const char *from);

	//! Destructor.
	virtual ~CodeConv();

	/**
	 * @brief    转换编码.
	 *
	 * @param    out        目的编码容器.
	 * @param    outsize    容器大小.
	 * @param    input      源编码容器.
	 * @param    inputsize  源编码串长度.
	 * @return   <0 表示失败. 其他转换后的长度.
	 */
	size_t conv(char *out, size_t outsize, char *input, size_t inputsize);

protected :

private :
	iconv_t		mCd;
};
#endif //CONV_H_ZMH_2006_07_14
/** @} */ // end of Chcode

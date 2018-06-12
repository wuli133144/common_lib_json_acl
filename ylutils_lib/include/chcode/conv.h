/*
 * =====================================================================================
 * 
 *        Filename:  conv.h
 * 
 *     Description:  ����ת����.
 * 
 *         Version:  1.0
 *         Created:  2006��07��14�� 16ʱ51��56�� CST
 *        Revision:  none
 *        Compiler:  g++
 * 
 *          Author:  ������ ()
 *         Company:  �����Ƽ�
 *           Email:  minghui.zhang@yeelion.com
 * 
 * =====================================================================================
 */
 /*!
 * \file conv.h
 * \brief ����ת����.
 *
 * ����ת����.
 * \author  ������
 * \date 2006��07��14�� 16ʱ51��56�� CST
 */
 
/** @addtogroup chcode
 *  
 *  ����ת��
 *  @{
 */



#ifndef CONV_H_ZMH_2006_07_14
#define CONV_H_ZMH_2006_07_14

#include <iostream>
#include <string>
#include <iconv.h>

using namespace std;
/*! \brief ����ת����
 *  ��װiconv���������б���ת��
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
	 * @brief    ת������.
	 *
	 * @param    out        Ŀ�ı�������.
	 * @param    outsize    ������С.
	 * @param    input      Դ��������.
	 * @param    inputsize  Դ���봮����.
	 * @return   <0 ��ʾʧ��. ����ת����ĳ���.
	 */
	size_t conv(char *out, size_t outsize, char *input, size_t inputsize);

protected :

private :
	iconv_t		mCd;
};
#endif //CONV_H_ZMH_2006_07_14
/** @} */ // end of Chcode

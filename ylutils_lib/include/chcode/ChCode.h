/*
 * =====================================================================================
 * 
 *        Filename:  ChCode.h
 * 
 *     Description:  ����ת����.  ��֧��big5==>gb2312   gbk(��)==>gb2312   utf-8==>gb.
 *                   ����������̽��.
 * 
 *         Version:  1.0
 *         Created:  2006��12��28�� 10ʱ59��01�� CST
 *        Revision:  none
 *        Compiler:  g++
 * 
 *          Author:  ������ ()
 *         Company:  �����Ƽ�
 *           Email:  minghui.zhang@yeelion.com
 * 
 * =====================================================================================
 */
/** @defgroup chcode
 *  �ַ���ת�롢����ת��
 *  @{
 */


#ifndef CHCODE_H_ZMH_2006_12_28
#define CHCODE_H_ZMH_2006_12_28

#include <iostream>
#include <string>

using namespace std;

#define CODEMAPLEN 0XFFFF		//!< ���ֱ���Ϊ˫�ֽ�, ���ֵΪ0XFFFF.

//! ����ö������
//!\enum E_CodeType ChCode.h chcode/ChCode.h
enum E_CodeType 
{
	E_UTF8 = 0,  /*!< E_UTF8 0 utf-8. */
	E_GB2312 = 1, /*!< E_GB2312 1 gb2312. */
	E_GBK = 2,    /*!< E_GBK 2 gbk. */
	E_BIG5 = 3,  /*!< E_BIG5 3 big5. */
	E_UNKOWN      /*!< E_UNKOWN 4 other. */
};
/*! \brief �ַ���zת��.
 *! \class ChCode ChCode.h chcode/ChCode.h
 *  ֧��big5==>gb2312   gbk(��)==>gb2312   ����������̽��.
 *  \version 1.0     
  *!  \code
 
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
\endcode   
 */

class ChCode
{
public :
	ChCode() {
	}
	//! Constructor.
	ChCode(E_CodeType to, E_CodeType from);
	
	//! Destructor.
	virtual ~ChCode();

	/**
	 * @brief    �����ַ�����ת��.
	 * 
	 * @param    to        Ŀ�����
	 * @param    from      Դ����.
	 * @return   void
	 */
	void set(E_CodeType to, E_CodeType from);

	//! ̽�⻺�����еı�������.
	E_CodeType codeDetect(const char *src);
	/**
	 * @brief    �����������ַ���ת��.
	 * 
	 * @param    to        Ŀ�껺����.
	 * @param    tolen     Ŀ�껺��������.
	 * @param    from      Դ�ַ�������.
	 * @param    fromlen   from ����.
	 * @return   Ŀ���ַ����ĳ���.
	 * @retval   -1        ��ʾ������Խ��. to�����̫��.
	 * @retval   >=0       �ɹ�ת����ĳ���.
	 */
	int conv(char *to, int tolen, const char *from, int fromlen);
	/**
	 * @brief    �����������ַ���ת��. big5 to gb
	 * 
	 * @param    buf       Դ�ַ�������.
	 * @return   Ŀ���ַ���
	 */
	char *big52gb(char *buf);
	/**
	 * @brief    �����������ַ���ת��. big5 to gb
	 * 
	 * @param    buf       Դ�ַ�������.
	 * @return   Ŀ���ַ���
	 */
	string &big52gb(string &buf);
	/**
	 * @brief    �����������ַ���ת��. gbk to gb
	 * 
	 * @param    buf       Դ�ַ�������.
	 * @return   Ŀ���ַ���
	 */
	char *gbk2gb(char *buf);
	/**
	 * @brief    �����������ַ���ת��. gbk to gb
	 * 
	 * @param    buf       Դ�ַ�������.
	 * @return   Ŀ���ַ���
	 */
	string &gbk2gb(string &buf);
	
protected :

private :
	bool isBig5(u_char c1, u_char c2) {
		return c1>=0xA1&&c1<=0xF9&&((c2>=0x40&&c2<=0x7E)||(c2>=0xA1&&c2<=0xFE));
	}

	bool isCommonBig5(u_char c1, u_char c2) {
		return (c1>=0xA4&&c1<=0xC5&&c2>=0x40&&c2<=0xFE) || (c1==0xC6&&c2>=0x40&&c2<=0x7E);
	}

	// gbk��gb2312�Ǽ��ݵ�, gb2312����λ��, ���ĳ�������16-55��.
	bool isCommonGBK(u_char c1, u_char c2) {
		return c1>=0xb0&&c1<=0xd7&&c2>=0xa1&&c2<=0xfe;
	}

	bool isGBK(u_char c1, u_char c2) {
		return c1>=0x81&&c1<=0xFE&&c2>=0x40&&c2<=0xFE&&c2!=0x7F;
	}

	bool isGB2312(u_char c1, u_char c2);
public :
	E_CodeType	mTo;
	E_CodeType	mFrom;

	static u_short		mBig5GB2312[CODEMAPLEN];
	static u_short		mBig5GBK[CODEMAPLEN];
	static u_short		mGBKGB2312[CODEMAPLEN];
	// UTF-8���ڱ䳤����. ����ת����Ҫ�ñ�ķ���.
};
#endif //CHCODE_H_ZMH_2006_12_28
/** @} */ // end of Chcode

/*
 * =====================================================================================
 * 
 *        Filename:  ChCode.h
 * 
 *     Description:  中文转码器.  将支持big5==>gb2312   gbk(繁)==>gb2312   utf-8==>gb.
 *                   并包括编码探测.
 * 
 *         Version:  1.0
 *         Created:  2006年12月28日 10时59分01秒 CST
 *        Revision:  none
 *        Compiler:  g++
 * 
 *          Author:  张明辉 ()
 *         Company:  亿览科技
 *           Email:  minghui.zhang@yeelion.com
 * 
 * =====================================================================================
 */
/** @defgroup chcode
 *  字符串转码、中文转码
 *  @{
 */


#ifndef CHCODE_H_ZMH_2006_12_28
#define CHCODE_H_ZMH_2006_12_28

#include <iostream>
#include <string>

using namespace std;

#define CODEMAPLEN 0XFFFF		//!< 汉字编码为双字节, 最大值为0XFFFF.

//! 编码枚举类型
//!\enum E_CodeType ChCode.h chcode/ChCode.h
enum E_CodeType 
{
	E_UTF8 = 0,  /*!< E_UTF8 0 utf-8. */
	E_GB2312 = 1, /*!< E_GB2312 1 gb2312. */
	E_GBK = 2,    /*!< E_GBK 2 gbk. */
	E_BIG5 = 3,  /*!< E_BIG5 3 big5. */
	E_UNKOWN      /*!< E_UNKOWN 4 other. */
};
/*! \brief 字符串z转码.
 *! \class ChCode ChCode.h chcode/ChCode.h
 *  支持big5==>gb2312   gbk(繁)==>gb2312   并包括编码探测.
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
	 * @brief    设置字符编码转向.
	 * 
	 * @param    to        目标编码
	 * @param    from      源编码.
	 * @return   void
	 */
	void set(E_CodeType to, E_CodeType from);

	//! 探测缓冲区中的编码类型.
	E_CodeType codeDetect(const char *src);
	/**
	 * @brief    将缓冲区的字符流转码.
	 * 
	 * @param    to        目标缓冲区.
	 * @param    tolen     目标缓冲区长度.
	 * @param    from      源字符缓冲区.
	 * @param    fromlen   from 长度.
	 * @return   目标字符流的长度.
	 * @retval   -1        表示缓冲区越界. to定义的太短.
	 * @retval   >=0       成功转换后的长度.
	 */
	int conv(char *to, int tolen, const char *from, int fromlen);
	/**
	 * @brief    将缓冲区的字符流转码. big5 to gb
	 * 
	 * @param    buf       源字符缓冲区.
	 * @return   目标字符流
	 */
	char *big52gb(char *buf);
	/**
	 * @brief    将缓冲区的字符流转码. big5 to gb
	 * 
	 * @param    buf       源字符缓冲区.
	 * @return   目标字符串
	 */
	string &big52gb(string &buf);
	/**
	 * @brief    将缓冲区的字符流转码. gbk to gb
	 * 
	 * @param    buf       源字符缓冲区.
	 * @return   目标字符流
	 */
	char *gbk2gb(char *buf);
	/**
	 * @brief    将缓冲区的字符流转码. gbk to gb
	 * 
	 * @param    buf       源字符缓冲区.
	 * @return   目标字符串
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

	// gbk与gb2312是兼容的, gb2312是区位码, 它的常用字在16-55区.
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
	// UTF-8属于变长编码. 它的转码需要用别的方法.
};
#endif //CHCODE_H_ZMH_2006_12_28
/** @} */ // end of Chcode

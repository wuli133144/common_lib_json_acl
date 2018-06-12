/*
 * =====================================================================================
 * 
 *        Filename:  sign.h
 * 
 *     Description:  封装leim的签名算法.
 * 
 *         Version:  1.0
 *         Created:  2006年05月25日 15时03分36秒 CST
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
 * \file sign.h
 * \brief 封装leim的签名算法.
 *
 * 封装leim的签名算法.
 * \author  张明辉
 * \date 2006年05月25日 15时03分36秒 CST
 */
 
 /** @defgroup sign
 *  签名算法
 *  @{
 */


#ifndef SIGN_H_ZMH_2006_05_25
#define SIGN_H_ZMH_2006_05_25

#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <openssl/md5.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include "ul_prime.h"
//! T_Sign
typedef struct
{ 
	//! sign1
	u_int sign1;
	//! sign2
	u_int sign2;
} T_Sign;
#define MD5_SIZE 33
/*! \brief 封装leim的签名算法.
 *! \class YeeSign sign.h sign/sign.h        
  *  \version 1.0 
  \code
  int main()
{
	T_Sign sign;
	u_int sign1, sign2;
	
	cout << "\033[32;1m\t\t   NEW\t\t\t\t   OLD\t\t\tequal\033[m" << endl;

	for (size_t i = 0; i < ArraySize(tests); i++)
	{
		cout << "\033[33;1m" << tests[i].src << "\033[m" << endl;

		//! new sign fun.
		YeeSign::getSign(sign, tests[i].src);

		//! old sign fun.
		creat_sign_f64(tests[i].src, strlen(tests[i].src), &sign1, &sign2);

		cout << "sign1 :\t\t" << sign.sign1 << ((sign1<10000000)?"\t":"") << "\t\t\t" << sign1 
			<< ((sign1<10000000)?"\t":"") << "\t\t\033[34;1m "
			<< ((sign.sign1==sign1)?"YES":"NO") << "\033[m" << endl;

		cout << "sign2 :\t\t" << sign.sign2 << ((sign2<10000000)?"\t":"") << "\t\t\t" << sign2 
			<< ((sign2<10000000)?"\t":"") << "\t\t\033[34;1m "
			<< ((sign.sign2==sign2)?"YES":"NO") << "\033[m" << endl << endl;
	}

	return 0;
}
  \endcode
 */
class YeeSign{
public:
	//! constructor
	YeeSign();
	//! decstructor
	virtual ~YeeSign();
/**
 * @brief    将指定的内存空间的数据做成32位无符号整型的数.
 *           通过算法1做的签名故命名为getSign1.
 *
 * @param    src    要做签名的串.
 * @param    len    src中要做签名的字节数. 如果len==-1则算法将对
 *           strlen(src)的字节做签名.
 * @return   返回str的整型表示的签名.
 */
static u_int getSign1(const char *src, int len = -1);
 
/**
 * @brief    将指定的内存空间的数据做成32位无符号整型的数.
 *           通过算法2做的签名故命名为getSign2.
 *
 * @param    src    要做签名的串.
 * @param    len    src中要做签名的字节数. 如果len==-1则算法将对
 *           strlen(src)的字节做签名.
 * @return   返回str的整型表示的签名.
 */
static u_int getSign2(const char *src, int len = -1);
 
/**
 * @brief    将指定的内存空间的数据做成两个32位无符号整数表示的坐标.
 *
 * @param    sign1  第一个签名.
 * @param    sign2  第二个签名.
 * @param    src    要做签名的串.
 * @param    len    src中要做签名的字节数. 如果len==-1则算法将对
 *           strlen(src)的字节做签名.
 */
static void getSign(u_int &sign1, u_int &sign2, const char *src, int len = -1);
 
/**
 * @brief    将指定的内存空间的数据做成两个32位无符号整数表示的坐标.
 *
 * @param    sign   签名坐标.
 * @param    src    要做签名的串.
 * @param    len    src中要做签名的字节数. 如果len==-1则算法将对
 *           strlen(src)的字节做签名.
 */
static void getSign(T_Sign &sign, const char *src, int len = -1);
 /**
 * @brief    将指定的内存空间的数据做成两个32位无符号整数表示的坐标.
 *
 * @param    sign   签名坐标.
 * @param    src    要做签名的串.
 * @param    len    src中要做签名的字节数. 如果len==-1则算法将对
 *           strlen(src)的字节做签名.
 */
static T_Sign *getSign(T_Sign *sign, const char *src, int len = -1);

/*!
 *@brief generate file's sig value 
 *@param  path   文件路径
 *@param  nsig1   sig签名1
 *@param  nsig2   sig签名2 
 *@return  boolean 是否成功
 */
static bool getNewsig(const char* path, unsigned int* nsig1, unsigned int* nsig2);
/*!
 *@brief generate file's sig value 
 *@param  path   文件路径
 *@param  sig1   sig签名1
 *@param  sig2   sig签名2 
 *@return  boolean 是否成功
 */
static bool getOldsig(const char* path, unsigned int* sig1, unsigned int* sig2);
	/**
	 * @brief   计算字符串64位签名
	 * \author  leiming
	 * \version 1.0       
	 * @param   psrc  待计算签名的字符串
	 * @param  slen   psrc的长度
	 * @param   sign1   结果签名高32位
	 * @param   sign2   结果签名低32位
	 * @return  1
	 */  
static int creat_sign_f64 (char* psrc, int slen, u_int* sign1 , u_int* sign2);

	/**
	 * @brief   计算字符串64位签名
	 * \author  leiming	 
	 * \version 1.0       
	 * @param   psrc  待计算签名的字符串
	 * @param  slen   psrc的长度
	 * @param   sign1   结果签名高32位
	 * @param   sign2   结果签名低32位
	 * @return  1
	 */  
static int creat_sign_fs64 (char* psrc, int slen, u_int* sign1 , u_int* sign2);


private:
	static void reverse_byte_order(unsigned char* buf);
};
#endif //SIGN_H_ZMH_2006_05_25
/** @} */ //

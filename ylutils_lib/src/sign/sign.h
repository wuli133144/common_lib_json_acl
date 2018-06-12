/*
 * =====================================================================================
 * 
 *        Filename:  sign.h
 * 
 *     Description:  ��װleim��ǩ���㷨.
 * 
 *         Version:  1.0
 *         Created:  2006��05��25�� 15ʱ03��36�� CST
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
 * \file sign.h
 * \brief ��װleim��ǩ���㷨.
 *
 * ��װleim��ǩ���㷨.
 * \author  ������
 * \date 2006��05��25�� 15ʱ03��36�� CST
 */
 
 /** @defgroup sign
 *  ǩ���㷨
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
/*! \brief ��װleim��ǩ���㷨.
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
 * @brief    ��ָ�����ڴ�ռ����������32λ�޷������͵���.
 *           ͨ���㷨1����ǩ��������ΪgetSign1.
 *
 * @param    src    Ҫ��ǩ���Ĵ�.
 * @param    len    src��Ҫ��ǩ�����ֽ���. ���len==-1���㷨����
 *           strlen(src)���ֽ���ǩ��.
 * @return   ����str�����ͱ�ʾ��ǩ��.
 */
static u_int getSign1(const char *src, int len = -1);
 
/**
 * @brief    ��ָ�����ڴ�ռ����������32λ�޷������͵���.
 *           ͨ���㷨2����ǩ��������ΪgetSign2.
 *
 * @param    src    Ҫ��ǩ���Ĵ�.
 * @param    len    src��Ҫ��ǩ�����ֽ���. ���len==-1���㷨����
 *           strlen(src)���ֽ���ǩ��.
 * @return   ����str�����ͱ�ʾ��ǩ��.
 */
static u_int getSign2(const char *src, int len = -1);
 
/**
 * @brief    ��ָ�����ڴ�ռ��������������32λ�޷���������ʾ������.
 *
 * @param    sign1  ��һ��ǩ��.
 * @param    sign2  �ڶ���ǩ��.
 * @param    src    Ҫ��ǩ���Ĵ�.
 * @param    len    src��Ҫ��ǩ�����ֽ���. ���len==-1���㷨����
 *           strlen(src)���ֽ���ǩ��.
 */
static void getSign(u_int &sign1, u_int &sign2, const char *src, int len = -1);
 
/**
 * @brief    ��ָ�����ڴ�ռ��������������32λ�޷���������ʾ������.
 *
 * @param    sign   ǩ������.
 * @param    src    Ҫ��ǩ���Ĵ�.
 * @param    len    src��Ҫ��ǩ�����ֽ���. ���len==-1���㷨����
 *           strlen(src)���ֽ���ǩ��.
 */
static void getSign(T_Sign &sign, const char *src, int len = -1);
 /**
 * @brief    ��ָ�����ڴ�ռ��������������32λ�޷���������ʾ������.
 *
 * @param    sign   ǩ������.
 * @param    src    Ҫ��ǩ���Ĵ�.
 * @param    len    src��Ҫ��ǩ�����ֽ���. ���len==-1���㷨����
 *           strlen(src)���ֽ���ǩ��.
 */
static T_Sign *getSign(T_Sign *sign, const char *src, int len = -1);

/*!
 *@brief generate file's sig value 
 *@param  path   �ļ�·��
 *@param  nsig1   sigǩ��1
 *@param  nsig2   sigǩ��2 
 *@return  boolean �Ƿ�ɹ�
 */
static bool getNewsig(const char* path, unsigned int* nsig1, unsigned int* nsig2);
/*!
 *@brief generate file's sig value 
 *@param  path   �ļ�·��
 *@param  sig1   sigǩ��1
 *@param  sig2   sigǩ��2 
 *@return  boolean �Ƿ�ɹ�
 */
static bool getOldsig(const char* path, unsigned int* sig1, unsigned int* sig2);
	/**
	 * @brief   �����ַ���64λǩ��
	 * \author  leiming
	 * \version 1.0       
	 * @param   psrc  ������ǩ�����ַ���
	 * @param  slen   psrc�ĳ���
	 * @param   sign1   ���ǩ����32λ
	 * @param   sign2   ���ǩ����32λ
	 * @return  1
	 */  
static int creat_sign_f64 (char* psrc, int slen, u_int* sign1 , u_int* sign2);

	/**
	 * @brief   �����ַ���64λǩ��
	 * \author  leiming	 
	 * \version 1.0       
	 * @param   psrc  ������ǩ�����ַ���
	 * @param  slen   psrc�ĳ���
	 * @param   sign1   ���ǩ����32λ
	 * @param   sign2   ���ǩ����32λ
	 * @return  1
	 */  
static int creat_sign_fs64 (char* psrc, int slen, u_int* sign1 , u_int* sign2);


private:
	static void reverse_byte_order(unsigned char* buf);
};
#endif //SIGN_H_ZMH_2006_05_25
/** @} */ //

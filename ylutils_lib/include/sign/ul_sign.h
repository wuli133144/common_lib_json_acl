//***********************************************************************
//			Signature Utilities (SU) 1.0
//
//   programmed by Ming Lei, Jul, 2000
//***********************************************************************

#ifndef __UL_SIGN_H__
#define __UL_SIGN_H__

// description
//
// input :
//    psrc : src data
//    slen : length of the src data, range : 0-256
//
// output :
//    return value : failed = -1, success = 1
//    psign1 : first 4 bytes of signature
//    psign2 : second 4 bytes of signature
//
// notice :
//    if slen==0            --> sign = 0:0
//    if slen>256 or slen<0 --> sign = 0xFFFFFFFF: 0xFFFFFFFF
//

// int creat_sign_t256(char *psrc, int slen, u_int* psign1, u_int* psign2);
// description
//
// input: 
//    psrc : src data
//    slen : length of the src data, range : 0--
//
// output :
//    return value : failed = -1, success = 1
//    sign1 : first 4 bytes of signature
//    sign2 : second 4 bytes of signature
//
// notice :
//    if slen==0            --> sign = 0:0
//
#include <sys/types.h>
#include <string.h>
#include <assert.h>
#include "ul_prime.h"
	/**
	 * @brief   计算字符串64位签名
	 * \version 1.0       
	 * @param   psrc  待计算签名的字符串
	 * @param  slen   psrc的长度
	 * @param   sign1   结果签名高32位
	 * @param   sign2   结果签名低32位
	 * @return  1
	 */  
int creat_sign_f64 (char* psrc, int slen, u_int* sign1 , u_int* sign2);

	/**
	 * @brief   计算字符串64位签名
	 * \version 1.0       
	 * @param   psrc  待计算签名的字符串
	 * @param  slen   psrc的长度
	 * @param   sign1   结果签名高32位
	 * @param   sign2   结果签名低32位
	 * @return  1
	 */  
int creat_sign_fs64 (char* psrc, int slen, u_int* sign1 , u_int* sign2);
#endif

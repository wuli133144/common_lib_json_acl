/** @defgroup base64
 *  负责url的加密和解密.
 *  @{
 */

/*
 * =====================================================================================
 * 
 *        Filename:  base64.h
 * 
 *     Description:  这两个函数摘自wget.负责url的加密和解密.
 * 
 *         Version:  1.0
 *         Created:  2006年04月12日 18时34分49秒 CST
 *        Revision:  none
 *        Compiler:  g++
 * 
 *          Author:  张明辉 ()
 *         Company:  亿览科技
 *           Email:  minghui.zhang@yeelion.com
 * 
 * =====================================================================================
 */


#ifndef BASE64_H_ZMH_2006_04_12
#define BASE64_H_ZMH_2006_04_12
#define BASE64_LENGTH(len) (4 * (((len) + 2) / 3))

/*!
 *\brief  摘自wget.负责url的加密和解密.
 *! \class YeeBase64 base64.h encoding/base64.h    
 \code
 
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


int main()
{
	char buffer[1024];
	char base64[BASE64_LENGTH(1024)];

	bzero(buffer, sizeof(buffer));
	bzero(base64, sizeof(base64));

	while (1)
	{
		printf("please input your string:");

		if (!fgets(buffer, 1024, stdin))
			break;

		//!< encode.
		YeeBase64::base64_encode(buffer, strlen(buffer), base64);

		//!< decode.
		bzero(buffer, sizeof(buffer));
		YeeBase64::base64_decode(base64, buffer);

		printf("%s\n", base64);
		printf("%s\n", buffer);

		bzero(buffer, sizeof(buffer));
		bzero(base64, sizeof(base64));
	}

	printf("\n");

	return 0;
}
 \endcode
 *  
 */
class YeeBase64{
public:
/**
 * \brief  Encode the string STR of length LENGTH to base64 format and place it
 *        to B64STORE.  The output will be \0-terminated, and must point to a
 *        writable buffer of at least 1+BASE64_LENGTH(length) bytes.  It
 *        returns the length of the resulting base64 data, not counting the
 *        terminating zero.
 *        This implementation will not emit newlines after 76 characters of
 *        base64 data.
 * @param    b64store   加密后的结果buffer.
 * @param    str        待加密的字符串.
 * @param    length     str中要加密的字节长度.
 *
 * @return   返回b64store的长度.
 */
static int base64_encode(char *b64store, const char *str, int length);
/**
 * @brief    encode to base64
 * @param    b64store   加密后的结果buffer.
 * @param    str        待加密的字符串.
 * @param    length     str中要加密的字节长度.
 * @param    xor_string 加密校验字符串
 *
 * @return   返回b64store的长度.
 */
static int base64_encode(char *b64store, const char *str, int length, const char *xor_string);

/** 
 *\brief  Decode data from BASE64 (assumed to be encoded as base64) into
 *          memory pointed to by TO.  TO should be large enough to accomodate
 *          the decoded data, which is guaranteed to be less than
 *          strlen(base64).

 *          Since TO is assumed to contain binary data, it is not
 *          NUL-terminated.  The function returns the length of the data
 *          written to TO.  -1 is returned in case of error caused by malformed
 *          base64 input.  
 * @param    to        解密后的串.
 * @param    base64    加密后的串.
 *
 * @return   解密后的数据的长度.
 */
static int base64_decode(char *to, const char *base64);
/** 
 *\brief      base64 decode
 * @param    to        解密后的串.
 * @param    base64    加密后的串.
 * @param    xor_string  解密校验字符串
 * @return   解密后的数据的长度.
 */
static int base64_decode(char *to, const char *base64, const char *xor_string);

//! How many bytes it will take to store LEN bytes in base64.  */

};
#endif //BASE64_H_ZMH_2006_04_12
/** @} */ // end of Utils

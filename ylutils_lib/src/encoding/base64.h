/** @defgroup base64
 *  ����url�ļ��ܺͽ���.
 *  @{
 */

/*
 * =====================================================================================
 * 
 *        Filename:  base64.h
 * 
 *     Description:  ����������ժ��wget.����url�ļ��ܺͽ���.
 * 
 *         Version:  1.0
 *         Created:  2006��04��12�� 18ʱ34��49�� CST
 *        Revision:  none
 *        Compiler:  g++
 * 
 *          Author:  ������ ()
 *         Company:  �����Ƽ�
 *           Email:  minghui.zhang@yeelion.com
 * 
 * =====================================================================================
 */


#ifndef BASE64_H_ZMH_2006_04_12
#define BASE64_H_ZMH_2006_04_12
#define BASE64_LENGTH(len) (4 * (((len) + 2) / 3))

/*!
 *\brief  ժ��wget.����url�ļ��ܺͽ���.
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
 * @param    b64store   ���ܺ�Ľ��buffer.
 * @param    str        �����ܵ��ַ���.
 * @param    length     str��Ҫ���ܵ��ֽڳ���.
 *
 * @return   ����b64store�ĳ���.
 */
static int base64_encode(char *b64store, const char *str, int length);
/**
 * @brief    encode to base64
 * @param    b64store   ���ܺ�Ľ��buffer.
 * @param    str        �����ܵ��ַ���.
 * @param    length     str��Ҫ���ܵ��ֽڳ���.
 * @param    xor_string ����У���ַ���
 *
 * @return   ����b64store�ĳ���.
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
 * @param    to        ���ܺ�Ĵ�.
 * @param    base64    ���ܺ�Ĵ�.
 *
 * @return   ���ܺ�����ݵĳ���.
 */
static int base64_decode(char *to, const char *base64);
/** 
 *\brief      base64 decode
 * @param    to        ���ܺ�Ĵ�.
 * @param    base64    ���ܺ�Ĵ�.
 * @param    xor_string  ����У���ַ���
 * @return   ���ܺ�����ݵĳ���.
 */
static int base64_decode(char *to, const char *base64, const char *xor_string);

//! How many bytes it will take to store LEN bytes in base64.  */

};
#endif //BASE64_H_ZMH_2006_04_12
/** @} */ // end of Utils

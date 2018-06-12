/*
 * =====================================================================================
 * 
 *        Filename:  FileSignature.h
 * 
 *     Description:  ��ȡ�ļ�ǩ��
 * 
 *         Version:  1.0
 *         Created:  2006��08��24�� 11ʱ13��00�� CST
 *        Revision:  none
 *        Compiler:  g++
 * 
 *          Author:  ��쿶� ()
 *         Company:  �����������缼��(����)���޹�˾
 *           Email:  weidong.gu@yeelion.com
 * 
 * =====================================================================================
 */
 /** @addtogroup sign
 *  ǩ���㷨
 *  @{
 */

#ifndef FILESIGNATURE_H_GWD_2006_08_24
#define FILESIGNATURE_H_GWD_2006_08_24

//#include <iostream>
//#include <string>

//using namespace std;

#include "id3.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <assert.h>
#ifndef WIN32
#include <unistd.h>
#endif

//ʹ���°汾ǩ��
#define NEW_VERSION

typedef unsigned int uint32;
typedef unsigned long long uint64;
const int HASH_SIZE = 10240;		//ȡ����hash���ֽ���
const uint64 SKIP_SIZE = 10240;		//�������ļ���ͷ�ͽ�β���ֽ���
const int MAX_BLOCK_NUM = 30;
const int PACKET_SIZE = 16384;
const int BLOCK_SIZE	= 4;
const int BLOCK_SIZE_4M = 1;
const int BLOCK_SIZE_16M = 4;
const int BLOCK_SIZE_64M = 8;
const int BLOCK_SIZE_128M = 16;
/*! \brief �°汾�ļ�ǩ��
 *! \class FileSignature FileSignature.h sign/FileSignature.h     
 * \author   ��쿶� 
  *  \version 1.0 
 */

class FileSignature
{
public :

	/** \brief �����ļ�ǩ��
	 * @param	filepath		[in]�ļ�·��
	 *@param 		sign1,sign2		[out]ǩ��
	 * @return					true 	�ɹ�
	 * 							false	ʧ��
	 */
	static bool CompFileSign( const char *filepath, uint32& sign1, uint32& sign2);
	
	/** \brief �����ļ�ǩ��
	* @param	filepath		[in]�ļ�·��
	* @param			szCodeBuffer	[out]��֤��Buffer���Ѿ�ת����16���ƴ�
	*@param			nLen			[in]��֤��Buffer�ĳ���
	* @return					true 	�ɹ�
	* 							false	ʧ��
	* @remark
	*		4M���ڵ��ļ����СΪ16K
	*		4~16M���ļ����СΪ64K
	*		16~64M���ļ����СΪ128K
	*		64M�����ļ����СΪ256K
	*/
	static bool GenFileVerifyCode( const char *filepath, char* szCodeBuffer);


protected :

private :
	
	static bool detectVirtualFile(FILE* fp, uint64& nOffset, uint64& nLength);

	
	static bool detectVirtualID3v2(FILE* fp, uint64& nOffset, uint64& nLength);

	
	static bool detectVirtualID3v1(FILE* fp, uint64& nOffset, uint64& nLength);

	
	static void calc_hash(const unsigned char* content, uint32 len, uint32& sign1, uint32& sign2);
	
	
	static uint32 calc_hash(const BYTE* content, uint32 len);

	
	static void DectoHexStr(uint32 decint, char* outStr);

	static uint32 HexStrtoDec(const char* hexStr);
};
#endif //FILESIGNATURE_H_GWD_2006_08_24


/**��@} */ //

/*
 * =====================================================================================
 * 
 *        Filename:  FileSignature.h
 * 
 *     Description:  获取文件签名
 * 
 *         Version:  1.0
 *         Created:  2006年08月24日 11时13分00秒 CST
 *        Revision:  none
 *        Compiler:  g++
 * 
 *          Author:  辜炜东 ()
 *         Company:  亿览在线网络技术(北京)有限公司
 *           Email:  weidong.gu@yeelion.com
 * 
 * =====================================================================================
 */
 /** @addtogroup sign
 *  签名算法
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

//使用新版本签名
#define NEW_VERSION

typedef unsigned int uint32;
typedef unsigned long long uint64;
const int HASH_SIZE = 10240;		//取出来hash的字节数
const uint64 SKIP_SIZE = 10240;		//跳过的文件开头和结尾的字节数
const int MAX_BLOCK_NUM = 30;
const int PACKET_SIZE = 16384;
const int BLOCK_SIZE	= 4;
const int BLOCK_SIZE_4M = 1;
const int BLOCK_SIZE_16M = 4;
const int BLOCK_SIZE_64M = 8;
const int BLOCK_SIZE_128M = 16;
/*! \brief 新版本文件签名
 *! \class FileSignature FileSignature.h sign/FileSignature.h     
 * \author   辜炜东 
  *  \version 1.0 
 */

class FileSignature
{
public :

	/** \brief 计算文件签名
	 * @param	filepath		[in]文件路径
	 *@param 		sign1,sign2		[out]签名
	 * @return					true 	成功
	 * 							false	失败
	 */
	static bool CompFileSign( const char *filepath, uint32& sign1, uint32& sign2);
	
	/** \brief 计算文件签名
	* @param	filepath		[in]文件路径
	* @param			szCodeBuffer	[out]验证码Buffer，已经转换成16进制串
	*@param			nLen			[in]验证码Buffer的长度
	* @return					true 	成功
	* 							false	失败
	* @remark
	*		4M以内的文件块大小为16K
	*		4~16M的文件块大小为64K
	*		16~64M的文件块大小为128K
	*		64M以上文件块大小为256K
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


/**　@} */ //

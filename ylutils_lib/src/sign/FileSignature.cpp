/*
 * =====================================================================================
 * 
 *        Filename:  FileSignature.cpp
 * 
 *     Description:  
 * 
 *         Version:  1.0
 *         Created:  2006年08月24日 11时41分29秒 CST
 *        Revision:  none
 *        Compiler:  g++
 * 
 *          Author:  辜炜东 (), weidong.gu@yeelion.com
 *         Company:  亿览在线网络技术(北京)有限公司
 * 
 * =====================================================================================
 */

#include "FileSignature.h"
#include <string.h>
#include <string>
using std::string;

//获取文件长度
bool GetFileSize( const char* filepath, uint64 &filesize )
{
	struct stat my_stat;
	if (stat(filepath, &my_stat) == 0)
	{
		filesize = my_stat.st_size;
		return true;
	}
	return false;
}
/*
//把str中的大写字母都转换成小写字母
void strlwr( char *str )
{
	char *ptr = str;
	while (*ptr++ != '\0')	
		*ptr = tolower( *ptr );
}
*/

/* 计算文件签名
 * @param	filepath		[in]文件路径
 * 			sign1,sign2		[out]签名
 */
bool FileSignature::CompFileSign( const char *filepath, uint32& sign1, uint32& sign2)
{
	uint64 filesize, basepos = 0;
	if (!GetFileSize( filepath, filesize ))
		return false;
	
	FILE* fp = fopen( filepath, "rb" );
	if (!fp)
	{
		return false;
	}
	
	const char* ext = filepath + strlen(filepath);
	while (*--ext)
	{
		if (*ext == '.')
		{
			ext ++;
			break;
		}
	}
	
	if (strcmp(ext,"MP3") == 0
		||strcmp(ext,"mp3") == 0
		||strcmp(ext,"Mp3") == 0
		||strcmp(ext,"mP3") == 0)
	{
		detectVirtualFile( fp, basepos, filesize );
	}

	if (filesize <= 0)
	{
		fclose( fp );
		return false;
	}
	
	//跳过前面10K
	if (filesize > SKIP_SIZE)
	{
		basepos += SKIP_SIZE;
		filesize -= SKIP_SIZE;
	}
	//跳过后面10K
	if (filesize > SKIP_SIZE)
		filesize -= SKIP_SIZE;
	
	//每16K字节(一个p2p基本传输块)读取若干(几十)字节，拼成算签名的缓冲区
	//bufSize为缓冲区大小
	uint32 bufSize = HASH_SIZE;
	if ((uint64)filesize < (uint64)(HASH_SIZE))
		bufSize = filesize;
	
	//加上存储文件大小的filesize的长度
	bufSize += sizeof(uint64);
	
	unsigned char* hashBuf = new unsigned char[bufSize];
	memset (hashBuf, 0, bufSize);
#ifdef NEW_VERSION
	int BLOCK_NUM = (int)(filesize/(16*1024));
	if (BLOCK_NUM == 0)
		BLOCK_NUM = 1;
	if (BLOCK_NUM > MAX_BLOCK_NUM)
		BLOCK_NUM = MAX_BLOCK_NUM;
#else
	int BLOCK_NUM = 10;
#endif


	//文件比HASH_SIZE还小 则读取整个文件
	if (bufSize < HASH_SIZE+sizeof(uint64) )
	{
		fseek( fp, basepos, SEEK_SET );
		if (fread( hashBuf, filesize, 1, fp) != 1)
		{
			delete []hashBuf;
			fclose(fp);
			return false;
		} 
		//加上文件大小
		memcpy( hashBuf + filesize, &filesize, sizeof(filesize));
	}
	else
	{
		uint32 nTotalRead = 0;					//已读字节数
		uint32 nRead = HASH_SIZE/BLOCK_NUM;		//每段读取的字节数
		uint64 nInterval = filesize/BLOCK_NUM;	//读取间隔
		for (int i = 0; i < BLOCK_NUM; i++)
		{
			fseek( fp, basepos + nInterval*i, SEEK_SET );
			if (i == BLOCK_NUM-1)
			{
				nRead = HASH_SIZE - nTotalRead;
			}
			if (fread( hashBuf + nTotalRead, nRead, 1, fp ) != 1)
			{
				printf("fread error. nTotalRead : %d.\n", nTotalRead);
				delete []hashBuf;
				fclose(fp);
				return false;
			}
			nTotalRead += nRead;
		}
		//加上文件大小
		memcpy( hashBuf + HASH_SIZE, &filesize, sizeof(filesize));
	}

	fclose(fp);
	
	calc_hash( hashBuf, bufSize, sign1, sign2 );
	delete []hashBuf;
	
	return true;
}

bool FileSignature::GenFileVerifyCode( const char *filepath, char* szCodeBuffer)
{
	uint64 filesize, basepos = 0;
	if (!GetFileSize( filepath, filesize ))
		return false;

	FILE* fp = fopen( filepath, "rb" );
	if (!fp)
	{
		return false;
	}

	const char* ext = filepath + strlen(filepath);
	while (*--ext)
	{
		if (*ext == '.')
		{
			ext ++;
			break;
		}
	}

	if (strcmp(ext,"MP3") == 0
		||strcmp(ext,"mp3") == 0
		||strcmp(ext,"Mp3") == 0
		||strcmp(ext,"mP3") == 0)
	{
		detectVirtualFile( fp, basepos, filesize );
	}

	if (filesize <= 0)
	{
		fclose( fp );
		return false;
	}
	fseek( fp, basepos, SEEK_SET );

	int blockSize = BLOCK_SIZE;
	if(filesize <= 4 * 1024 * 1024)
		blockSize = BLOCK_SIZE_4M;
	else if (filesize <= 16 * 1024 * 1024)
		blockSize = BLOCK_SIZE_16M;
	else if (filesize <= 64 * 1024 * 1024)
		blockSize = BLOCK_SIZE_64M;
	else
		blockSize = BLOCK_SIZE_128M;

	int blocknum = filesize / ( blockSize * PACKET_SIZE);
	if ((filesize % ( blockSize * PACKET_SIZE))  != 0)
		blocknum++;

	char* buf = new char[PACKET_SIZE];
	int nRead = 0;
	uint32 hashvalue = 0;
	char uint32Str[9];
	szCodeBuffer[0] = 0;
#ifdef _DEBUG
	string strIntHashValue = "";
#endif
	for (int i = 0; i < blocknum; i++)
	{
		int j = 0;
		hashvalue = 0;
		while(j < blockSize && (nRead = fread (buf, 1, PACKET_SIZE, fp)) > 0)
		{
			hashvalue = hashvalue ^ calc_hash((BYTE*)buf, nRead);
			j++;
		}

#ifdef _DEBUG
		char tmp[20];
		sprintf(tmp, "%u", hashvalue);
		strIntHashValue += tmp;
		strIntHashValue += "\r\n";
#endif
		DectoHexStr(hashvalue, uint32Str);
		strcat(szCodeBuffer, uint32Str);
	}


#ifdef _DEBUG
	printf("%s\r\n\r\n", strIntHashValue.c_str());
	strIntHashValue = "";
	char tmp[9];
	for ( int i = 0; i < strlen(szCodeBuffer) / 8; i++)
	{
		tmp[8] = 0;
		memcpy(tmp, szCodeBuffer + i * 8, 8);
		uint32 iTmp = HexStrtoDec(tmp);
		char tmp[20];
		sprintf(tmp, "%u", iTmp);
		strIntHashValue += tmp;
		strIntHashValue += "\r\n";
	}
	printf("%s", strIntHashValue.c_str());
#endif
	fclose(fp);
	delete[] buf;
}


/**
 * 检验文件是否为mp3,如果是mp3则返回文件头结束的位置和除文件头后剩下的长度
 * @param 	fp 						[in]文件名指针
 * 			nOffset 				[out]文件头偏移位置
 * 			nLength 				[out]文件体长度
 * 
 * Remarks:
 * 	打开一个文件后，可以得到hFile, nOffset为0, nLength为文件长度，detectVirtualFile会
 * 将文件头和尾的歌曲Tag信息长度忽略掉，输出新的nOffset和nLength
 */
bool FileSignature::detectVirtualFile(FILE* fp, uint64& nOffset, uint64& nLength)
{
	bool bVirtual = false;
	bVirtual |= detectVirtualID3v2 ( fp, nOffset, nLength );
	bVirtual |= detectVirtualID3v1 ( fp, nOffset, nLength );
	return bVirtual;
}

/**
 * 判断MP3文件的TAG版本是否为ID3v2，如果是则将nOfferSet设为从MP3数据块开始的位置，长度为删除ID3V2长度后剩下的大小
 * @param	fp						[in]文件指针
 * 			nOffset					[out]文件体偏移位置
 * 			nLength					[out]文件体长度
 */
bool FileSignature::detectVirtualID3v2(FILE* fp, uint64& nOffset, uint64& nLength)
{
	if (!fp)
		return false;
	
	ID3V2_HEADER pHeader;
	fseek( fp, nOffset, SEEK_SET );
	if (fread( &pHeader, sizeof(pHeader), 1, fp) != 1)
		return false;

	if (strncmp( pHeader.szTag, ID3V2_TAG, 3) != 0)
		return false;
	if (strncmp( pHeader.szTag, "id3", 3) == 0)
		printf("id3\n");
	if (pHeader.nMajorVersion < 2 || pHeader.nMajorVersion > 4)
		return false;
	if (pHeader.nFlags & ~ID3V2_KNOWNMASK )
		return false;
	if (pHeader.nFlags & ~ID3V2_UNSYNCHRONISED )
		return false;

	//nTagSize表示ID3V2头大小
	uint32 nTagSize = SWAP_LONG( pHeader.nSize );
	ID3_DESYNC_SIZE( nTagSize );

	if (pHeader.nFlags & ID3V2_FOOTERPRESENT)
		nTagSize += 10;
	nTagSize += sizeof(pHeader);
	
	if (nLength <= nTagSize)
		return false;

	nOffset += nTagSize;
	nLength -= nTagSize;

	return true;
}

/**
 * 检验文件是否包含ID3v1信息，如果包含，则将ID3V1信息忽略，得出新的nOffset和nLength
 * @param 	fp 						[in]文件指针
 * 			nOffset 				[out]文件头偏移位置
 * 			nLength 				[out]文件体长度
 */
bool FileSignature::detectVirtualID3v1(FILE* fp, uint64& nOffset, uint64& nLength)
{
	if (!fp)
		return false;
	if (nLength < 128)
		return false;
	
	ID3V1 pInfo;
	fseek( fp, nOffset+nLength-128, SEEK_SET );
	if (fread( &pInfo, sizeof(pInfo), 1, fp ) != 1)
		return false;

	if (strncmp( pInfo.szTag, ID3V1_TAG, 3) != 0)
		return false;

	if (strncpy( pInfo.szTag, "tag", 3) == 0)
		printf("tag\n");

	nLength -= 128;
	return true;
}


/**
 * 计算一串二进制内容的hash值
 * @param 	content	 				[in]指向要计算的二进制内容
 * 			len 					[in]二进制内容串的长度
 * 			sign1 					[out]输出的hash值的前面4个字节
 * 			sign2 					[out]输出的hash值的后面4个字节
 */
void FileSignature::calc_hash(const unsigned char* content, uint32 len, uint32& sign1, uint32& sign2)
{
	sign1 = sign2 = 0;
	for (int i = 0; i < (int)len/2; i++)
	{
		sign1 *= 16777619;
		sign1 ^= (uint32)*((unsigned char*)content + i*2);
		sign2 *= 16777619;
		sign2 ^= (uint32)*((unsigned char*)content + i*2 + 1);
	}
}

uint32 FileSignature::calc_hash(const BYTE* content, uint32 len)
{
	const BYTE *end = content + len;
	uint32 hash = 0;
	for (; content < end; content++)
	{
		hash *= 16777619;
		hash ^= (uint32) *(unsigned char*) content;
	}
	return (hash);
}

void FileSignature::DectoHexStr(uint32 decint, char* outStr)
{
	outStr[0] = 0;
	char tmp[9];
	sprintf(tmp, "%08X", decint);
	strcat(outStr, tmp);
}

uint32 FileSignature::HexStrtoDec(const char* hexStr)
{
	if (strlen(hexStr) != 8)
		return 0;

	uint32 dec;

	char buf[8];
	buf[2] = 0;

	dec = (uint32)strtoul(hexStr, NULL, 16);
	return dec;
}


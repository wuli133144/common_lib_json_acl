/*
 * =====================================================================================
 * 
 *        Filename:  FileSignature.cpp
 * 
 *     Description:  
 * 
 *         Version:  1.0
 *         Created:  2006��08��24�� 11ʱ41��29�� CST
 *        Revision:  none
 *        Compiler:  g++
 * 
 *          Author:  ��쿶� (), weidong.gu@yeelion.com
 *         Company:  �����������缼��(����)���޹�˾
 * 
 * =====================================================================================
 */

#include "FileSignature.h"
#include <string.h>
#include <string>
using std::string;

//��ȡ�ļ�����
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
//��str�еĴ�д��ĸ��ת����Сд��ĸ
void strlwr( char *str )
{
	char *ptr = str;
	while (*ptr++ != '\0')	
		*ptr = tolower( *ptr );
}
*/

/* �����ļ�ǩ��
 * @param	filepath		[in]�ļ�·��
 * 			sign1,sign2		[out]ǩ��
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
	
	//����ǰ��10K
	if (filesize > SKIP_SIZE)
	{
		basepos += SKIP_SIZE;
		filesize -= SKIP_SIZE;
	}
	//��������10K
	if (filesize > SKIP_SIZE)
		filesize -= SKIP_SIZE;
	
	//ÿ16K�ֽ�(һ��p2p���������)��ȡ����(��ʮ)�ֽڣ�ƴ����ǩ���Ļ�����
	//bufSizeΪ��������С
	uint32 bufSize = HASH_SIZE;
	if ((uint64)filesize < (uint64)(HASH_SIZE))
		bufSize = filesize;
	
	//���ϴ洢�ļ���С��filesize�ĳ���
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


	//�ļ���HASH_SIZE��С ���ȡ�����ļ�
	if (bufSize < HASH_SIZE+sizeof(uint64) )
	{
		fseek( fp, basepos, SEEK_SET );
		if (fread( hashBuf, filesize, 1, fp) != 1)
		{
			delete []hashBuf;
			fclose(fp);
			return false;
		} 
		//�����ļ���С
		memcpy( hashBuf + filesize, &filesize, sizeof(filesize));
	}
	else
	{
		uint32 nTotalRead = 0;					//�Ѷ��ֽ���
		uint32 nRead = HASH_SIZE/BLOCK_NUM;		//ÿ�ζ�ȡ���ֽ���
		uint64 nInterval = filesize/BLOCK_NUM;	//��ȡ���
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
		//�����ļ���С
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
 * �����ļ��Ƿ�Ϊmp3,�����mp3�򷵻��ļ�ͷ������λ�úͳ��ļ�ͷ��ʣ�µĳ���
 * @param 	fp 						[in]�ļ���ָ��
 * 			nOffset 				[out]�ļ�ͷƫ��λ��
 * 			nLength 				[out]�ļ��峤��
 * 
 * Remarks:
 * 	��һ���ļ��󣬿��Եõ�hFile, nOffsetΪ0, nLengthΪ�ļ����ȣ�detectVirtualFile��
 * ���ļ�ͷ��β�ĸ���Tag��Ϣ���Ⱥ��Ե�������µ�nOffset��nLength
 */
bool FileSignature::detectVirtualFile(FILE* fp, uint64& nOffset, uint64& nLength)
{
	bool bVirtual = false;
	bVirtual |= detectVirtualID3v2 ( fp, nOffset, nLength );
	bVirtual |= detectVirtualID3v1 ( fp, nOffset, nLength );
	return bVirtual;
}

/**
 * �ж�MP3�ļ���TAG�汾�Ƿ�ΪID3v2���������nOfferSet��Ϊ��MP3���ݿ鿪ʼ��λ�ã�����Ϊɾ��ID3V2���Ⱥ�ʣ�µĴ�С
 * @param	fp						[in]�ļ�ָ��
 * 			nOffset					[out]�ļ���ƫ��λ��
 * 			nLength					[out]�ļ��峤��
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

	//nTagSize��ʾID3V2ͷ��С
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
 * �����ļ��Ƿ����ID3v1��Ϣ�������������ID3V1��Ϣ���ԣ��ó��µ�nOffset��nLength
 * @param 	fp 						[in]�ļ�ָ��
 * 			nOffset 				[out]�ļ�ͷƫ��λ��
 * 			nLength 				[out]�ļ��峤��
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
 * ����һ�����������ݵ�hashֵ
 * @param 	content	 				[in]ָ��Ҫ����Ķ���������
 * 			len 					[in]���������ݴ��ĳ���
 * 			sign1 					[out]�����hashֵ��ǰ��4���ֽ�
 * 			sign2 					[out]�����hashֵ�ĺ���4���ֽ�
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


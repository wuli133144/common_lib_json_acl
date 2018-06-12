/*
 * =====================================================================================
 * 
 *        Filename:  Compress.cpp
 * 
 *     Description:  
 * 
 *         Version:  1.0
 *         Created:  2005��12��14�� 16ʱ01��43�� CST
 *        Revision:  none
 *        Compiler:  g++
 * 
 * 
 * =====================================================================================
 */

#include "Compress.h"


int Compress::compress(string &result, const char *source, int sourceLen)
	throw (CompressException)
{
	uLong resultBoundLen = compressBound((uLong)sourceLen);

	result.resize((size_t)resultBoundLen);

	ulong realLen = resultBoundLen;
	int ret = ::compress((Bytef *)&result[0], (uLongf *)&realLen,
			(const Bytef *)source, (uLong)sourceLen);

	string msg = "Compress::compress() : ";
	switch (ret)
	{
	case Z_OK :
		result.resize(realLen);
		return Z_OK;
		break;
	case Z_MEM_ERROR :
		msg += "Z_MEM_ERROR û���㹻���ڴ�.";
		throw CompressException(msg);
		break;
	case Z_BUF_ERROR :
		msg += "Z_BUF_ERROR û���㹻�������.";
		throw CompressException(msg);
		break;
	default :
		break;
	}

	return Z_OK;
}

int Compress::unCompress(string &result, int rawLen, const char *compressedBytes, int compressedLen)
	throw (CompressException)
{
	//! rawLen��Ҫ��ͨ��ѹ��ʱ��洢���µ�.
	//! �������Ӧ�ô��ڵ���ԭ���ĳ��ȣ��������ֻ���������Ĵ���.
	result.resize(rawLen);

	ulong realLen = rawLen;
	int ret = ::uncompress((Bytef *)&result[0], (uLong *)&realLen,
			(Bytef *)compressedBytes, (uLong)compressedLen);

	string msg = "Compress::unCompress() : ";
	switch (ret)
	{
	case Z_OK :
		result.resize(realLen);
		return Z_OK;
	case Z_MEM_ERROR :
		msg += "Z_MEM_ERROR û���㹻���ڴ�.";
		throw CompressException(msg);
		break;
	case Z_BUF_ERROR :
		msg += "Z_BUF_ERROR û���������.";
		throw CompressException(msg);
		break;
	case Z_DATA_ERROR :
		msg += "Z_DATA_ERROR ���������д�.";
		throw CompressException(msg);
		break;
	default :
		break;
	}

	return Z_OK;
}


#ifdef _TEST


static void dohelp(int argc, char *argv[])
{
	cout << "usage : " << argv[0] << " string" << endl;
	return;
}

#include <fstream>

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		dohelp(argc, argv);
		return 1;
	}

	string compressed;

	try {
		Compress::compress(compressed, argv[1], strlen(argv[1]));

		cout << "the raw len : " << strlen(argv[1]) << endl;
		cout << "after compressing len : " << compressed.length() << endl;

		ofstream fout("com.gzip", ios_base::binary);
		fout.write((const char *)compressed.c_str(), compressed.size());
		fout.close();

		string uncompressed;

		Compress::unCompress(uncompressed, strlen(argv[1]),
				compressed.c_str(), compressed.length());

		cout << "after uncompressing : " << uncompressed << endl;
	} catch (CompressException &E) {
		cout << E.what() << endl;
	} catch (exception &E) {
		cout << E.what() << endl;
	}

	return 0;
}
#endif //_TEST

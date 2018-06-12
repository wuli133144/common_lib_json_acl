/*
 * =====================================================================================
 * 
 *        Filename:  Compress.h
 * 
 *     Description:  �򵥷�װһ��zlibc���г��õĺ���.
 * 
 *         Version:  1.0
 *         Created:  2005��12��09�� 09ʱ34��36�� CST
 *        Revision:  none
 *        Compiler:  g++
 * 
 *          Author:  ������ ()
 * 
 * =====================================================================================
 */
/** @defgroup zip
 * �򵥷�װһ��zlibc���г��õĺ���.
 *  
 *  @{
 */

#ifndef COMPRESS_H_ZMH_2005_12_09
#define COMPRESS_H_ZMH_2005_12_09

#include <iostream>
#include <string>
#include <zlib.h>

using namespace std;
/*! 
 *   CompressException
   *! \class CompressException Compress.h compress/Compress.h  
 */
class CompressException : public std::exception
{
public :
	//! Constructor.
	explicit
	CompressException(const string &msg) throw() : mMsg(msg){}
	
	//! Destructor.
	virtual ~CompressException() throw() {}

	//! ��ȡ�쳣��Ϣ.
	virtual const char *what() const throw() {return mMsg.c_str();}

private :
	string mMsg;
};
/*! \brief   Compress
  *! \class Compress Compress.h compress/Compress.h  
  \code
  
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
  \endcode
 */
class Compress
{
public :
	//! Constructor.
	Compress();

	//! Destructor.
	virtual ~Compress();

	/**
	 * @brief    ��ѹ�����Ĵ���ѹ����,����ԭ�е�zlib.h�е�uncompress��������ĳ���
	 *           �Ͷ�̬�ڴ��������̱ȽϷ�������������string��������.
	 *
	 * @param    result           ��Ž�ѹ���.
	 * @param    rawLen           ԭʼ���ĳ��ȣ�������������Ա�Ԥ����ռ�.
	 * @param    compressedBytes  ѹ�����Ĵ�.
	 * @param    compressedLen    ѹ�����ĳ���.
	 * @return   ѹ���Ľ����ʶ.
	 * @retval   Z_OK             ��ѹ�ɹ�.
	 *
	 * ���¼�����uncompressԭ�����ķ���ֵ�ڴ˲��ᱻ���أ��滻���׳��쳣.
	 * @retval   Z_MEM_ERROR      û���㹻���ڴ�.
	 * @retval   Z_BUF_ERROR      ������治��������zlib.h�е�����,�����ﲻ�ᷢ��.
	 * @retval   Z_DATA_ERROR     �������ݲ�����.
	 *
	 * @exception                 û�з���Z_OK���׳��쳣�������û���Ҫ��ͨ������
	 *                            ����ֵ���жϽ�ѹ����ˣ�Ӧ�ò�׽�쳣.
	*/ 
	static int unCompress(string &result, int rawLen, 
			const char *compressedBytes, int compressedLen)
		throw(CompressException);

	//! unCompress�������.
	static int compress(string &result, const char *source, int sourceLen)
		throw(CompressException);
protected :

private :
		//! bigpc�ϵ�zlib�汾û������������Ҿ͸�����һ������û���ҵ�
		//! ���������Դ���룬��ͨ�����Ĺ����ж�������ô����.
		//! �����ҵ���Դ��,���滻���Լ����ֵĹ���
		//static uLong compressBound(uLong sourceLen) {
		//	return sourceLen*3;
			//return sourceLen + (sourceLen >> 12) + (sourceLen >> 14) + 11;
	//	}
	
};
#endif //COMPRESS_H_ZMH_2005_12_09
/** @} */ //

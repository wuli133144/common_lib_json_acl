#ifndef _YL_MD5_H_
#define _YL_MD5_H_

#include <string>

using namespace std;


class MD5
{
public:
	// �����ļ���md5�������ַ���md5ֵ��32���ֽڣ��������ʧ�ܣ�����һ�����ַ���
	static string getFileMd5(const char* path);
private:	
	static void reverse_byte_order(unsigned char* buf);
};

#endif


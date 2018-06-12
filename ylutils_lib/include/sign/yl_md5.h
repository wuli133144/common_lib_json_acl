#ifndef _YL_MD5_H_
#define _YL_MD5_H_

#include <string>

using namespace std;


class MD5
{
public:
	// 计算文件的md5，返回字符串md5值，32个字节，如果计算失败，返回一个空字符串
	static string getFileMd5(const char* path);
private:	
	static void reverse_byte_order(unsigned char* buf);
};

#endif


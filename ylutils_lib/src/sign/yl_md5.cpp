#include "yl_md5.h"
#include <openssl/md5.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

void MD5::reverse_byte_order(unsigned char* buf)
{
	unsigned char *p1, *p2, *p3, *p4;
	p1 = buf;
	p2 = buf + 1;
	p3 = buf + 2;
	p4 = buf + 3;

	unsigned char t;
	t = *p1;
	*p1 = *p4;
	*p4 = t;

	t = *p2;
	*p2 = *p3;
	*p3 = t;
}

string MD5::getFileMd5(const char* path)
{
	int fd = open(path, O_RDONLY);
	if(fd < 0) {
		return "";
	}

	int BUFSIZE = 8192;
	char* buf = new char[BUFSIZE];
	if(buf == NULL) {
		return "";
	}

	MD5_CTX ctx;
	MD5_Init(&ctx);
        while(1) {
                int nread = read(fd, buf, BUFSIZE);
		if(nread == 0) {
			break;
		}else if(nread < 0) {
			return "";
		}else {
			MD5_Update(&ctx, buf, nread);			
		}
        }

        close(fd);

	unsigned char* md5val = new unsigned char[16];
	MD5_Final(md5val, &ctx);
	reverse_byte_order(md5val);
	reverse_byte_order(md5val+4);
	reverse_byte_order(md5val+8);
	reverse_byte_order(md5val+12);

	unsigned int* md5uint = (unsigned int*)md5val;

	char md5_result[50];
	sprintf(md5_result, "%08x%08x%08x%08x", md5uint[0],md5uint[1],md5uint[2],md5uint[3]);

	delete [] buf;
	delete [] md5val;

	return string(md5_result);
}

#ifdef _TEST

#include <iostream>

int main(int argc, char* argv[])
{
	cout<<MD5::getFileMd5(argv[1])<<endl;
	return 0;
}

#endif


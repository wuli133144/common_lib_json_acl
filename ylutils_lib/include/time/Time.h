#ifndef _TIME_H_
#define _TIME_H_

#include <string>

using namespace std;

class Time
{
public :
	//! constructor
	Time();
	//! destructor
	~Time();

	//! 获取目前的时间，单位是毫秒.
	static long getCurrentMillis();

	//! 获取目前的时间，单位是毫秒，另一种实现
	static long getCurrentMillis2();

	//! 当前时间字符串，格式：Wed Nov 10 10:41:16 2010
	static string nowString();

	// 将当前时间字符串写入buf中，成功返回0，失败返回-1。格式同上。由调用者保证buf足够大。
	static int getCurrTimeStr(char* buf);

	// 返回指定时间的字符串，格式 2010-11-10 10:41:16
	static string getTimeStr_a(time_t a);

	// sleep, 时间精确到毫秒
	static void sleep_ms(long t);

protected :

private :

};

#endif

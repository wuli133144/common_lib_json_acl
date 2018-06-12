#include "Time.h"
#include <sys/timeb.h>
#include <sys/time.h>
#include <unistd.h>
#include <time.h>

long Time::getCurrentMillis()
{
	struct timeb tb;

	ftime(&tb);
	return tb.time * 1000 + tb.millitm;
}

long Time::getCurrentMillis2()
{
	struct timeval tv;
	struct timezone tz;

	gettimeofday(&tv, &tz);

	return tv.tv_sec*1000 + (long)(tv.tv_usec/1000);
}

string Time::nowString()
{
	time_t tt = time(NULL);
	return ctime(&tt);
}

int Time::getCurrTimeStr(char* buf)
{
        time_t a = time(0);
        if(ctime_r(&a, buf) == NULL)
                return -1;
	buf[strlen(buf)-1] = 0;
        return 0;
}

string Time::getTimeStr_a(time_t a)
{
        struct tm ta;
        localtime_r(&a, &ta);

        char buf[24];
        sprintf(buf, "%d-%02d-%02d %02d:%02d:%02d", ta.tm_year+1900,ta.tm_mon+1,ta.tm_mday,ta.tm_hour,ta.tm_min,ta.tm_sec);
        return string(buf);
}

void Time::sleep_ms(long t)
{
	timespec req, rem;
	req.tv_sec = t/1000;
	req.tv_nsec = (t%1000) * 1000000;

	nanosleep(&req, &rem);
}

#ifdef _TEST

#include <iostream>

int main(int argc, char* argv[])
{
	int ms = atoi(argv[1]);

	cout<<Time::getCurrentMillis()<<endl;
	Time::sleep_ms(ms);
	cout<<Time::getCurrentMillis2()<<endl;

	cout<<Time::nowString()<<endl;
	char buf[100];
	Time::getCurrTimeStr(buf);
	cout<<buf<<endl;

	return 0;
}

#endif

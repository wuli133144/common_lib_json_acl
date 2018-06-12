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

	//! ��ȡĿǰ��ʱ�䣬��λ�Ǻ���.
	static long getCurrentMillis();

	//! ��ȡĿǰ��ʱ�䣬��λ�Ǻ��룬��һ��ʵ��
	static long getCurrentMillis2();

	//! ��ǰʱ���ַ�������ʽ��Wed Nov 10 10:41:16 2010
	static string nowString();

	// ����ǰʱ���ַ���д��buf�У��ɹ�����0��ʧ�ܷ���-1����ʽͬ�ϡ��ɵ����߱�֤buf�㹻��
	static int getCurrTimeStr(char* buf);

	// ����ָ��ʱ����ַ�������ʽ 2010-11-10 10:41:16
	static string getTimeStr_a(time_t a);

	// sleep, ʱ�侫ȷ������
	static void sleep_ms(long t);

protected :

private :

};

#endif

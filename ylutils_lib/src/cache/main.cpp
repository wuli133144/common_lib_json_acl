#include "CacheManager.h"

#include <stdio.h>

struct A
{
	int a;
	int b;
};

int main()
{
	CacheManager<struct A> cacher(50);
	A a;
	a.a = 10;
	a.b = 5;
	char buf[64];
	for(int i = 0; i < 30; i++)
	{
		sprintf(buf,"tt_%d",i);
		cacher.insert(buf,a);
	}

	A tmp;
	cacher.find("tt_0",tmp);
	cacher.find("tt_1",tmp);
	
	for(int i = 31; i < 70; i++)
	{
		sprintf(buf,"tt_%d",i);
		cacher.insert(buf,a);
	}
	A aa;
	cacher.find("tt",aa);
	cout << aa.a << "\t" << aa.b << endl;

	cacher.remove("tt");
	if(cacher.find("tt",aa))
		cout << aa.a << "\t" << aa.b << endl;
	else
		cout << "not found" << endl;
}

/*
 * =====================================================================================
 * 
 *        Filename:  SafeVectorByCond.cpp
 * 
 *     Description:  
 * 
 *         Version:  1.0
 *         Created:  2006年06月17日 12时49分18秒 CST
 *        Revision:  none
 *        Compiler:  g++
 * 
 *          Author:  张明辉 (), minghui.zhang@yeelion.com
 *         Company:  亿览科技
 * 
 * =====================================================================================
 */

#include "SafeVectorByCond.h"


static SafeVectorByCond sv;

void *get(void *arg)
{
	const char *name = (const char *)arg;
	int value = 0;
	while (1)
	{
		sv.get(value);
		cout << name << " get a value : " << value << endl;
	}

	return NULL;
}

void *put(void *arg)
{
	const char *name = (const char *)arg;
	int value = 0;
	while (1)
	{
		cout << endl << endl << name << " put a value : " << value << endl;
		sv.put(value++);
		sleep(1);
	}

	return NULL;
}

int main()
{
	pthread_t put_id1, put_id2, get_id;

	char *put_name1 = "put1";
	char *put_name2 = "put2";
	char *get_name = "get";
	
	pthread_create(&put_id1, NULL, put, put_name1);
	pthread_create(&put_id2, NULL, put, put_name2);
	pthread_create(&get_id, NULL, get, get_name);

	pthread_join(put_id1, NULL);
	pthread_join(put_id2, NULL);
	pthread_join(get_id, NULL);

	return 0;
}


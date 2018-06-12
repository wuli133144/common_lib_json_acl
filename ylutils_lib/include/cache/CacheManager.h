#ifndef CACHEMANAGER_H_2008_11_11
#define CACHEMANAGER_H_2008_11_11

#include "sign/sign.h"

#include <pthread.h>

using namespace std;

/**
 * 简单的Cache管理器
 * 
 * (1) cache的内容是一个定长的结构体，可以通过一个字符串定位到该结构体
 * (2) 替换策略非常简单，最不常使用的被替换掉
 * (3) 线程安全
 * (4) 实现方法，采用两个双向链表，一个是hash冲突链，另一个是活动链
 */

//! 定义双向链表
typedef struct
{
	int next;
	int prev;
} T_Link;

template<class T> struct Item
{
	T 	value;
	T_Sign 	sign;
	T_Link 	hashlink;
	T_Link  link;
};

inline bool operator==(const T_Sign& sign1, const T_Sign& sign2)
{
	return (sign1.sign1 == sign2.sign1) && (sign2.sign2 == sign2.sign2);
}

template<class T> class CacheManager
{
	public:
		CacheManager(u_int s = 1000000);
		~CacheManager();

		/**
		 * insert(): 在cache中添加一个item
		 */ 
		void insert( const string& key, const T& item );

		/**
		 * remove(): 删除key对应的item
		 */
		void remove( const string& key );
		
		/**
		 * find(): 获取key对应的item
		 */
		bool find( const string& key, T& value );
		
	private:
		//! 回收已经分配的cache空间
		void reclaim();

		int get_from_free_list();
		void insert_into_free_list( int pos );
		void del_from_busy_list( int pos );
		void insert_into_busy_list( int pos );
		
	private:
		u_int		size;
		Item<T>* 	pvalue;
		int*		pidx;
		T_Link		free_list;
		T_Link		busy_list;
		u_int 		free_list_size;
		pthread_mutex_t fmutex;
};


template<class T> CacheManager<T>::CacheManager( u_int s )
{
	size = s;
	pidx = new int[size];
	pvalue = new Item<T>[size];
	for(u_int i = 0; i < size; i++)
	{
		pidx[i] = -1;
		pvalue[i].hashlink.next = -1;
		pvalue[i].hashlink.prev = -1;
		pvalue[i].link.next = i+1;
		pvalue[i].link.prev = i-1;
	}
	free_list_size = size;
	free_list.next = 0;
	free_list.prev = size-1;
	busy_list.next = busy_list.prev = -1;
	pvalue[0].link.prev = -1;
	pvalue[size-1].link.next = -1;
	pthread_mutex_init(&fmutex,NULL);
}

template<class T> CacheManager<T>::~CacheManager()
{
	delete[] pidx;
	delete[] pvalue;
	pthread_mutex_destroy(&fmutex);
}

template<class T> int CacheManager<T>::get_from_free_list()
{
	int ret = free_list.next;
	//cout << "\nret = " << ret << endl;
	if(pvalue[ret].link.next == -1)
		free_list.prev = -1;
	else
		pvalue[pvalue[ret].link.next].link.prev = -1;
	free_list.next = pvalue[ret].link.next;
	--free_list_size;
	//cout << "free_list_size = " << free_list_size << endl;
	//cout << " [ " << free_list.next << "," << free_list.prev << " ] " << endl;
	return ret;
}

template<class T> void CacheManager<T>::insert_into_free_list( int pos )
{
	int k = free_list.prev;
	//cout << " $$$   k = " << k << endl;
	if(k == -1)
	{
		free_list.prev = pos;
		free_list.next = pos;
		pvalue[pos].link.next = -1;
		pvalue[pos].link.prev = -1;
	}
	else
	{
		free_list.prev = pos;
		pvalue[k].link.next = pos;
		pvalue[pos].link.next = -1;
		pvalue[pos].link.prev = k;
	}
	++free_list_size;
	//cout << "free_list_size = " << free_list_size << endl;
}

template<class T> void CacheManager<T>::insert_into_busy_list( int pos )
{
	int k = busy_list.prev;
	if(k == -1)
	{
		busy_list.prev = pos;
		busy_list.next = pos;
		pvalue[pos].link.next = -1;
		pvalue[pos].link.prev = -1;
	}
	else
	{
		busy_list.prev = pos;
		pvalue[k].link.next = pos;
		pvalue[pos].link.next = -1;
		pvalue[pos].link.prev = k;
	}
}

template<class T> void CacheManager<T>::del_from_busy_list( int pos )
{
	if(pvalue[pos].link.prev == -1)
		busy_list.next = pvalue[pos].link.next;
	else
		pvalue[pvalue[pos].link.prev].link.next = pvalue[pos].link.next;
	if(pvalue[pos].link.next == -1)
		busy_list.prev = pvalue[pos].link.prev;
	else
		pvalue[pvalue[pos].link.next].link.prev = pvalue[pos].link.prev;
}

template<class T> void CacheManager<T>::reclaim()
{
	//cout << "reclaim() ... ..." << endl;
	u_int count = (u_int)(size*0.1);
	//cout << "count = " << count << endl;
	for(u_int i = 0; i < count; i++)
	{
		int k = busy_list.next;
		//cout << "k = " << k << endl;
		del_from_busy_list(k);
		
		// delete from hash list
		int index = (pvalue[k].sign.sign1+pvalue[k].sign.sign2)%size;
		if(pvalue[k].hashlink.prev == -1)
			pidx[index] = pvalue[k].hashlink.next;
		else
			pvalue[pvalue[k].hashlink.prev].hashlink.next = pvalue[k].hashlink.next;
		if(pvalue[k].hashlink.next != -1)
			pvalue[pvalue[k].hashlink.next].hashlink.prev = pvalue[k].hashlink.prev;
		
		insert_into_free_list(k);
	}
	//cout << "reclaim() ... ... finished" << endl;
}

template<class T> void CacheManager<T>::insert( const string& key, const T& value )
{
	T_Sign sign;
	YeeSign::getSign(sign,key.c_str());
	int index = (sign.sign1+sign.sign2)%size;
	//cout << "find() index = " << index << endl;
	pthread_mutex_lock(&fmutex);
	if(free_list_size == 0)
		reclaim();
	if(pidx[index] != -1)
	{
		int vi = pidx[index];
		do
		{
			if(pvalue[vi].sign == sign)
			{
				pvalue[vi].value = value;
				pthread_mutex_unlock(&fmutex);
				return;
			}
			vi = pvalue[vi].hashlink.next;
		} while(vi != -1);
	}

	int pos = get_from_free_list();
	//cout << "free slot = " << pos << endl;
	pvalue[pos].value = value;
	pvalue[pos].sign = sign;
	pvalue[pos].hashlink.next = pidx[index];
	pvalue[pos].hashlink.prev = -1;
	if(pidx[index] != -1)
		pvalue[pidx[index]].hashlink.prev = pos;
	pidx[index] = pos;
	insert_into_busy_list(pos);
	pthread_mutex_unlock(&fmutex);
}

template<class T> void CacheManager<T>::remove( const string& key)
{
	T_Sign sign;
	YeeSign::getSign(sign,key.c_str());
	int index = (sign.sign1+sign.sign2)%size;
	pthread_mutex_lock(&fmutex);
	if(pidx[index] != -1)
	{
		int vi = pidx[index];
		do
		{
			if(pvalue[vi].sign == sign)
			{
				del_from_busy_list(vi);
				insert_into_free_list(vi);
				if(pvalue[vi].hashlink.prev == -1)
					pidx[index] = pvalue[vi].hashlink.next;
				else
					pvalue[pvalue[vi].hashlink.prev].hashlink.next = pvalue[vi].hashlink.next;
				if(pvalue[vi].hashlink.next != -1)
					pvalue[pvalue[vi].hashlink.next].hashlink.prev = pvalue[vi].hashlink.prev;
				break;
			}
			vi = pvalue[vi].hashlink.next;
		} while(vi != -1);
	}
	pthread_mutex_unlock(&fmutex);
}

template<class T> bool CacheManager<T>::find( const string& key, T& value )
{
	bool ret = false;
	T_Sign sign;
	YeeSign::getSign(sign,key.c_str());
	int index = (sign.sign1+sign.sign2)%size;
	//cout << "find() index = " << index << endl;
	pthread_mutex_lock(&fmutex);
	if(pidx[index] != -1)
	{
		int vi = pidx[index];
		do
		{
			if(pvalue[vi].sign == sign)
			{
				value = pvalue[vi].value;
				del_from_busy_list(vi);
				insert_into_busy_list(vi);
				ret = true;
				break;
			}
			vi = pvalue[vi].hashlink.next;
		} while(vi != -1);
	}
	pthread_mutex_unlock(&fmutex);
	return ret;
}

#endif

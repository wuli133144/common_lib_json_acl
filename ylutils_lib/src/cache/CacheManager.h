#ifndef CACHEMANAGER_H_2008_11_11
#define CACHEMANAGER_H_2008_11_11

#include "sign/sign.h"

#include <stdio.h>
#include <pthread.h>

#include <fstream>
#include <sstream>

using namespace std;

/**
 * 简单的Cache管理器
 * 
 * (1) cache的内容是一个定长的结构体，可以通过一个字符串定位到该结构体
 * (2) 替换策略非常简单，最不常使用的被替换掉
 * (3) 线程安全
 * (4) 实现方法，采用两个双向链表，一个是hash冲突链，另一个是活动链
 * (5) add 2009.12.15 by changfeiliu，加入了日志统计功能
 * (6) 加入load,save功能
 */

//! 定义双向链表
typedef struct
{
	int next;
	int prev;
} T_Link;

template<class T> struct Item
{
	T 		value;
	T_Sign 	sign;
	T_Link 	hashlink;
	T_Link  	link;
};

inline bool operator==(const T_Sign& sign1, const T_Sign& sign2)
{
	return (sign1.sign1 == sign2.sign1) && (sign2.sign2 == sign2.sign2);
}

template<class T> class CacheManager
{
	public:
		CacheManager();
		~CacheManager();

		void init(u_int s = 1000000);

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

		/**
		 * size(): 返回cache的大小
		 */
		u_int get_size();

		void set_data_dir( const string& dir );
		bool load();
		bool save();
		
	private:
		//! 回收已经分配的cache空间
		void reclaim();

		int get_from_free_list();
		void insert_into_free_list( int pos );
		void del_from_busy_list( int pos );
		void insert_into_busy_list( int pos );
		
	private:
		u_int		size;
		Item<T>* 		pvalue;
		int*			pidx;
		T_Link		free_list;
		T_Link		busy_list;
		u_int 		free_list_size;
		pthread_mutex_t fmutex;

		// 记录统计信息
		u_int		visit_cnt;
		u_int		hit_cnt;

		string 		data_dir;

		ofstream 		fstat;
};


template<class T> CacheManager<T>::CacheManager()
{
	visit_cnt = 0;
	hit_cnt = 0;
	pidx = NULL;
	pvalue = NULL;
	data_dir = "";
	pthread_mutex_init(&fmutex,NULL);

	fstat.open("cache_stat.log");
}

template<class T> CacheManager<T>::~CacheManager()
{
	fstat.close();
	delete[] pidx;
	delete[] pvalue;
	pthread_mutex_destroy(&fmutex);
}

template<class T> u_int CacheManager<T>::get_size()
{
	return size;
}


template<class T> void CacheManager<T>::init( u_int s )
{
	if(pidx != NULL)
		delete[] pidx;
	if(pvalue != NULL)
		delete[] pvalue;
	
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
}

template<class T> void CacheManager<T>::set_data_dir( const string& dir )
{
	data_dir = dir;
}

template<class T> bool CacheManager<T>::load()
{
	if(pidx != NULL)
		delete[] pidx;
	if(pvalue != NULL)
		delete[] pvalue;
	
	string fn = data_dir + "/cache.head";
	int buf[6];
	FILE* fp = fopen(fn.c_str(),"rb");
	if(fp == NULL || fread(buf,sizeof(int),6,fp) != 6)
		goto __end;
	size = (u_int)buf[0];
	free_list_size = (u_int)buf[1];
     free_list.next = buf[2];
	free_list.prev = buf[3];
	busy_list.next = buf[4];
	busy_list.prev = buf[5];
	fclose(fp);

	pidx = new int[size];
	pvalue = new Item<T>[size];

	fn = data_dir + "/cache.idx1";
	fp = fopen(fn.c_str(),"rb");
	if(fp == NULL || fread(pidx,sizeof(int),size,fp) != size)
		goto __end;
	fclose(fp);

	fn = data_dir + "/cache.idx2";
	fp = fopen(fn.c_str(),"rb");
	if(fp == NULL || fread(pvalue,sizeof(Item<T>),size,fp) != size)
		goto __end;
	fclose(fp);
	return true;

__end:
	if(fp != NULL)
		fclose(fp);
	return false;
}

template<class T> bool CacheManager<T>::save()
{
	string fn = data_dir + "/cache.head";
	FILE* fp = fopen(fn.c_str(),"wb");
	if(fp == NULL || fwrite(&size,sizeof(u_int),1,fp) != 1 ||
			fwrite(&free_list_size,sizeof(u_int),1,fp) != 1 ||
			fwrite(&free_list,sizeof(int),2,fp) != 2 ||
			fwrite(&busy_list,sizeof(int),2,fp) != 2)
		goto __end;
	fclose(fp);

	fn = data_dir + "/cache.idx1";
	fp = fopen(fn.c_str(),"wb");
	if(fp == NULL || fwrite(pidx,sizeof(int),size,fp) != size)
		goto __end;
	fclose(fp);

	fn = data_dir + "/cache.idx2";
	fp = fopen(fn.c_str(),"wb");
	if(fp == NULL || fwrite(pvalue,sizeof(Item<T>),size,fp) != size)
		goto __end;
	fclose(fp);
	return true;
	
__end:
	if(fp != NULL)
		fclose(fp);
	return false;
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
	++visit_cnt;
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
				++hit_cnt;
				break;
			}
			vi = pvalue[vi].hashlink.next;
		} while(vi != -1);
	}
	pthread_mutex_unlock(&fmutex);

	// 记录统计结果
	if(hit_cnt > visit_cnt)
		hit_cnt = visit_cnt = 0;
	else
	{
		if(visit_cnt%500 == 0)
		{
			ostringstream oss;
			oss << "\nvisit_cnt = " << visit_cnt << endl;
			oss << "hit_cnt = " << hit_cnt << endl;
			oss << "hit ratio = " << hit_cnt/(double)visit_cnt << endl;
			fstat << oss.str();
			fstat.flush();
		}
	}
	
	return ret;
}

#endif

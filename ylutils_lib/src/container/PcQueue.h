#ifndef _PC_QUEUE_H_
#define _PC_QUEUE_H_

#include <pthread.h>
#include <queue>
#include "exception/Exception.h"
#include <errno.h>

using namespace std;

template <class ItemType>
class PcQueue
{
public:
	PcQueue();
	~PcQueue();
	
	// ������м���һ������
	void put(ItemType& t);

	// �Ӷ�����ȡ��һ���������û�У�һֱ�ȴ�
	ItemType get();

	// �Ӷ�����ȡ��һ���������û�У��ȴ�һ��ʱ��
	ItemType geto(int sec) throw(Exception);

	// �ж϶����Ƿ�Ϊ��
	bool empty();

	// ��ö����е�Ԫ�ظ���
	int size();

private:
	queue<ItemType> mQueue;
	pthread_cond_t mCond;	
	pthread_mutex_t mLock;	
};

template <class ItemType>
PcQueue<ItemType>::PcQueue()
{
	pthread_mutex_init(&mLock, NULL);
	pthread_cond_init(&mCond, NULL);
}

template <class ItemType>
PcQueue<ItemType>::~PcQueue()
{
	pthread_mutex_destroy(&mLock);
	pthread_cond_destroy(&mCond);
}
	
template <class ItemType>
void PcQueue<ItemType>::put(ItemType& t)
{
	pthread_mutex_lock(&mLock);
	mQueue.push(t);
	pthread_cond_signal(&mCond);
	pthread_mutex_unlock(&mLock);
}

template <class ItemType>
ItemType PcQueue<ItemType>::get()
{
	pthread_mutex_lock(&mLock);
	while(mQueue.empty())
		pthread_cond_wait(&mCond, &mLock);
	ItemType t = mQueue.front();
	mQueue.pop();
	pthread_mutex_unlock(&mLock);

	return t;
}

template <class ItemType>
ItemType PcQueue<ItemType>::geto(int sec) throw(Exception)
{
        struct timespec to;
    	to.tv_sec  = time(NULL) + sec;
    	to.tv_nsec = 0;

	pthread_mutex_lock(&mLock);
	while(mQueue.empty()) {
		if(pthread_cond_timedwait(&mCond, &mLock, &to) == ETIMEDOUT) {
			pthread_mutex_unlock(&mLock);
			throw Exception("PcQueue::geto timeout");
		}
	}
	ItemType t = mQueue.front();
	mQueue.pop();
	pthread_mutex_unlock(&mLock);

	return t;
}

template <class ItemType>
bool PcQueue<ItemType>::empty()
{
	pthread_mutex_lock(&mLock);
	bool status = mQueue.empty();
	pthread_mutex_unlock(&mLock);

	return status;
}

template <class ItemType>
int PcQueue<ItemType>::size()
{
	pthread_mutex_lock(&mLock);
	int size = (int)mQueue.size();
	pthread_mutex_unlock(&mLock);

	return size;
}

#endif

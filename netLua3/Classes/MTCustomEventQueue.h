/***********************************************************************************
 *Copyright(C),2010-2011,SAINTGAME Company
 *FileName: MTCustomEventQueue.H
 *Author  : young
 *Version : 1.0
 *Date    : 2015/3/18
 *Description: 线程安全的消息池，用于线程同步数据
 *Others:
 *History:        //修改历史记录列表，每条修改记录应包含修改日期、修改者及修改内容简介
     1.Date:
       Author:
       Modification:
**************************************************************************************/

#ifndef __MTCustomEventQueue_H__
#define __MTCustomEventQueue_H__

#include "cocos2d.h"
#include <mutex>

USING_NS_CC;
using namespace std;

class MTCustomEventQueue : public Ref
{
private:
	/**
	 *@name:消息名称
	 *@obj:数据对象
	 */
	typedef struct
	{
		string   name;
		const char*  pData;
	} NotificationArgs;

private:
	MTCustomEventQueue();
	~MTCustomEventQueue();

public:
	static MTCustomEventQueue* getInstance()
	{
		if (mInstance == NULL)
		{
			mInstance = new MTCustomEventQueue;
		}
		return mInstance;
	}

	void pushCustomEvents(float dt);
	void pushCustomEvent(const char* name, const char* pData);

private:
	static MTCustomEventQueue      *mInstance;
	vector<NotificationArgs>    vecNotifications;

};

/************************************
 * 全局互斥量                                                            
 ************************************/
extern mutex shareNotificationQueueLock;

/************************************
 * 互斥量的生命周期                                                            
 ************************************/
class LifeCircleMutexLocker
{
public:
	LifeCircleMutexLocker(mutex *m) : mt(m)
	{
		mt->lock();
	}

	~LifeCircleMutexLocker()
	{
		mt->unlock();
	}
private:
	mutex *mt;
};

#define LifeCircleMutexLock(_mutex_) LifeCircleMutexLocker __locker__(_mutex_)

#endif //__MTCustomEventQueue_H__

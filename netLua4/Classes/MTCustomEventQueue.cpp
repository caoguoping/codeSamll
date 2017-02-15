#include "MTCustomEventQueue.h"

mutex shareNotificationQueueLock;

MTCustomEventQueue* MTCustomEventQueue::mInstance = NULL;

MTCustomEventQueue::MTCustomEventQueue()
{
}

MTCustomEventQueue::~MTCustomEventQueue()
{
}

void MTCustomEventQueue::pushCustomEvents(float dt)
{
	LifeCircleMutexLock(&shareNotificationQueueLock);

	auto dispatcher = Director::getInstance()->getEventDispatcher();
	for (size_t i = 0; i < vecNotifications.size(); i++)
	{
		NotificationArgs arg = vecNotifications[i];
		EventCustom event(arg.name);
		//event.setUserData((void*)arg.pData);
		event.setCgpData(arg.pData);
		dispatcher->dispatchEvent(&event);
	}
	vecNotifications.clear();
}

void MTCustomEventQueue::pushCustomEvent(const char* name, const char* pData)
{
	LifeCircleMutexLock(&shareNotificationQueueLock);

	NotificationArgs arg;
	arg.name = name;
	arg.pData = pData;
	vecNotifications.push_back(arg);
}

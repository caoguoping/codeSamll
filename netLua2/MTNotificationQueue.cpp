#include "MTNotificationQueue.h"

mutex shareNotificationQueueLock;

MTNotificationQueue* MTNotificationQueue::mInstance = NULL;

MTNotificationQueue::MTNotificationQueue()
{
}

MTNotificationQueue::~MTNotificationQueue()
{
}

void MTNotificationQueue::postNotifications(float dt)
{
	LifeCircleMutexLock(&shareNotificationQueueLock);

	auto dispatcher = Director::getInstance()->getEventDispatcher();
	for (size_t i = 0; i < vecNotifications.size(); i++)
	{
		NotificationArgs arg = vecNotifications[i];
		EventCustom event(arg.name);
		event.setUserData(arg.pData);
		dispatcher->dispatchEvent(&event);
	}
	vecNotifications.clear();
}

void MTNotificationQueue::postNotification(const char* name, void* pData)
{
	LifeCircleMutexLock(&shareNotificationQueueLock);

	NotificationArgs arg;
	arg.name = name;
	arg.pData = pData;
	vecNotifications.push_back(arg);
}

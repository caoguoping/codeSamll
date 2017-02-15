
#include "ContentManager.h"

static ContentManager *instance = nullptr;

ContentManager *ContentManager::getInstance()
{
    if(instance == nullptr)
        instance = new ContentManager();
    return instance;
}

void ContentManager::test()
{
	log("hello lua bingding!");
}

ContentManager::ContentManager()
{
}

#include "HelloWorldScene.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "TTSocketClient.h"
#include "NetData.h"

USING_NS_CC;

using namespace cocostudio::timeline;

Scene* HelloWorld::createScene()
{
    auto scene = Scene::create();
    auto layer = HelloWorld::create();
    scene->addChild(layer);
    return scene;
}

void HelloWorld::eventTest(cocos2d::EventCustom* pEvent)
{
	char*   pData = (char*)pEvent->getUserData();
	DataRcv*  rcv = DataRcv::create(pData);
	unsigned short MainCmd = rcv->readWORD();
	unsigned short SubCmd = rcv->readWORD();


}

bool HelloWorld::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    auto rootNode = CSLoader::createNode("MainScene.csb");
    addChild(rootNode);
	
	// TODO: 向服务器发送选择的服务器相关信息
	TTSocketClient::getInstance()->startSocketLogin("139.196.237.203",5050);

	std::string uid = "1711514028";

	auto listener = EventListenerCustom::create("rcvDataLogin", CC_CALLBACK_1(HelloWorld::eventTest, this));
	_eventDispatcher->addEventListenerWithFixedPriority(listener, 1);


	DataSnd*   snd = DataSnd::create(1, 2);

	snd->wrDWORD(65536);
	snd->wrString(uid, 66);
	snd->wrString(uid, 64);
	snd->wrString(uid, 66);
	snd->wrString(uid, 32);
	snd->wrDWORD(3);
	snd->sendData(0);

    return true;
}

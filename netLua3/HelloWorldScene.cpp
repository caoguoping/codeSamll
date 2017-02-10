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

bool HelloWorld::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    auto rootNode = CSLoader::createNode("MainScene.csb");
    addChild(rootNode);
	
	// TODO: 向服务器发送选择的服务器相关信息
	TTSocketClient::getInstance()->startSocketLogin("139.196.237.203:5050");

	std::string uid = "1711514028";
	DataSnd*   snd = DataSnd::create(1, 2);

	snd->wrDWORD(65536);
	snd->wrString(uid, 66);
	snd->wrString(uid, 64);
	snd->wrString(uid, 66);
	snd->wrString(uid, 32);
	snd->wrByte(3);
	snd->sendData(0);

    return true;
}

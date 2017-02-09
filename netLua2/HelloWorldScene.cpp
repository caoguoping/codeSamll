#include "HelloWorldScene.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "TTSocketClient.h"

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

	// TODO: �����������ѡ��ķ����������Ϣ
	TTSocketClient::getInstance()->startSocket("139.196.237.203:5050");


    return true;
}

#include "LoadingScene.h"

#include "LoadingScene.h"

#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "GameHallScene.h"

//#include "HelloWorldScene.h"
USING_NS_CC;

using namespace cocostudio::timeline;

Scene* LoadingLayer::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = LoadingLayer::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool LoadingLayer::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    this->initData();
    return true;
}

void LoadingLayer::initData()
{
    auto rootNode = CSLoader::createNode("LoadingUI/LoadingLayer.csb");
    addChild(rootNode);
    
    this->runAction(Sequence::create(DelayTime::create(1.0),CallFunc::create([=](){
        this->toGameHall();
    }), NULL));
    
}

void LoadingLayer::toGameHall()
{
    Director::getInstance()->replaceScene(GameHallLayer::createScene());
}






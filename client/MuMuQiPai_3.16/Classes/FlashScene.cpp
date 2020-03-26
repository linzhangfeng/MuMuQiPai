#include "FlashScene.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

#include "LoadingScene.h"
USING_NS_CC;

using namespace cocostudio::timeline;

Scene* FlashLayer::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = FlashLayer::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool FlashLayer::init()
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

void FlashLayer::toLoading()
{
    Director::getInstance()->replaceScene(LoadingLayer::createScene());
}

void FlashLayer::initData()
{
    auto rootNode = CSLoader::createNode("FlashUI/FlashLayer.csb");
    addChild(rootNode);
    
    this->runAction(Sequence::create(DelayTime::create(1.0),CallFunc::create([=](){
        this->toLoading();
    }), NULL));
}

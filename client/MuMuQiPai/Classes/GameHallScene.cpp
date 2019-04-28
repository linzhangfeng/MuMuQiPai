#include "GameHallScene.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "Utils/JsUtils.h"
#include "Utils/Utils.h"
#include "HotUpdateLayer.h"
#include "CommonModel.h"
#include "Net/HttpAgent.h"
USING_NS_CC;

using namespace cocostudio::timeline;
using namespace cocos2d::ui;
Scene* GameHallLayer::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = GameHallLayer::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool GameHallLayer::init()
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

void GameHallLayer::initData()
{
    auto root = CSLoader::createNode("StudioUI/GameHallUI/GameHallLayer.csb");
    addChild(root);
    
    Button* createRoomBtn = (Button*)Utils::findNode(root,"btn_createroom");
    createRoomBtn->addClickEventListener([=](Ref*){
        JsUtils::initLoad();
        this->toRoom();
        return true;
    });
    
    
    Button* addRoomBtn = (Button*)Utils::findNode(root,"btn_addroom");
    
    std::string url = "http://127.0.0.1:9000/";
    CCHttpAgent::getInstance()->checkChangeURL(url);
    addRoomBtn->addClickEventListener([=](Ref*){
        log("enter=http=test");
        CCHttpAgent::getInstance()->sendHttpPost([=](std::string tag){
            CCHttpPacket* loginPacket = CCHttpAgent::getInstance()->packets[tag];
            
            if (this->getReferenceCount() <= 0 || this->getReferenceCount() > 10)return;
            
            if (loginPacket->status != 3)
            {
                PlatformHelper::showToast("请求用户数据失败");
                return;
            }
            
            if (loginPacket->resultIsOK())
            {
                Json::Value data = loginPacket->recvVal["resultMap"]["myInfoVO"];
                
                CCHttpAgent::getInstance()->packets.erase(tag);
                delete loginPacket;
            }
        },"get_serverinfo","","serverinfo");
        return true;
    });
}

void GameHallLayer::toRoom()
{
//    const std::string& path = "mj_lin/scripts/main.js";
    const std::string& path = "youlinqipai/scripts/main.js";
    string packName =CommonModel::getInstance()->getPackName();
    Json::Value data;
    data["entry"] = path;
    data["remoteManifestUrl"]=packName+"/version/project.manifest";
    data["remoteVersionUrl"]=packName+"/version/version.manifest";
    data["versionPath"]=(packName+"/version/");
    
    Director::getInstance()->replaceScene(HotUpdateLayer::createScene(data));
    
//    JsUtils::runScript(path);
}
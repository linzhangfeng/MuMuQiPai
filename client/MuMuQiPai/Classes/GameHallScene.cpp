#include "GameHallScene.h"
#include "HotUpdateLayer.h"
#include "CommonModel.h"
#include "Net/HttpAgent.h"
#include "UserModel.h"
#include "lin.h"
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
    //获取用户数据
    std::string url = "http://127.0.0.1:9001/";
    CCHttpAgent::getInstance()->checkChangeURL(url);
    Json::Value json;
    json["account"] = UserModel::getInstance()->getAccount();
    
    CCHttpAgent::getInstance()->sendHttpPost([=](std::string tag){
        CCHttpPacket* loginPacket = CCHttpAgent::getInstance()->packets[tag];
        
        if (this->getReferenceCount() <= 0 || this->getReferenceCount() > 10)return;
        
        if (loginPacket->status != 3)
        {
            PlatformHelper::showToast("网络连接错误，请求用户数据失败");
            return;
        }
        
        if (loginPacket->resultIsOK())
        {
            Json::Value data = loginPacket->recvData;
            //初始化用户信息
            UserModel::getInstance()->setUid(data["userid"].asInt());
            UserModel::getInstance()->setMoney(data["coins"].asInt());
            UserModel::getInstance()->setUserName(data["name"].asString());
            UserModel::getInstance()->setLevel(data["lv"].asInt());
            UserModel::getInstance()->setIp(data["ip"].asString());
            UserModel::getInstance()->setExp(data["exp"].asInt());
        }else{
            PlatformHelper::showToast("账号或密码错误");
        }
    },"login",json.toStyledString(),"login");
    
    auto root = CSLoader::createNode("StudioUI/GameHallUI/GameHallLayer.csb");
    addChild(root);
    
    Button* createRoomBtn = (Button*)Utils::findNode(root,"btn_createroom");
    createRoomBtn->addClickEventListener([=](Ref*){
        JsUtils::initLoad();
        this->toRoom();
        return true;
    });
    
    
    Button* addRoomBtn = (Button*)Utils::findNode(root,"btn_addroom");
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
//    const std::string& path = "youlinqipai/scripts/main.js";
//    string packName =CommonModel::getInstance()->getPackName();
//    Json::Value data;
//    data["entry"] = path;
//    data["remoteManifestUrl"]=packName+"/version/project.manifest";
//    data["remoteVersionUrl"]=packName+"/version/version.manifest";
//    data["versionPath"]=(packName+"/version/");
//    
//    Director::getInstance()->replaceScene(HotUpdateLayer::createScene(data));
    
    const std::string& path = "mj_lin/scripts/main.js";
    JsUtils::runScript(path);
}
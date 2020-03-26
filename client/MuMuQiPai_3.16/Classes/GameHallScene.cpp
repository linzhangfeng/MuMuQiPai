#include "GameHallScene.h"
#include "HotUpdateLayer.h"
#include "CommonModel.h"
#include "Net/HttpAgent.h"
#include "UserModel.h"
#include "lin.h"
#include "UserModel.h"
#include "Common.h"
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
    //每次进入房间获取用户信息
    getUserInfo();
    
    auto root = CSLoader::createNode("GameHallUI/GameHallLayer.csb");
    addChild(root);
    
    Button* createRoomBtn = (Button*)Utils::findNode(root,"btn_createroom");
    createRoomBtn->addClickEventListener([=](Ref*){
//        TcpClientTest::getInstance(1111)->startConnect();
        return true;
        std::string roomId = UserModel::getInstance()->getCurRoomId();
        if(roomId != ""){
            PlatformHelper::showToast("游戏重连中......");
            this->getRoomInfo();
            return true;
        }
        createRoom();
        return true;
    });
    
    
    Button* addRoomBtn = (Button*)Utils::findNode(root,"btn_addroom");
    addRoomBtn->addClickEventListener([=](Ref*){
        log("enter=http=test");
        Json::Value json;
        json["name"] = "linzhangfeng";
        TcpClientTest::getInstance(1111)->sendData(json.toStyledString());
        return true ;
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
    
    int roomId = 1111;
    TcpClientTest::getInstance(roomId)->setPost(9999);
    TcpClientTest::getInstance(roomId)->setHost("119.23.221.227");
    
    TcpClientTest::getInstance(roomId)->startConnect();
}


void GameHallLayer::createRoom()
{
    CCHttpAgent::getInstance()->checkChangeURL(CCHttpAgent::m_gameUrl);
    
    Json::Value json;
    json["userId"] = UserModel::getInstance()->getUid();
    
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
            std::string roomId = loginPacket->recvVal["roomId"].asString();
            UserModel::getInstance()->setCurRoomId(roomId);
            CCHttpAgent::getInstance()->packets.erase(tag);
            delete loginPacket;
            
            if(roomId != ""){
                PlatformHelper::showToast("正在进入游戏中......");
                this->getRoomInfo();
            }
        }
    },"create_room",json.toStyledString(),"createRoom");
}

void GameHallLayer::getRoomInfo()
{
    CCHttpAgent::getInstance()->checkChangeURL(CCHttpAgent::m_gameUrl);
    
    Json::Value json;
    json["roomId"] = UserModel::getInstance()->getCurRoomId();
    
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
            Json::Value roomData = loginPacket->recvVal["roomData"];
            std::string roomId = loginPacket->recvVal["roomId"].asString();
            UserModel::getInstance()->setCurRoomId(roomId);
            CommonModel::getInstance()->setRoomData(roomData.toStyledString());
            
            CCHttpAgent::getInstance()->packets.erase(tag);
            delete loginPacket;
            
            //进入游戏
            this->toRoom();
        }
    },"get_room_info",json.toStyledString(),"getRoomInfo");
}

void GameHallLayer::getUserInfo()
{
    //获取用户数据
    CCHttpAgent::getInstance()->checkChangeURL(CCHttpAgent::m_hallUrl);
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
            Json::Value data = loginPacket->recvVal;
            //初始化用户信息
            UserModel::getInstance()->setUid(data["userid"].asInt());
            UserModel::getInstance()->setMoney(data["coins"].asInt());
            UserModel::getInstance()->setUserName(data["name"].asString());
            UserModel::getInstance()->setLevel(data["lv"].asInt());
            UserModel::getInstance()->setIp(data["ip"].asString());
            UserModel::getInstance()->setExp(data["exp"].asInt());
            std::string roomId = data["roomId"].asString();
            if(roomId != ""){
                UserModel::getInstance()->setCurRoomId(roomId);
            }else{
                UserModel::getInstance()->setCurRoomId("");
            }
        }else{
            PlatformHelper::showToast("账号或密码错误");
        }
    },"login",json.toStyledString(),"login");
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
    JsUtils::initLoad();

    const std::string& path = "mj_lin/scripts/main.js";
    JsUtils::runScript(path);
}
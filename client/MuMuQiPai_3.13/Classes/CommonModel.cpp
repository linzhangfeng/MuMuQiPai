//
//  CommonModel.cpp
//  ZJH
//
//  Created by luochuanting on 13-6-21.
//
//

#include "CommonModel.h"
#include "GameHallScene.h"
#include "Net/HttpAgent.h"
#include "UserModel.h"
CommonModel *g_commonModel;

CommonModel::CommonModel()
{
    
}

CommonModel::~CommonModel()
{
    
}

CommonModel* CommonModel::getInstance()
{
    if (g_commonModel == NULL) {
        g_commonModel = new CommonModel();
        g_commonModel->init();
        g_commonModel->retain();
    }
    return g_commonModel;
}

void CommonModel::init()
{
    
}

void CommonModel::toHall()
{
    Director::getInstance()->replaceScene(GameHallLayer::createScene());
}

void CommonModel::hallToRoomH()
{
    Director::getInstance()->replaceScene(GameHallLayer::createScene());
}

void CommonModel::hallToRoomV()
{
    Director::getInstance()->replaceScene(GameHallLayer::createScene());
}

string CommonModel::getPackName()
{
    return "mj_lin";
}

void CommonModel::http_getRoomInfo(const MyCallBack_Str &callback,std::string roomId)
{
    CCHttpAgent::getInstance()->checkChangeURL(CCHttpAgent::getInstance()->getGameUrl());
    
    Json::Value json;
    json["roomId"] = UserModel::getInstance()->getCurRoomId();
    json["userId"] = UserModel::getInstance()->getUid();
    if(roomId != ""){
        json["roomId"] = roomId;
    }
    
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
            if(roomId != ""){
                callback("normal");
            }else{
                callback("");
            }
        }else{
            callback("");
            UserModel::getInstance()->setCurRoomId("");
        }
    },"get_room_info",json.toStyledString(),"getRoomInfo");
}

void CommonModel::http_addRoom(const MyCallBack_Str &callback,std::string roomId){
    CCHttpAgent::getInstance()->checkChangeURL(CCHttpAgent::getInstance()->getGameUrl());
    
    this->http_getRoomInfo([=](std::string str){
        if(str != "normal"){
            PlatformHelper::showToast("房间号不存在");
        }else{
            Json::Value json;
            json["userId"] = UserModel::getInstance()->getUid();
            json["roomId"] = roomId;
            
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
                        callback("normal");
                    }else{
                        callback("");
                    }
                }
            },"add_room",json.toStyledString(),"addRoom");
        }
    },roomId);
}
void CommonModel::http_register(const MyCallBack_Str &callback,std::string account,std::string password)
{
    CCHttpAgent::getInstance()->checkChangeURL(CCHttpAgent::getInstance()->getAccountUrl());
    Json::Value json;
    json["account"] = account;
    json["password"] = password;
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
            callback("normal");
        }else{
            callback("");
        }
    },"register",json.toStyledString(),"register");
}

void CommonModel::toRoom()
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



#include "lin.h"
#include "GameHallScene.h"
#include "LoginScene.h"
#include "Net/HttpAgent.h"
#include "UserModel.h"
#include "CreateRoom.hpp"
#include "Net/TcpClient.h"
//#include "HelloWorldScene.h"
#include "socketioTest.h"

using namespace cocostudio::timeline;

Scene* LoginLayer::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = LoginLayer::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool LoginLayer::init()
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

void LoginLayer::initData()
{
    auto rootNode = CSLoader::createNode("StudioUI/LoginUI/LoginLayer.csb");
    addChild(rootNode);
    
    m_rootNode = rootNode;
    m_loginNode = (Node*)Utils::findNode(m_rootNode, "LoginNode");
    m_loginNode->setVisible(false);
    m_registerNode = (Node*)Utils::findNode(m_rootNode, "RegisterNode");
    m_registerNode->setVisible(false);
    m_loadingConnectTip = (Text*)Utils::findNode(m_rootNode, "loading_connect_tip");
    m_loadingConnectTip->setVisible(false);
    
    Button* weixinLoginBtn = (Button*)Utils::findNode(m_rootNode,"btn_login_weixin");
    weixinLoginBtn->addClickEventListener([=](Ref*){
//        showDialogs(LoginLayer::Status::Login);
//        auto layer = CreateRoom::create();:9000
//        this->addChild(layer,100)
//        CCTcpClient::getInstance(1)->connect();
        SocketTest::getInstance()->connect();
        return true;
    });
    Button* youkeLoginBtn = (Button*)Utils::findNode(m_rootNode,"btn_login_youke");
    youkeLoginBtn->addClickEventListener([=](Ref*){
        showDialogs(LoginLayer::Status::Login);
        return true;
    });
    
    Button* cancleLoginBtn = (Button*)Utils::findNode(m_rootNode,"btn_cancel_login");
    cancleLoginBtn->addClickEventListener([=](Ref*){
        m_loginNode->setVisible(false);
        return true;
    });
    
    Button* registerBtn = (Button*)Utils::findNode(m_rootNode,"btn_register");
    registerBtn->addClickEventListener([=](Ref*){
        showDialogs(LoginLayer::Status::Register);
        return true;
    });
    
    Button* registerSubmitBtn = (Button*)Utils::findNode(m_registerNode,"btn_register");
    registerSubmitBtn->addClickEventListener([=](Ref*){
        TextField* accountTextField = (TextField*)Utils::findNode(m_registerNode,"user_name");
        TextField* passwordTextFiled = (TextField*)Utils::findNode(m_registerNode,"user_password");
        TextField* passwordTextFiled1 = (TextField*)Utils::findNode(m_registerNode,"user_password1");
        std::string account=accountTextField->getString();
        std::string password=passwordTextFiled->getString();
        std::string password1=passwordTextFiled1->getString();
        setRegisterStatus(LoginLayer::Status::Register_Status_start);
    
        if(account == "" || password == "" || password1 == ""){
            PlatformHelper::showToast("账号或密码不能为空！");
            return true;
        }
        if(password1 != password){
            PlatformHelper::showToast("两次密码不一致！");
        }
        
        Json::Value json;
        json["account"] = account;
        json["password"] = password;
        CCHttpAgent::getInstance()->sendHttpPost([=](std::string tag){
            CCHttpPacket* loginPacket = CCHttpAgent::getInstance()->packets[tag];
            
            if (this->getReferenceCount() <= 0 || this->getReferenceCount() > 10)return;
            
            if (loginPacket->status != 3)
            {
                setRegisterStatus(LoginLayer::Status::Register_Status_Fail);
                PlatformHelper::showToast("网络连接错误，请求用户数据失败");
                return;
            }
            
            if (loginPacket->resultIsOK())
            {
                PlatformHelper::showToast("注册成功！");
                setRegisterStatus(LoginLayer::Status::Register_Status_Ok);
            }else{
                setRegisterStatus(LoginLayer::Status::Register_Status_Fail);
                PlatformHelper::showToast("账号已经存在，请重新设置");
            }
        },"register",json.toStyledString(),"register");
        return true;
    });
    
    Button* cancleRegisterBtn = (Button*)Utils::findNode(m_registerNode,"btn_cancel_register");
    cancleRegisterBtn->addClickEventListener([=](Ref*){
        m_registerNode->setVisible(false);
        return true;
    });
    
    Button* loginBtn = (Button*)Utils::findNode(m_rootNode,"btn_login");
    TextField* accountTextField = (TextField*)Utils::findNode(m_rootNode,"user_name");
    TextField* passwordTextFiled = (TextField*)Utils::findNode(m_rootNode,"user_password");
    std::string user_name = Utils::getItem("user_name");
    std::string user_password = Utils::getItem("user_password");
    if(user_name != "")accountTextField->setString(user_name);
    if(user_password != "")passwordTextFiled->setString(user_password);
    
    loginBtn->addClickEventListener([=](Ref*){
        std::string account=accountTextField->getString();
        std::string password=passwordTextFiled->getString();
        setLoginStatus(LoginLayer::Status::Login_Status_Start);
        
        Json::Value json;
        json["account"] = account;
        json["password"] = password;
        
        if(account == "" || password == ""){
            PlatformHelper::showToast("账号或密码不能为空！");
            return true;
        }
        Utils::saveItem("user_name", account);
        Utils::saveItem("user_password", password);
        
        CCHttpAgent::getInstance()->sendHttpPost([=](std::string tag){
            CCHttpPacket* loginPacket = CCHttpAgent::getInstance()->packets[tag];
            
            if (this->getReferenceCount() <= 0 || this->getReferenceCount() > 10)return;
            
            if (loginPacket->status != 3)
            {
                setLoginStatus(LoginLayer::Status::Login_Status_Fail);
                PlatformHelper::showToast("网络连接错误，请求用户数据失败");
                return;
            }
            
            if (loginPacket->resultIsOK())
            {
                UserModel::getInstance()->setAccount(account);
                this->runAction(Sequence::create(DelayTime::create(1.0),CallFunc::create([=](){
                    this->toGameHall();
                }), NULL));
            }else{
                setLoginStatus(LoginLayer::Status::Login_Status_Fail);
                PlatformHelper::showToast("账号或密码错误");
            }
        },"login",json.toStyledString(),"login");

        return true;
    });
    
//    auto layer = CreateRoom::create();
//    this->addChild(layer,100);
}

void LoginLayer::hideLoginBtns()
{
    Button* weixinLoginBtn = (Button*)Utils::findNode(m_rootNode,"btn_login_weixin");
    Button* youkeLoginBtn = (Button*)Utils::findNode(m_rootNode,"btn_login_weixin");
    weixinLoginBtn->setVisible(false);
    youkeLoginBtn->setVisible(false);
}

void LoginLayer::showLoginBtns(){
    Button* weixinLoginBtn = (Button*)Utils::findNode(m_rootNode,"btn_login_weixin");
    Button* youkeLoginBtn = (Button*)Utils::findNode(m_rootNode,"btn_login_weixin");
    weixinLoginBtn->setVisible(true);
    youkeLoginBtn->setVisible(true);
}

void LoginLayer::setLoginStatus(Status type)
{
    if(type == LoginLayer::Status::Login_Status_Start){
        m_loginNode->setVisible(false);
        hideLoginBtns();
        m_loadingConnectTip->setVisible(true);
    }else if(type == LoginLayer::Status::Login_Status_Ok){
        m_loginNode->setVisible(false);
        hideLoginBtns();
        m_loadingConnectTip->setVisible(true);
    }else if(type == LoginLayer::Status::Login_Status_Fail){
        m_loginNode->setVisible(false);
        showLoginBtns();
        m_loadingConnectTip->setVisible(false);
    }
}

void LoginLayer::setRegisterStatus(Status type)
{
    if(type == LoginLayer::Status::Register_Status_start){
        m_loginNode->setVisible(false);
        m_registerNode->setVisible(false);
        m_loadingConnectTip->setVisible(true);
        hideLoginBtns();
    }else if(type == LoginLayer::Status::Register_Status_Ok){
        m_loginNode->setVisible(false);
        m_registerNode->setVisible(false);
        m_loadingConnectTip->setVisible(false);
        showLoginBtns();
    }else if(type == LoginLayer::Status::Register_Status_Fail){
        m_loginNode->setVisible(false);
        m_registerNode->setVisible(false);
        m_loadingConnectTip->setVisible(false);
        showLoginBtns();
    }
}

void LoginLayer::showDialogs(Status type)
{
    if(type == LoginLayer::Status::Register){
        m_loadingConnectTip->setVisible(false);
        m_registerNode->setVisible(true);
        m_loginNode->setVisible(false);
    }else if(type == LoginLayer::Status::Login){
        m_loadingConnectTip->setVisible(false);
        m_registerNode->setVisible(false);
        m_loginNode->setVisible(true);
    }
}

void LoginLayer::toGameHall()
{
    Director::getInstance()->replaceScene(GameHallLayer::createScene());
}
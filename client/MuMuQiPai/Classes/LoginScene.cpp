#include "lin.h"
#include "GameHallScene.h"
#include "LoginScene.h"
#include "Net/HttpAgent.h"
//#include "HelloWorldScene.h"

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
    m_loadingConnectTip = (Text*)Utils::findNode(m_rootNode, "loading_connect_tip");
    m_loadingConnectTip->setVisible(false);
    
    Button* weixinLoginBtn = (Button*)Utils::findNode(m_rootNode,"btn_login_weixin");
    weixinLoginBtn->addClickEventListener([=](Ref*){
        m_loginNode->setVisible(true);
        return true;
    });
    Button* youkeLoginBtn = (Button*)Utils::findNode(m_rootNode,"btn_login_youke");
    youkeLoginBtn->addClickEventListener([=](Ref*){
        m_loginNode->setVisible(true);
        return true;
    });
    
    Button* cancleLoginBtn = (Button*)Utils::findNode(m_rootNode,"btn_cancel_login");
    cancleLoginBtn->addClickEventListener([=](Ref*){
        m_loginNode->setVisible(false);
        return true;
    });
    
    Button* loginBtn = (Button*)Utils::findNode(m_rootNode,"btn_login");
    loginBtn->addClickEventListener([=](Ref*){
        TextField* accountTextField = (TextField*)Utils::findNode(m_rootNode,"user_name");
        TextField* passwordTextFiled = (TextField*)Utils::findNode(m_rootNode,"user_password");
        std::string account=accountTextField->getString();
        std::string password=passwordTextFiled->getString();
        setLoginStatus(LoginLayer::LoginStatus::LoginStatus_Start);
        Json::Value json;
        
        json["account"] = account;
        json["password"] = password;
        
        if(account == "" || password == ""){
            PlatformHelper::showToast("账号或密码不能为空！");
            return true;
        }
        
        CCHttpAgent::getInstance()->sendHttpPost([=](std::string tag){
            CCHttpPacket* loginPacket = CCHttpAgent::getInstance()->packets[tag];
            
            if (this->getReferenceCount() <= 0 || this->getReferenceCount() > 10)return;
            
            if (loginPacket->status != 3)
            {
                setLoginStatus(LoginLayer::LoginStatus::LoginStatus_Fail);
                PlatformHelper::showToast("网络连接错误，请求用户数据失败");
                return;
            }
            
            if (loginPacket->resultIsOK())
            {
                this->runAction(Sequence::create(DelayTime::create(1.0),CallFunc::create([=](){
                    this->toGameHall();
                }), NULL));
            }else{
                setLoginStatus(LoginLayer::LoginStatus::LoginStatus_Fail);
                PlatformHelper::showToast("账号或密码错误");
            }
        },"login",json.toStyledString(),"login");

        return true;
    });
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

void LoginLayer::setLoginStatus(LoginStatus type)
{
    if(type == LoginStatus::LoginStatus_Start){
        m_loginNode->setVisible(false);
        hideLoginBtns();
        m_loadingConnectTip->setVisible(true);
    }else if(type == LoginStatus::LoginStatus_Ok){
        m_loginNode->setVisible(false);
        hideLoginBtns();
        m_loadingConnectTip->setVisible(true);
    }else if(type == LoginStatus::LoginStatus_Fail){
        m_loginNode->setVisible(false);
        showLoginBtns();
        m_loadingConnectTip->setVisible(false);
    }
}

void LoginLayer::toGameHall()
{
    Director::getInstance()->replaceScene(GameHallLayer::createScene());
}
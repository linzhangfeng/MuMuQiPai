
#ifndef __LoginLayer_H__
#define __LoginLayer_H__

#include "lin.h"
class LoginLayer : public cocos2d::Layer
{
public:
    enum Status {
        Login = 0,
        Register,
        Login_Status_Start,
        Login_Status_Ok,
        Login_Status_Fail,
        Register_Status_start,
        Register_Status_Ok,
        Register_Status_Fail,
    };

    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();
    
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // implement the "static create()" method manually
    CREATE_FUNC(LoginLayer);
    
    void initData();
    void toGameHall();
    void hideLoginBtns();
    void showLoginBtns();
    void showDialogs(Status type);
    void setLoginStatus(Status type);
    void setRegisterStatus(Status type);
private:
    Node* m_rootNode;
    Node* m_loginNode;
    Node* m_registerNode;
    Text* m_loadingConnectTip;
};

#endif // __HELLOWORLD_SCENE_H__

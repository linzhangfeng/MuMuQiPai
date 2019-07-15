#include "AppDelegate.h"
//#include "LoadingScene.h"
//#include "HelloWorldScene.h"
//#include "HotUpdateLayer.h"
#include "FlashScene.h"
#include "GameHallScene.h"
#include "LoadingScene.h"
#include "LoginScene.h"
#include "Net/HttpAgent.h"
USING_NS_CC;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs()
{
    //set OpenGL context attributions,now can only set six attributions:
    //red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = GLViewImpl::createWithRect("MuMuQiPai", Rect(0, 0, 1280, 720));
        director->setOpenGLView(glview);
    }

    director->getOpenGLView()->setDesignResolutionSize(1280, 720, ResolutionPolicy::SHOW_ALL);

    // turn on display FPS
    director->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);
    FileUtils::getInstance()->addSearchPath("/Users/linzhangfeng/Desktop/linzhangfeng/svn/MuMuQiPai/client/MuMuQiPai/cocos2d/cocos/scripting/js-bindings");
    FileUtils::getInstance()->addSearchPath("StudioUI/");
    FileUtils::getInstance()->addSearchPath("/Users/linzhangfeng/Desktop/linzhangfeng/svn/MuMuQiPai/client/JS");
//    FileUtils::getInstance()->addSearchPath("/Users/linzhangfeng/Desktop/linzhangfeng/svn/MuMuQiPai/client/JS/output");
    
    std::string url = "http://127.0.0.1:9000/";
    CCHttpAgent::getInstance()->checkChangeURL(url);
    CCHttpAgent::getInstance()->setUrl(url);
//    const Json::Value& dat = {};
//    auto scene = GameHallLayer::createScene(dat);
//    auto scene = GameHallLayer::createScene();
//        auto scene = LoadingLayer::createScene();
    auto scene = LoginLayer::createScene();
    // run
    director->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
#include "AppDelegate.h"
#include "HelloWorldScene.h"
#include "FlashScene.h"
#include "GameHallScene.h"
#include "LoadingScene.h"
#include "LoginScene.h"
#include "Net/HttpAgent.h"
#include "Net/SocketClient.h"

// #define USE_AUDIO_ENGINE 1
// #define USE_SIMPLE_AUDIO_ENGINE 1

#if USE_AUDIO_ENGINE && USE_SIMPLE_AUDIO_ENGINE
#error "Don't use AudioEngine and SimpleAudioEngine at the same time. Please just select one in your game!"
#endif

#if USE_AUDIO_ENGINE
#include "audio/include/AudioEngine.h"
using namespace cocos2d::experimental;
#elif USE_SIMPLE_AUDIO_ENGINE
#include "audio/include/SimpleAudioEngine.h"
using namespace CocosDenshion;
#endif

USING_NS_CC;

static cocos2d::Size designResolutionSize = cocos2d::Size(1280, 720);
static cocos2d::Size smallResolutionSize = cocos2d::Size(480, 320);
static cocos2d::Size mediumResolutionSize = cocos2d::Size(1024, 768);
static cocos2d::Size largeResolutionSize = cocos2d::Size(2048, 1536);

AppDelegate::AppDelegate()
{
}

AppDelegate::~AppDelegate() 
{
#if USE_AUDIO_ENGINE
    AudioEngine::end();
#elif USE_SIMPLE_AUDIO_ENGINE
    SimpleAudioEngine::end();
#endif
}

// if you want a different context, modify the value of glContextAttrs
// it will affect all platforms
void AppDelegate::initGLContextAttrs()
{
    // set OpenGL context attributes: red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

// if you want to use the package manager to install more packages,  
// don't modify or remove this function
static int register_all_packages()
{
    return 0; //flag for packages manager
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
        glview = GLViewImpl::createWithRect("MuMuQiPai_3.15", cocos2d::Rect(0, 0, designResolutionSize.width, designResolutionSize.height));
#else
        glview = GLViewImpl::create("MuMuQiPai_3.15");
#endif
        director->setOpenGLView(glview);
    }

    // turn on display FPS
    director->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0f / 60);

    // Set the design resolution
    glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::NO_BORDER);
    auto frameSize = glview->getFrameSize();
    // if the frame's height is larger than the height of medium size.
    if (frameSize.height > mediumResolutionSize.height)
    {        
        director->setContentScaleFactor(MIN(largeResolutionSize.height/designResolutionSize.height, largeResolutionSize.width/designResolutionSize.width));
    }
    // if the frame's height is larger than the height of small size.
    else if (frameSize.height > smallResolutionSize.height)
    {        
        director->setContentScaleFactor(MIN(mediumResolutionSize.height/designResolutionSize.height, mediumResolutionSize.width/designResolutionSize.width));
    }
    // if the frame's height is smaller than the height of medium size.
    else
    {        
        director->setContentScaleFactor(MIN(smallResolutionSize.height/designResolutionSize.height, smallResolutionSize.width/designResolutionSize.width));
    }

    register_all_packages();

    
    FileUtils::getInstance()->addSearchPath("/Users/linzhangfeng/Desktop/linzhangfeng/svn/MuMuQiPai/client/MuMuQiPai_3.15/cocos2d/cocos/scripting/js-bindings");
    FileUtils::getInstance()->addSearchPath("StudioUI/");
    FileUtils::getInstance()->addSearchPath("/Users/linzhangfeng/Desktop/linzhangfeng/svn/MuMuQiPai/client/JS");
    //    FileUtils::getInstance()->addSearchPath("/Users/linzhangfeng/Desktop/linzhangfeng/svn/MuMuQiPai/client/JS/output");
    
    if(0){ //本地调试
        CCHttpAgent::m_accountUrl = "http://127.0.0.1:9000/";
        CCHttpAgent::m_hallUrl = "http://127.0.0.1:9001/";
        CCHttpAgent::m_gameUrl = "http://127.0.0.1:9003/";
        SocketClient::m_url = "127.0.0.1:10000";
    }else{
        CCHttpAgent::m_accountUrl = "http://119.23.221.227:9000/";
        CCHttpAgent::m_hallUrl = "http://119.23.221.227:9001/";
        CCHttpAgent::m_gameUrl = "http://119.23.221.227:9003/";
        SocketClient::m_url = "119.23.221.227:10000";
    }
    
    CCHttpAgent::getInstance()->checkChangeURL(CCHttpAgent::m_accountUrl);
    CCHttpAgent::getInstance()->setUrl(CCHttpAgent::m_accountUrl);
    //    const Json::Value& dat = {};
    //    auto scene = GameHallLayer::createScene(dat);
    //    auto scene = GameHallLayer::createScene();
//        auto scene = LoadingLayer::createScene();
    auto scene = LoginLayer::createScene();
//    auto scene = HelloWorld::createScene();
    // run
    director->runWithScene(scene);
    return true;
}

// This function will be called when the app is inactive. Note, when receiving a phone call it is invoked.
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

#if USE_AUDIO_ENGINE
    AudioEngine::pauseAll();
#elif USE_SIMPLE_AUDIO_ENGINE
    SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
    SimpleAudioEngine::getInstance()->pauseAllEffects();
#endif
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

#if USE_AUDIO_ENGINE
    AudioEngine::resumeAll();
#elif USE_SIMPLE_AUDIO_ENGINE
    SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
    SimpleAudioEngine::getInstance()->resumeAllEffects();
#endif
}

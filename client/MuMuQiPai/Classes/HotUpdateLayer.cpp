//
//  HotUpdateLayer.cpp
//  ZJH
//
//  Created by luochuanting on 13-6-21.
//
//

#include "HotUpdateLayer.h"
#include "lin.h"
Scene *HotUpdateLayer::createScene(const Json::Value& dat)
{
    Scene *scene = Scene::create();
    auto layer = HotUpdateLayer::create();
    scene->addChild(layer,1,"update");
    layer->m_data = dat;
    layer->m_runJS = dat["entry"].asString();
    layer->onLoadEnd(true);
    
    return scene;
}

HotUpdateLayer::HotUpdateLayer()
{
    
}

HotUpdateLayer::~HotUpdateLayer()
{
    
}

bool HotUpdateLayer::init()
{
    if (!Layer::init())
    {
        return false;
    }
    Size size = cocos2d::Director::getInstance()->getWinSize();
    auto glview = cocos2d::Director::getInstance()->getOpenGLView();
    if(size.width < size.height)
    {
        glview->setDesignResolutionSize(720, 1280, ResolutionPolicy::EXACT_FIT);
    }else{
        glview->setDesignResolutionSize(1280, 720, ResolutionPolicy::EXACT_FIT);
    }
    SpriteFrameCache::getInstance()->removeSpriteFrames();
    Director::getInstance()->getTextureCache()->removeAllTextures();
    
    auto rootNode = CSLoader::createNode("StudioUI/LoadingUI/LoadingLayer_0.csb");
    addChild(rootNode);
    
    m_tTip = (Text*)Utils::findNode(rootNode,"tip");
    m_sDots = (Sprite*)Utils::findNode(rootNode,"loading_icon");
    m_lLoadingBar = (LoadingBar*)Utils::findNode(rootNode,"loading_bar_fg");
    m_tProgressText = (Text*)Utils::findNode(rootNode,"progressText");
    m_sLoadingGameIcon = (Sprite*)Utils::findNode(rootNode,"loading_game_icon");
    m_iLoadingBarBg = (ImageView*)Utils::findNode(rootNode,"loading_bar_bg");
    
    m_sLoadingGameIcon->setVisible(false);
    m_iLoadingBarBg->setVisible(false);
    
    m_sDots->runAction(RepeatForever::create(RotateBy::create(3, 360)));
    
    m_index =0;
    m_aAssetsMEx = nullptr;
    m_aAssetsMExListener = nullptr;
    m_bFrist = false;
    return true;
}

void HotUpdateLayer::onLoadEnd(bool is)
{
    if(is){
        if( m_index >= 1 ){
            if(m_aAssetsMEx){
                CC_SAFE_RELEASE_NULL(m_aAssetsMEx);
            }
            std::string manifestPath = m_data["versionPath"].asString()+"project.manifest";
            std::string storagePath = Utils::getSavePath();
            if(!FileUtils::getInstance()->isFileExist(manifestPath)){
                m_bFrist = true;
                this->m_tTip->setString(StringUtils::format("首次进入，游戏正在安装中(%d/%d)...",1,1) );
            }
            
            m_aAssetsMEx = AssetsManagerEx::create(manifestPath, storagePath);
            m_aAssetsMEx->retain();
            m_index++;
            
            startDownloadCallback();
        }else{
            this->runAction(Sequence::create(DelayTime::create(0.5),CallFunc::create([=](){
                JsUtils::initLoad();
                JsUtils::runScript(m_runJS);
            }), NULL));
        }
    }else{
        PlatformHelper::showToast("资源更新失败，请重试！");
    }
};
void HotUpdateLayer::startDownloadCallback()
{
    if (!m_aAssetsMEx->getLocalManifest()->isLoaded())
    {
        CCLOG("Fail to update assets, step skipped.");
        onLoadEnd(true);
    }
    else
    {
        if(m_aAssetsMExListener)
        {
            
            Director::getInstance()->getEventDispatcher()->removeEventListener(m_aAssetsMExListener);
            
        }
        m_aAssetsMExListener = cocos2d::extension::EventListenerAssetsManagerEx::create(m_aAssetsMEx, [=](EventAssetsManagerEx* event){
            
            CCLOG("check update CB:%d",(int)event->getEventCode());
            bool isFail =false;
            bool restart =false;
            
            switch (event->getEventCode())
            {
                case EventAssetsManagerEx::EventCode::ERROR_NO_LOCAL_MANIFEST:
                {
                    CCLOG("No local manifest file found, skip assets update.");
                    restart = true;
                }
                    break;
                case EventAssetsManagerEx::EventCode::ERROR_DOWNLOAD_MANIFEST:
                case EventAssetsManagerEx::EventCode::ERROR_PARSE_MANIFEST:
                {
                    CCLOG("Fail to download manifest file, update skipped.");
                    isFail = true;
                    
                }
                    break;
                case EventAssetsManagerEx::EventCode::ALREADY_UP_TO_DATE:
                {
                    
                    CCLOG("Update finished. %s", event->getMessage().c_str());
                    restart = true;
                    
                }
                    break;
                case EventAssetsManagerEx::EventCode::NEW_VERSION_FOUND:
                {
                    Director::getInstance()->getEventDispatcher()->removeEventListener(m_aAssetsMExListener);
                    
                    m_aAssetsMExListener = cocos2d::extension::EventListenerAssetsManagerEx::create(m_aAssetsMEx, [=](EventAssetsManagerEx* event){
                        static int failCount = 0;
                        bool updateFail = false;
                        bool restart =false;
                        bool back = false;
                        CCLOG("NEW_VERSION_FOUND CB:%d",(int)event->getEventCode());
                        switch (event->getEventCode())
                        {
                            case EventAssetsManagerEx::EventCode::ERROR_NO_LOCAL_MANIFEST:
                            {
                                CCLOG("No local manifest file found, skip assets update.");
                                restart=true;
                            }
                                break;
                            case EventAssetsManagerEx::EventCode::UPDATE_PROGRESSION:
                            {
                                std::string assetId = event->getAssetId();
                                float percent = event->getPercentByFile();
                                std::string str;
                                if (assetId == AssetsManagerEx::VERSION_ID)
                                {
                                    str = "";
                                }
                                else if (assetId == AssetsManagerEx::MANIFEST_ID)
                                {
                                    str = "";
                                }
                                else
                                {
                                    if(!m_bFrist)
                                    {
                                        m_tTip->setString(StringUtils::format("发现新版本，游戏正在更新中(%d/%d)...",m_index,m_data["list"].size()) );
                                    }
                                    else
                                    {
                                        
                                        m_tTip->setString(StringUtils::format("首次进入，游戏正在安装中(%d/%d)...",m_index,m_data["list"].size()) );
                                    }
                                    str = StringUtils::format("%.2f%%", percent);
                                    m_lLoadingBar->setPercent(percent);
                                    m_iLoadingBarBg->setVisible(true);
                                    
                                    CCLOG("%.2f Percent", percent);
                                }
                                if (m_tProgressText != nullptr)m_tProgressText->setString(str);
                            }
                                break;
                            case EventAssetsManagerEx::EventCode::ERROR_DOWNLOAD_MANIFEST:
                            case EventAssetsManagerEx::EventCode::ERROR_PARSE_MANIFEST:
                            {
                                CCLOG("Fail to download manifest file, update skipped.");
                                updateFail=true;
                            }
                                break;
                            case EventAssetsManagerEx::EventCode::ALREADY_UP_TO_DATE:
                            case EventAssetsManagerEx::EventCode::UPDATE_FINISHED:
                            {
                                CCLOG("Update finished. %s", event->getMessage().c_str());
                                std::string assetId = event->getAssetId();
                                restart=true;
                                
                                if (assetId == AssetsManagerEx::VERSION_ID)
                                {
                                }
                                else if (assetId == AssetsManagerEx::MANIFEST_ID)
                                {
                                }
                                else
                                {
                                    
                                    if(m_index < 1)
                                    {
                                        // _tip->setString("加载中。。。");
                                    }
                                    else
                                    {
                                        m_tTip->setString("资源更新成功，正在进入游戏");
                                    }
                                   m_tProgressText->setString("");
                                }
                                
                            }
                                break;
                            case EventAssetsManagerEx::EventCode::UPDATE_FAILED:
                            {
                                CCLOG("Update failed. %s", event->getMessage().c_str());
                                
                                failCount++;
                                if (failCount < 5)
                                {
                                    m_aAssetsMEx->downloadFailedAssets();
                                }
                                else
                                {
                                    CCLOG("Reach maximum fail count, exit update process");
                                    failCount = 0;
                                    back=true;
                                    
                                }
                            }
                                break;
                            case EventAssetsManagerEx::EventCode::ERROR_UPDATING:
                            {
                                CCLOG("Asset %s : %s", event->getAssetId().c_str(), event->getMessage().c_str());
                            }
                                break;
                            case EventAssetsManagerEx::EventCode::ERROR_DECOMPRESS:
                            {
                                CCLOG("%s", event->getMessage().c_str());
                            }
                                break;
                            default:
                                break;
                        }
                        if(updateFail)
                        {
                            this->onLoadEnd(false);
                        }
                        else if(back)
                        {
                            this->onLoadEnd(false);
                        }
                        else if(restart)
                        {
                            this->onLoadEnd(true);
                        }
                        
                    });
                    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(m_aAssetsMExListener, 1);
                    m_aAssetsMEx->update();
                }
                    break;
                default:
                    break;
            }
            if(isFail)
            {
                this->onLoadEnd(false);
            }
            else if(restart)
            {
                this->onLoadEnd(true);
            }
            
        });
        Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(m_aAssetsMExListener, 1);
        m_aAssetsMEx->checkUpdate();
        auto size = Director::getInstance()->getWinSize();
    }
}

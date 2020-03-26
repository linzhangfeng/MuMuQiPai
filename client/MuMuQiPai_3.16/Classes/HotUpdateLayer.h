//
//  ZJHModel.h
//  ZJH
//
//
//

#ifndef __ZJH__HotUpdateLayer__
#define __ZJH__HotUpdateLayer__

#include <iostream>
#include <string>
#include <map>

#include "lin.h"

class HotUpdateLayer : public Layer
{
private:
    Text* m_tProgressText;
    Text* m_tTip;
    LoadingBar* m_lLoadingBar;
    Sprite* m_sDots;
    Sprite* m_sLoadingGameIcon;
    ImageView* m_iLoadingBarBg;
    string m_runJS;
    cocos2d::extension::AssetsManagerEx* m_aAssetsMEx;
    cocos2d::extension::EventListenerAssetsManagerEx* m_aAssetsMExListener;
    int m_index;
    bool m_bFrist;
public:
    static cocos2d::Scene *createScene(const Json::Value& dat);
    
    HotUpdateLayer();
    virtual ~HotUpdateLayer();
    virtual bool init();
    void onLoadEnd(bool is);
    void startDownloadCallback();
    CREATE_FUNC(HotUpdateLayer);
    
    Json::Value m_data;
};
#endif /* defined(__ZJH__ZJHModel__) */

#pragma once
#include "cocos2d.h"

class JsUtils : public cocos2d::Ref
{
public:
    JsUtils();
    virtual ~JsUtils();
    
    static JsUtils* getInstance();
    
    //执行js脚本
    static void runScript(const std::string& file);
    //清空js虚拟机
    static void resetJsVM();
    static void initLoad();
};
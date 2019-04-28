#ifndef __ZJH__LIN_H__
#define __ZJH__LIN_H__

#include "json.h"
#include "Utils/JsUtils.h"
#include "Utils/Utils.h"
#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"
#include "SimpleAudioEngine.h"
#include "cocostudio/CocoStudio.h"
#include "PlatformHelper.h"
using namespace std;
using namespace cocos2d;
using namespace CocosDenshion;
using namespace cocos2d::ui;
using namespace cocostudio::timeline;
USING_NS_CC_EXT;

typedef std::function<void()> MyCallBack_Null;
typedef std::function<void(std::string v)> MyCallBack_Str;
typedef std::function<void(int v)> MyCallBack_Int;
typedef std::function<void(Node* node)> MyCallBack_Node;
typedef std::function<void(Json::Value _json)> MyCallBack_Json;

#endif
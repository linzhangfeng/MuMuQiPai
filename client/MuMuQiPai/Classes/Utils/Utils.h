#pragma once
#include "cocos2d.h"
using namespace std;
using namespace cocos2d;
using namespace cocos2d::ui;

class Utils
{
public:
    Utils();
    virtual ~Utils();
    
    static Utils* getInstance();
    
    static Node* findNode(Node*,const string& key);
    static long long getCurrentTime();
    static long long getCurrentSecend();
};
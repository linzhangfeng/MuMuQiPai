#pragma once
#include "cocos2d.h"

#include "json/json.h"

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
    static std::string getSavePath();
    static void saveItem(const char* itemName,std::string strValue);
    static std::string getItem(const char* itemName);
    
    static Json::Value ReadFile(string filename)
    {
        Json::Value value_date;
        if(filename.length() > 0)
        {
            std::string data = FileUtils::getInstance()->getStringFromFile(filename.c_str());
            if (data != "")
            {
                Json::Reader read;
                read.parse(data, value_date);
            }
        }
        return value_date;
    }
    
    static Json::Value ParseJsonStr(string str)
    {
        Json::Value value_date;
        if(str.length() > 0)
        {
            if (str != "")
            {
                Json::Reader read;
                read.parse(str, value_date);
            }
        }
        return value_date;
    }
};


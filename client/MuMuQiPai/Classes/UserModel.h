//
//  ZJHModel.h
//  ZJH
//
//  Created by luochuanting on 13-6-21.
//
//

#ifndef __LIN__UserModel__
#define __LIN__UserModel__

#include <iostream>
#include <string>
#include <map>

#include "cocos2d.h"
#include "SimpleAudioEngine.h"

using namespace std;
using namespace cocos2d;
using namespace CocosDenshion;

class UserModel : public Ref
{
private:
    int m_uid;
    int m_money;
    std::string m_imsi;
    std::string m_imei;
    std::string m_channel;
public:
    UserModel();
    virtual ~UserModel();
    /** Return the shared instance **/
    static UserModel *getInstance();
    
    int getUid()
    {
        return m_uid;
    }
    
    void setUid(int uid)
    {
        m_uid = uid;
    }
    
    int getMoney()
    {
        return m_money;
    }
    
    void setMoney(int money)
    {
        m_money = money;
    }
    
    std::string getImsi(){
        return m_imsi;
    }
    
    void setImsi(std::string imsi){
        m_imsi = imsi;
    }
    
    std::string getChannel()
    {
        return m_channel;
    }
    
    void setChannel(std::string channel)
    {
        m_channel = channel;
    }
};
#endif /* defined(__ZJH__ZJHModel__) */

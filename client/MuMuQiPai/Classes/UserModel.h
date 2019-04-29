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
    std::string m_account;
    std::string m_userName;
    std::string m_ip;
    int m_sex;
    int m_level;
    int m_exp;
public:
    UserModel();
    virtual ~UserModel();
    /** Return the shared instance **/
    static UserModel *getInstance();
    
    int getUid(){return m_uid;}
    void setUid(int uid){m_uid = uid;}
    
    int getLevel(){return m_level;}
    void setLevel(int lv){m_level = lv;}
    
    int getSex(){return m_sex;}
    void setSex(int sex){m_sex = sex;}
    
    int getMoney(){return m_money;}
    void setMoney(int money){m_money = money;}
    
    std::string getImsi(){return m_imsi;}
    void setImsi(std::string imsi){m_imsi = imsi;}
    
    std::string getChannel(){return m_channel;}
    void setChannel(std::string channel){m_channel = channel;}
    
    std::string getAccount(){return m_account;}
    void setAccount(std::string account){m_account = account;}
    
    std::string getUserName(){return m_userName;}
    void setUserName(std::string userName){m_userName = userName;}
    
    std::string getIp(){return m_ip;}
    void setIp(std::string ip){m_ip = ip;}
    
    int getExp(){return m_exp;}
    void setExp(int exp){m_exp = exp;}
};
#endif /* defined(__ZJH__ZJHModel__) */

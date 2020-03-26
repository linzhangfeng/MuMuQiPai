//
//  ZJHModel.h
//  ZJH
//
//  Created by luochuanting on 13-6-21.
//
//

#ifndef __ZJH__ZJHModel__
#define __ZJH__ZJHModel__

#include <iostream>
#include <string>
#include <map>

#include "json/json.h"

#include "cocos2d.h"
#include "SimpleAudioEngine.h"

using namespace std;
using namespace cocos2d;
using namespace CocosDenshion;

class CommonModel : public Ref
{
private:
    std::string m_roomData;
public:
    CommonModel();
    virtual ~CommonModel();
    /** Return the shared instance **/
    static CommonModel *getInstance();
    void init();
    void toHall();
    void hallToRoomV();
    void hallToRoomH();
    string getPackName();
    std::string getRoomData(){
        return m_roomData;
    }
    void setRoomData(std::string roomData){
        m_roomData = m_roomData;
    }
};
#endif /* defined(__ZJH__ZJHModel__) */

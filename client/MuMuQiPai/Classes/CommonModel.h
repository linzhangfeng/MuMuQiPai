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

#include "cocos2d.h"
#include "SimpleAudioEngine.h"

using namespace std;
using namespace cocos2d;
using namespace CocosDenshion;

class CommonModel : public Ref
{
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
};
#endif /* defined(__ZJH__ZJHModel__) */

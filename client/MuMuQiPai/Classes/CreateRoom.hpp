//
//  CreateRoom.hpp
//  ZJH
//
//  Created by apple on 16/7/13.
//
//

#ifndef CreateRoom_hpp
#define CreateRoom_hpp

#include <stdio.h>

#include "lin.h"

class  CreateRoom: public Layer
{
private:
    Json::Value m_showData;
    ListView *m_listPackageBg;
    ListView *m_listRoomConfigBg;
    std::map<int, std::vector<Layout*>> m_butMap;
    int m_baoIndex;
public:
    bool initData();
    void showUI();
    void test();
    void initTextData();
    Layout* selectBtnDraw(string titleStr, bool selected, int cellType);
    void updateRoomConfigView();
    void updatePackageConfigView();
    void selectBtnOn(Ref*ref);
public:
    
    virtual bool init();
    CREATE_FUNC(CreateRoom);
};


#endif /* CreateRoom_hpp */

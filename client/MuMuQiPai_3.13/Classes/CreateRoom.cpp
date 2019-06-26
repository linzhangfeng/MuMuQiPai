//
//  CreateRoom.cpp
//  ZJH
//
//  Created by apple on 16/7/13.
//
//

#include "CreateRoom.hpp"

bool CreateRoom::init()
{
    
    if (!Layer::init())
    {
        return false;
    }
    Size size = Director::getInstance()->getWinSize();
    initData();
    return true;
}
bool CreateRoom::initData()
{
    this->initTextData();
    
    auto rootNode = CSLoader::createNode("StudioUI/CreateRoomUI/CreateRoomLayer.csb");
    addChild(rootNode);
    
    m_listPackageBg = (ListView*)Utils::findNode(rootNode,"listView_package");
    m_listRoomConfigBg = (ListView*)Utils::findNode(rootNode,"listView_roomconfig");
    
    Button* createRoomBtn = (Button*)Utils::findNode(rootNode,"btn_create_room");
    createRoomBtn->addClickEventListener([=](Ref*){
        this->removeFromParent();
        return true;
    });
    
    Button* closeBtn = (Button*)Utils::findNode(rootNode,"btn_close");
    closeBtn->addClickEventListener([=](Ref*){
        this->removeFromParent();
        return true;
    });
    m_baoIndex = 0;
    this->updatePackageConfigView();
    this->updateRoomConfigView();
//    this->test();
    return true;
}


void CreateRoom::updateRoomConfigView()
{
    m_butMap.clear();
    auto listSize = m_listPackageBg->getContentSize();
    auto date = m_showData[m_baoIndex]["gameCustomPackageList"];
    for(int i = 0 ; i < date.size();i++)
    {
        auto tempObject = date[i];
        std::string classifyName = tempObject["classifyName"].asString();
        std::vector<Layout*> tempBtn;
        Text* countText  = Text::create(classifyName, "", 45);
        countText->setAnchorPoint(Vec2(0, 1));
        countText->setTextHorizontalAlignment(cocos2d::TextHAlignment::LEFT);
        countText->setPosition(Vec2(50, m_listRoomConfigBg->getContentSize().height - 45));
        countText->setTextColor(Color4B(197, 151, 70, 255));
        
        Json::Value gameCustomClassifyVOList = tempObject["gameCustomPlayValInfoList"];
        
        for(int k = 0; k < gameCustomClassifyVOList.size();k++){
            Json::Value tempObject1 = gameCustomClassifyVOList[k];
            std::string playName = tempObject1["playName"].asString();
            bool visible = tempObject1["visible"].asBool();
            auto btn = this->selectBtnDraw(playName, visible, 1);
            btn->setAnchorPoint(Vec2(0, 1));
            btn->setTag(100*i+k);
            btn->addClickEventListener(CC_CALLBACK_1(CreateRoom::selectBtnOn, this));
            tempBtn.push_back(btn);
        }
        
        if(tempBtn.size() > 0){
            auto size1 = m_listRoomConfigBg->getContentSize();
            auto layout = Layout::create();
            layout->setContentSize(Size(size1.width, ceil(tempBtn.size()/3.0) *90));
            m_listRoomConfigBg->addChild(layout);
            
            countText->setPosition(Vec2(50,layout->getContentSize().height - 30));
            layout->addChild(countText);
            auto x = 2/3;
            for (int k3 = 0; k3 < tempBtn.size(); k3++)
            {
                Layout *temp = tempBtn.at(k3);
                temp->setPosition(Vec2(190 + k3%3*((layout->getContentSize().width-190)/3), layout->getContentSize().height - k3/3*90));
                layout->addChild(temp, 2);
            }
            m_butMap[i] = tempBtn;
        }
    }
}

void CreateRoom::updatePackageConfigView()
{
    auto size = m_listPackageBg->getContentSize();
    for(int i = 0; i < m_showData.size();i++){
        auto tempObject = m_showData[i];
        auto sprite = Sprite::create("GameHall/createRoom/title1.png");
        auto titleSize = sprite->getContentSize();
        auto layout3 = Layout::create();
        layout3->setContentSize(Size(size.width, titleSize.height));
        sprite->setPosition(Vec2(size.width*0.5, titleSize.height*0.5));
        layout3->addChild(sprite);
        
        auto size = sprite->getContentSize();
        auto classifyName = tempObject["packageName"].asString();
        Text* countText  = Text::create(classifyName, "", 45);
        countText->setAnchorPoint(Vec2(0.5, 0.5));
        countText->setTextHorizontalAlignment(cocos2d::TextHAlignment::LEFT);
        countText->setPosition(Vec2(size.width*0.5, size.height*0.5));
        countText->setTextColor(Color4B(197, 151, 70, 255));
        sprite->addChild(countText);
        
        m_listPackageBg->addChild(layout3, 5);
    }
}

void CreateRoom::selectBtnOn(Ref*ref)
{
    Layout *btn = (Layout*)ref;
    ImageView *selecIcon = (ImageView*)btn->getChildByName("selecIcon");
    
    int tag = btn->getTag();
    Json::Value tempObject = m_showData[tag/100]["gameCustomPlayValInfoList"][tag%100];
    if(tempObject["choiceType"].asInt() == 1)
    {
        selecIcon->setVisible(true);
        tempObject["iptValue"] = tempObject["playId"];
        auto tempBtns = m_butMap[tag/100];
        for(int i = 0 ; i < tempBtns.size();i++){
            auto tempBtn = tempBtns[i];
            if(btn == tempBtn){
                continue;
            }
            selecIcon = (ImageView*)btn->getChildByName("selecIcon");
            selecIcon->setVisible(false);
            m_showData[tag/100]["gameCustomPlayValInfoList"][i]["iptValue"] = Json::Value::null;
        }
    }else if(tempObject["choiceType"].asInt() == 2){
        bool isSele = !(selecIcon->isVisible());
        //多选
        selecIcon->setVisible(isSele);
        if (isSele)
        {
            m_showData[m_baoIndex]["gameCustomPackageList"][tag/100]["gameCustomPlayValInfoList"][tag%100]["iptValue"] = m_showData[m_baoIndex]["gameCustomPackageList"][tag/100]["gameCustomPlayValInfoList"][tag%100]["playId"];
            
        }else
        {
            m_showData[m_baoIndex]["gameCustomPackageList"][tag/100]["gameCustomPlayValInfoList"][tag%100]["iptValue"] = Json::Value::null;
        }
    }
}

void CreateRoom::test()
{
    auto size = m_listPackageBg->getContentSize();
    for(int i = 0; i < 15;i++){
        auto sprite = Sprite::create("GameHall/createRoom/title1.png");
        auto titleSize = sprite->getContentSize();
        auto layout3 = Layout::create();
        layout3->setContentSize(Size(size.width, titleSize.height));
        sprite->setPosition(Vec2(size.width*0.5, titleSize.height*0.5));
        layout3->addChild(sprite);
        m_listPackageBg->addChild(layout3, 5);
    }
    auto size1 = m_listPackageBg->getContentSize();
    for(int i = 0 ; i < 4;i++)
    {
        std::vector<Layout*> tempBtn;
        Text* countText  = Text::create("局数", "", 45);
        countText->setAnchorPoint(Vec2(0, 1));
        countText->setTextHorizontalAlignment(cocos2d::TextHAlignment::LEFT);
        countText->setPosition(Vec2(50, m_listRoomConfigBg->getContentSize().height - 45));
        countText->setTextColor(Color4B(197, 151, 70, 255));
        
        for(int i = 0; i < 5;i++){
            auto btn = this->selectBtnDraw("测试", true, 1);
            btn->setAnchorPoint(Vec2(0, 1));
            tempBtn.push_back(btn);
        }
        
        if(tempBtn.size() > 0){
            auto size1 = m_listRoomConfigBg->getContentSize();
            auto layout = Layout::create();
            layout->setContentSize(Size(size1.width, ceil(tempBtn.size()/3.0) *90));
            m_listRoomConfigBg->addChild(layout);
            
            countText->setPosition(Vec2(50,layout->getContentSize().height - 30));
            layout->addChild(countText);
            auto x = 2/3;
            for (int k3 = 0; k3 < tempBtn.size(); k3++)
            {
                Layout *temp = tempBtn.at(k3);
                temp->setPosition(Vec2(190 + k3%3*((layout->getContentSize().width-190)/3), layout->getContentSize().height - k3/3*90));
                layout->addChild(temp, 2);
            }
        }
    }
}


void CreateRoom::initTextData()
{
    Json::Value allData;
    for(int j = 0 ; j < 3;j++){
        Json::Value data;
        Json::Value gameCustomPackageList;
        data["packageId"] = 307;
        data["packageName"] = StringUtils::format("%d人麻将",j);
        for(int i = 0; i < 2;i++){
            Json::Value object;
            object["classifyId"] = 307;
            object["classifyName"] = StringUtils::format("抓马配置%d",i);
            object["id"] = 63161;
            
            if(i == 0){
                object["choiceType"] = 1;
            }else{
                object["choiceType"] = 2;
            }
            
            Json::Value gameCustomClassifyVOList;
            for(int k = 0 ; k < 3; k++){
                Json::Value gameCustomPlayValInfoList;
                gameCustomPlayValInfoList["classifyId"] = 4043;
                gameCustomPlayValInfoList["playId"] = 1132001 + 100*i + k;
                gameCustomPlayValInfoList["iptValue"] = 1132001 + 100*i + k;
                gameCustomPlayValInfoList["playName"] = StringUtils::format("%d=抓马",i,k);
                if(k == 0){
                    gameCustomPlayValInfoList["visible"] = true;
                }else{
                    gameCustomPlayValInfoList["visible"] = false;
                }
                gameCustomClassifyVOList.append(gameCustomPlayValInfoList);
            }
            object["gameCustomPlayValInfoList"] = gameCustomClassifyVOList;
            gameCustomPackageList.append(object);
        }
        data["gameCustomPackageList"] = gameCustomPackageList;
        allData.append(data);
    }

    m_showData = allData;
    log("lin=initTextData data [%s]", m_showData.toStyledString().c_str());
}
void CreateRoom::showUI()
{
//    for(int i = 0; i < m_showData.size();i++){
//        Json::Value object = m_showData[i];
//        Json::Value gameCustomPlayValInfoList = object["gameCustomPlayValInfoList"];
//        std::vector<Layout*> tempBtn;
//        
//        Text* countText  = Text::create("局数", "", 45);
//        countText->setAnchorPoint(Vec2(0, 1));
//        countText->setTextHorizontalAlignment(cocos2d::TextHAlignment::LEFT);
//        countText->setPosition(Vec2(50, layout3->getContentSize().height - 45));
//        countText->setTextColor(Color4B(197, 151, 70, 255));
//        layout3->addChild(countText);
//        
//        for(int k = 0; k < gameCustomPlayValInfoList.size();k++){
//            Json::Value tempObject = gameCustomPlayValInfoList[k];
//            std::string titleStr =tempObject["playName"].asString();
//            auto btn = selectBtnDraw(titleStr, true, k);
//            tempBtn.push_back(btn);
//        }
//    }
}

Layout *CreateRoom::selectBtnDraw(string titleStr, bool selected, int cellType)
{
    Layout *btn = Layout::create();
    btn->setContentSize(Size((550-50)/2*0.66, 90));
    
    Text *title = Text::create(titleStr, "", 43);
    title->setContentSize(Size(225-25, 90));
    title->setTextColor(Color4B(80, 0, 0, 255));
    title->setAnchorPoint(Vec2(0, 0.5));
    title->setPosition(Vec2(70, 45));
    title->setTextHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
    btn->addChild(title);
    
    ImageView *kongImg = ImageView::create();
    kongImg->setAnchorPoint(Vec2(0, 0.5));
    kongImg->setPosition(Vec2(0, 45));
    btn->addChild(kongImg);
    
    ImageView *selelctImg = ImageView::create();
    selelctImg->setName("selecIcon");
    selelctImg->setAnchorPoint(Vec2(0, 0.5));
    selelctImg->setPosition(kongImg->getPosition());
    btn->addChild(selelctImg, 2);
    selelctImg->setVisible(selected);
    
    if (cellType == 1)
    {
        kongImg->loadTexture("GameHall/createRoom/point_hold_kong2.png");
        selelctImg->loadTexture("GameHall/createRoom/play_point_check_sigle_can.png");
        selelctImg->setPosition(kongImg->getPosition() + Vec2(-7,2));
        
    }else if (cellType == 2)
    {
        kongImg->loadTexture("GameHall/createRoom/point_hold_kong1.png");
        selelctImg->loadTexture("GameHall/createRoom/play_point_check_can.png");
        selelctImg->setPosition(kongImg->getPosition() + Vec2(-5,0));
    }
    return  btn;
}


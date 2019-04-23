//
//  CommonModel.cpp
//  ZJH
//
//  Created by luochuanting on 13-6-21.
//
//

#include "CommonModel.h"
#include "GameHallScene.h"
CommonModel *g_commonModel;

CommonModel::CommonModel()
{
    
}

CommonModel::~CommonModel()
{
    
}

CommonModel* CommonModel::getInstance()
{
    if (g_commonModel == NULL) {
        g_commonModel = new CommonModel();
        g_commonModel->init();
        g_commonModel->retain();
    }
    return g_commonModel;
}

void CommonModel::init()
{
    
}

void CommonModel::toHall()
{
    Director::getInstance()->replaceScene(GameHallLayer::createScene());
}

void CommonModel::hallToRoomH()
{
    Director::getInstance()->replaceScene(GameHallLayer::createScene());
}

void CommonModel::hallToRoomV()
{
    Director::getInstance()->replaceScene(GameHallLayer::createScene());
}


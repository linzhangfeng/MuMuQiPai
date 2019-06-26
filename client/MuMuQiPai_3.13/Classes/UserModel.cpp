//
//  UserModel.cpp
//  ZJH
//
//  Created by luochuanting on 13-6-21.
//
//

#include "UserModel.h"
UserModel *g_pUserModel;

UserModel::UserModel()
{
    
}

UserModel::~UserModel()
{
    
}

UserModel* UserModel::getInstance()
{
    if (g_pUserModel == NULL) {
        g_pUserModel = new UserModel();
        g_pUserModel->retain();
    }
    return g_pUserModel;
}




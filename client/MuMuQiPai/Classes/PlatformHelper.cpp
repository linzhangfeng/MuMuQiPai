//
//  PlatformHelper.cpp
//  ZJH
//
//  Created by wujunru on 14-4-16.
//
//

#include "PlatformHelper.h"
#include "lin.h"

void PlatformHelper::showToast(std::string str,int type)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    if (type == 0) {
        IOSToast::showToast(str.c_str(), IOSToast::ToastDurationNormal);
    }
    else
    {
        IOSToast::showToast(str.c_str(), IOSToast::ToastDurationLong);
    }
#else
    if (type == 0)
    {
        sdkDoForVoid_C("showToast_Short",str);
    }
    else
    {
        sdkDoForVoid_C("showToast_Long",str);
    }
#endif
}
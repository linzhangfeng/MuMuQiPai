//
//  PlatformHelper.h
//  ZJH
//
//  Created by Apple on 13-11-27.
//
//

#ifndef ZJH_PlatformHelper_h
#define ZJH_PlatformHelper_h

#include <string>

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#ifndef ENABLE_IOS_PAY
#define ENABLE_IOS_PAY 1
#endif
#else
#ifndef ENABLE_IOS_PAY
#define ENABLE_IOS_PAY 1
#endif
#endif

class PlatformHelper
{
public:
    static void showToast(std::string str,int type = 0);
};
#endif

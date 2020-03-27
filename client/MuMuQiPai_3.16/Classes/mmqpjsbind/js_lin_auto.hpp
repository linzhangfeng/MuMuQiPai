#include "base/ccConfig.h"
#ifndef __lin_h__
#define __lin_h__

#include "jsapi.h"
#include "jsfriendapi.h"

extern JSClass  *jsb_PlatformHelper_class;
extern JSObject *jsb_PlatformHelper_prototype;

bool js_lin_PlatformHelper_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_lin_PlatformHelper_finalize(JSContext *cx, JSObject *obj);
void js_register_lin_PlatformHelper(JSContext *cx, JS::HandleObject global);
void register_all_lin(JSContext* cx, JS::HandleObject obj);
bool js_lin_PlatformHelper_showToast(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CommonModel_class;
extern JSObject *jsb_CommonModel_prototype;

bool js_lin_CommonModel_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_lin_CommonModel_finalize(JSContext *cx, JSObject *obj);
void js_register_lin_CommonModel(JSContext *cx, JS::HandleObject global);
void register_all_lin(JSContext* cx, JS::HandleObject obj);
bool js_lin_CommonModel_hallToRoomV(JSContext *cx, uint32_t argc, jsval *vp);
bool js_lin_CommonModel_getPackName(JSContext *cx, uint32_t argc, jsval *vp);
bool js_lin_CommonModel_toHall(JSContext *cx, uint32_t argc, jsval *vp);
bool js_lin_CommonModel_setRoomData(JSContext *cx, uint32_t argc, jsval *vp);
bool js_lin_CommonModel_init(JSContext *cx, uint32_t argc, jsval *vp);
bool js_lin_CommonModel_getRoomData(JSContext *cx, uint32_t argc, jsval *vp);
bool js_lin_CommonModel_hallToRoomH(JSContext *cx, uint32_t argc, jsval *vp);
bool js_lin_CommonModel_getInstance(JSContext *cx, uint32_t argc, jsval *vp);
bool js_lin_CommonModel_CommonModel(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_UserModel_class;
extern JSObject *jsb_UserModel_prototype;

bool js_lin_UserModel_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_lin_UserModel_finalize(JSContext *cx, JSObject *obj);
void js_register_lin_UserModel(JSContext *cx, JS::HandleObject global);
void register_all_lin(JSContext* cx, JS::HandleObject obj);
bool js_lin_UserModel_setUserName(JSContext *cx, uint32_t argc, jsval *vp);
bool js_lin_UserModel_getCurRoomId(JSContext *cx, uint32_t argc, jsval *vp);
bool js_lin_UserModel_setCurRoomId(JSContext *cx, uint32_t argc, jsval *vp);
bool js_lin_UserModel_getImsi(JSContext *cx, uint32_t argc, jsval *vp);
bool js_lin_UserModel_setChannel(JSContext *cx, uint32_t argc, jsval *vp);
bool js_lin_UserModel_getSex(JSContext *cx, uint32_t argc, jsval *vp);
bool js_lin_UserModel_setImsi(JSContext *cx, uint32_t argc, jsval *vp);
bool js_lin_UserModel_getMoney(JSContext *cx, uint32_t argc, jsval *vp);
bool js_lin_UserModel_getLevel(JSContext *cx, uint32_t argc, jsval *vp);
bool js_lin_UserModel_getIp(JSContext *cx, uint32_t argc, jsval *vp);
bool js_lin_UserModel_getAccount(JSContext *cx, uint32_t argc, jsval *vp);
bool js_lin_UserModel_getUid(JSContext *cx, uint32_t argc, jsval *vp);
bool js_lin_UserModel_getUserName(JSContext *cx, uint32_t argc, jsval *vp);
bool js_lin_UserModel_setLevel(JSContext *cx, uint32_t argc, jsval *vp);
bool js_lin_UserModel_setAccount(JSContext *cx, uint32_t argc, jsval *vp);
bool js_lin_UserModel_setExp(JSContext *cx, uint32_t argc, jsval *vp);
bool js_lin_UserModel_getChannel(JSContext *cx, uint32_t argc, jsval *vp);
bool js_lin_UserModel_getExp(JSContext *cx, uint32_t argc, jsval *vp);
bool js_lin_UserModel_setUid(JSContext *cx, uint32_t argc, jsval *vp);
bool js_lin_UserModel_setMoney(JSContext *cx, uint32_t argc, jsval *vp);
bool js_lin_UserModel_setSex(JSContext *cx, uint32_t argc, jsval *vp);
bool js_lin_UserModel_setIp(JSContext *cx, uint32_t argc, jsval *vp);
bool js_lin_UserModel_getInstance(JSContext *cx, uint32_t argc, jsval *vp);
bool js_lin_UserModel_UserModel(JSContext *cx, uint32_t argc, jsval *vp);

#endif // __lin_h__

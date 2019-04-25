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
bool js_lin_CommonModel_init(JSContext *cx, uint32_t argc, jsval *vp);
bool js_lin_CommonModel_hallToRoomH(JSContext *cx, uint32_t argc, jsval *vp);
bool js_lin_CommonModel_getInstance(JSContext *cx, uint32_t argc, jsval *vp);
bool js_lin_CommonModel_CommonModel(JSContext *cx, uint32_t argc, jsval *vp);

#endif // __lin_h__

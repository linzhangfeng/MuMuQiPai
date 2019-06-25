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

extern JSClass  *jsb_SocketRequest_class;
extern JSObject *jsb_SocketRequest_prototype;

bool js_lin_SocketRequest_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_lin_SocketRequest_finalize(JSContext *cx, JSObject *obj);
void js_register_lin_SocketRequest(JSContext *cx, JS::HandleObject global);
void register_all_lin(JSContext* cx, JS::HandleObject obj);

extern JSClass  *jsb_SocketResponse_class;
extern JSObject *jsb_SocketResponse_prototype;

bool js_lin_SocketResponse_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_lin_SocketResponse_finalize(JSContext *cx, JSObject *obj);
void js_register_lin_SocketResponse(JSContext *cx, JS::HandleObject global);
void register_all_lin(JSContext* cx, JS::HandleObject obj);
bool js_lin_SocketResponse_release(JSContext *cx, uint32_t argc, jsval *vp);
bool js_lin_SocketResponse_parseDatta(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_SocketClient_class;
extern JSObject *jsb_SocketClient_prototype;

bool js_lin_SocketClient_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_lin_SocketClient_finalize(JSContext *cx, JSObject *obj);
void js_register_lin_SocketClient(JSContext *cx, JS::HandleObject global);
void register_all_lin(JSContext* cx, JS::HandleObject obj);
bool js_lin_SocketClient_connect_req(JSContext *cx, uint32_t argc, jsval *vp);
bool js_lin_SocketClient_getJsonStr(JSContext *cx, uint32_t argc, jsval *vp);
bool js_lin_SocketClient_seCurRoomId(JSContext *cx, uint32_t argc, jsval *vp);
bool js_lin_SocketClient_setConnectState(JSContext *cx, uint32_t argc, jsval *vp);
bool js_lin_SocketClient_get(JSContext *cx, uint32_t argc, jsval *vp);
bool js_lin_SocketClient_onError(JSContext *cx, uint32_t argc, jsval *vp);
bool js_lin_SocketClient_onClose(JSContext *cx, uint32_t argc, jsval *vp);
bool js_lin_SocketClient_getCurRoomId(JSContext *cx, uint32_t argc, jsval *vp);
bool js_lin_SocketClient_create_response(JSContext *cx, uint32_t argc, jsval *vp);
bool js_lin_SocketClient_reset_response_queue(JSContext *cx, uint32_t argc, jsval *vp);
bool js_lin_SocketClient_init(JSContext *cx, uint32_t argc, jsval *vp);
bool js_lin_SocketClient_isConnect(JSContext *cx, uint32_t argc, jsval *vp);
bool js_lin_SocketClient_send_Data(JSContext *cx, uint32_t argc, jsval *vp);
bool js_lin_SocketClient_put(JSContext *cx, uint32_t argc, jsval *vp);
bool js_lin_SocketClient_data_req(JSContext *cx, uint32_t argc, jsval *vp);
bool js_lin_SocketClient_disconnect_req(JSContext *cx, uint32_t argc, jsval *vp);
bool js_lin_SocketClient_getInstance(JSContext *cx, uint32_t argc, jsval *vp);
bool js_lin_SocketClient_SocketClient(JSContext *cx, uint32_t argc, jsval *vp);

#endif // __lin_h__

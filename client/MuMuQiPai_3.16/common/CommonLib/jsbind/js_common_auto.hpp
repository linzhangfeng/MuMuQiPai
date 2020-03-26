#include "base/ccConfig.h"
#ifndef __js_common_auto_h__
#define __js_common_auto_h__

#include "jsapi.h"
#include "jsfriendapi.h"

extern JSClass  *jsb_JPacket_class;
extern JSObject *jsb_JPacket_prototype;

bool js_js_common_auto_JPacket_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_js_common_auto_JPacket_finalize(JSContext *cx, JSObject *obj);
void js_register_js_common_auto_JPacket(JSContext *cx, JS::HandleObject global);
void register_all_js_common_auto(JSContext* cx, JS::HandleObject obj);
bool js_js_common_auto_JPacket_end(JSContext *cx, uint32_t argc, jsval *vp);
bool js_js_common_auto_JPacket_parse(JSContext *cx, uint32_t argc, jsval *vp);
bool js_js_common_auto_JPacket_tostring(JSContext *cx, uint32_t argc, jsval *vp);
bool js_js_common_auto_JPacket_sefe_check(JSContext *cx, uint32_t argc, jsval *vp);
bool js_js_common_auto_JPacket_decodeStr(JSContext *cx, uint32_t argc, jsval *vp);
bool js_js_common_auto_JPacket_codeStr(JSContext *cx, uint32_t argc, jsval *vp);
bool js_js_common_auto_JPacket_JPacket(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_TcpClientTest_class;
extern JSObject *jsb_TcpClientTest_prototype;

bool js_js_common_auto_TcpClientTest_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_js_common_auto_TcpClientTest_finalize(JSContext *cx, JSObject *obj);
void js_register_js_common_auto_TcpClientTest(JSContext *cx, JS::HandleObject global);
void register_all_js_common_auto(JSContext* cx, JS::HandleObject obj);
bool js_js_common_auto_TcpClientTest_connectReq(JSContext *cx, uint32_t argc, jsval *vp);
bool js_js_common_auto_TcpClientTest_readRes(JSContext *cx, uint32_t argc, jsval *vp);
bool js_js_common_auto_TcpClientTest_createRequre(JSContext *cx, uint32_t argc, jsval *vp);
bool js_js_common_auto_TcpClientTest_resetResponeQueue(JSContext *cx, uint32_t argc, jsval *vp);
bool js_js_common_auto_TcpClientTest_disconnect(JSContext *cx, uint32_t argc, jsval *vp);
bool js_js_common_auto_TcpClientTest_getCurrentSecend(JSContext *cx, uint32_t argc, jsval *vp);
bool js_js_common_auto_TcpClientTest_get(JSContext *cx, uint32_t argc, jsval *vp);
bool js_js_common_auto_TcpClientTest_sendData(JSContext *cx, uint32_t argc, jsval *vp);
bool js_js_common_auto_TcpClientTest_startConnect(JSContext *cx, uint32_t argc, jsval *vp);
bool js_js_common_auto_TcpClientTest_dataReq(JSContext *cx, uint32_t argc, jsval *vp);
bool js_js_common_auto_TcpClientTest_resetConnect(JSContext *cx, uint32_t argc, jsval *vp);
bool js_js_common_auto_TcpClientTest_getCurrentTime(JSContext *cx, uint32_t argc, jsval *vp);
bool js_js_common_auto_TcpClientTest_setHost(JSContext *cx, uint32_t argc, jsval *vp);
bool js_js_common_auto_TcpClientTest_init(JSContext *cx, uint32_t argc, jsval *vp);
bool js_js_common_auto_TcpClientTest_put(JSContext *cx, uint32_t argc, jsval *vp);
bool js_js_common_auto_TcpClientTest_writeData(JSContext *cx, uint32_t argc, jsval *vp);
bool js_js_common_auto_TcpClientTest_disconnectReq(JSContext *cx, uint32_t argc, jsval *vp);
bool js_js_common_auto_TcpClientTest_setPost(JSContext *cx, uint32_t argc, jsval *vp);
bool js_js_common_auto_TcpClientTest_createRespone(JSContext *cx, uint32_t argc, jsval *vp);
bool js_js_common_auto_TcpClientTest_getInstance(JSContext *cx, uint32_t argc, jsval *vp);
bool js_js_common_auto_TcpClientTest_TcpClientTest(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_Network_class;
extern JSObject *jsb_Network_prototype;

bool js_js_common_auto_Network_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_js_common_auto_Network_finalize(JSContext *cx, JSObject *obj);
void js_register_js_common_auto_Network(JSContext *cx, JS::HandleObject global);
void register_all_js_common_auto(JSContext* cx, JS::HandleObject obj);
bool js_js_common_auto_Network_tcp_connect(JSContext *cx, uint32_t argc, jsval *vp);
bool js_js_common_auto_Network_setIpv6Enable(JSContext *cx, uint32_t argc, jsval *vp);
bool js_js_common_auto_Network_connect_nonb(JSContext *cx, uint32_t argc, jsval *vp);
bool js_js_common_auto_Network_Network(JSContext *cx, uint32_t argc, jsval *vp);

#endif // __js_common_auto_h__

#include "js_common_auto.hpp"
#include "ScriptingCore.h"
#include "scripting/js-bindings/manual/cocos2d_specifics.hpp"
#include "Common.h"

template<class T>
static bool dummy_constructor(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS_ReportError(cx, "Constructor for the requested class is not available, please refer to the API reference.");
    return false;
}

static bool empty_constructor(JSContext *cx, uint32_t argc, jsval *vp) {
    return false;
}

static bool js_is_native_obj(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    args.rval().setBoolean(true);
    return true;
}
JSClass  *jsb_JPacket_class;
JSObject *jsb_JPacket_prototype;

bool js_js_common_auto_JPacket_end(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    JPacket* cobj = (JPacket *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_js_common_auto_JPacket_end : Invalid Native Object");
    if (argc == 0) {
        cobj->end();
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_js_common_auto_JPacket_end : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_js_common_auto_JPacket_parse(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    JPacket* cobj = (JPacket *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_js_common_auto_JPacket_parse : Invalid Native Object");
    if (argc == 1) {
        std::string arg0;
        ok &= jsval_to_std_string(cx, args.get(0), &arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_js_common_auto_JPacket_parse : Error processing arguments");
        int ret = cobj->parse(arg0);
        jsval jsret = JSVAL_NULL;
        jsret = int32_to_jsval(cx, ret);
        args.rval().set(jsret);
        return true;
    }

    JS_ReportError(cx, "js_js_common_auto_JPacket_parse : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_js_common_auto_JPacket_tostring(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    JPacket* cobj = (JPacket *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_js_common_auto_JPacket_tostring : Invalid Native Object");
    if (argc == 0) {
        std::string& ret = cobj->tostring();
        jsval jsret = JSVAL_NULL;
        jsret = std_string_to_jsval(cx, ret);
        args.rval().set(jsret);
        return true;
    }

    JS_ReportError(cx, "js_js_common_auto_JPacket_tostring : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_js_common_auto_JPacket_sefe_check(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    JPacket* cobj = (JPacket *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_js_common_auto_JPacket_sefe_check : Invalid Native Object");
    if (argc == 0) {
        int ret = cobj->sefe_check();
        jsval jsret = JSVAL_NULL;
        jsret = int32_to_jsval(cx, ret);
        args.rval().set(jsret);
        return true;
    }

    JS_ReportError(cx, "js_js_common_auto_JPacket_sefe_check : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_js_common_auto_JPacket_decodeStr(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    if (argc == 1) {
        std::string arg0;
        ok &= jsval_to_std_string(cx, args.get(0), &arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_js_common_auto_JPacket_decodeStr : Error processing arguments");

        std::string ret = JPacket::decodeStr(arg0);
        jsval jsret = JSVAL_NULL;
        jsret = std_string_to_jsval(cx, ret);
        args.rval().set(jsret);
        return true;
    }
    JS_ReportError(cx, "js_js_common_auto_JPacket_decodeStr : wrong number of arguments");
    return false;
}

bool js_js_common_auto_JPacket_codeStr(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    if (argc == 1) {
        std::string arg0;
        ok &= jsval_to_std_string(cx, args.get(0), &arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_js_common_auto_JPacket_codeStr : Error processing arguments");

        std::string ret = JPacket::codeStr(arg0);
        jsval jsret = JSVAL_NULL;
        jsret = std_string_to_jsval(cx, ret);
        args.rval().set(jsret);
        return true;
    }
    JS_ReportError(cx, "js_js_common_auto_JPacket_codeStr : wrong number of arguments");
    return false;
}

bool js_js_common_auto_JPacket_constructor(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JPacket* cobj = new (std::nothrow) JPacket();

    js_type_class_t *typeClass = js_get_type_from_native<JPacket>(cobj);

    // link the native object with the javascript object
    JS::RootedObject jsobj(cx, jsb_create_weak_jsobject(cx, cobj, typeClass, "JPacket"));
    args.rval().set(OBJECT_TO_JSVAL(jsobj));
    if (JS_HasProperty(cx, jsobj, "_ctor", &ok) && ok)
        ScriptingCore::getInstance()->executeFunctionWithOwner(OBJECT_TO_JSVAL(jsobj), "_ctor", args);
    return true;
}


void js_JPacket_finalize(JSFreeOp *fop, JSObject *obj) {
    CCLOGINFO("jsbindings: finalizing JS object %p (JPacket)", obj);
    js_proxy_t* nproxy;
    js_proxy_t* jsproxy;
    JSContext *cx = ScriptingCore::getInstance()->getGlobalContext();
    JS::RootedObject jsobj(cx, obj);
    jsproxy = jsb_get_js_proxy(jsobj);
    if (jsproxy) {
        JPacket *nobj = static_cast<JPacket *>(jsproxy->ptr);
        nproxy = jsb_get_native_proxy(jsproxy->ptr);

        if (nobj) {
            jsb_remove_proxy(nproxy, jsproxy);
            delete nobj;
        }
        else
            jsb_remove_proxy(nullptr, jsproxy);
    }
}
void js_register_js_common_auto_JPacket(JSContext *cx, JS::HandleObject global) {
    jsb_JPacket_class = (JSClass *)calloc(1, sizeof(JSClass));
    jsb_JPacket_class->name = "JPacket";
    jsb_JPacket_class->addProperty = JS_PropertyStub;
    jsb_JPacket_class->delProperty = JS_DeletePropertyStub;
    jsb_JPacket_class->getProperty = JS_PropertyStub;
    jsb_JPacket_class->setProperty = JS_StrictPropertyStub;
    jsb_JPacket_class->enumerate = JS_EnumerateStub;
    jsb_JPacket_class->resolve = JS_ResolveStub;
    jsb_JPacket_class->convert = JS_ConvertStub;
    jsb_JPacket_class->finalize = js_JPacket_finalize;
    jsb_JPacket_class->flags = JSCLASS_HAS_RESERVED_SLOTS(2);

    static JSPropertySpec properties[] = {
        JS_PS_END
    };

    static JSFunctionSpec funcs[] = {
        JS_FN("end", js_js_common_auto_JPacket_end, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("parse", js_js_common_auto_JPacket_parse, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("tostring", js_js_common_auto_JPacket_tostring, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("sefe_check", js_js_common_auto_JPacket_sefe_check, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FS_END
    };

    static JSFunctionSpec st_funcs[] = {
        JS_FN("decodeStr", js_js_common_auto_JPacket_decodeStr, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("codeStr", js_js_common_auto_JPacket_codeStr, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FS_END
    };

    jsb_JPacket_prototype = JS_InitClass(
        cx, global,
        JS::NullPtr(),
        jsb_JPacket_class,
        js_js_common_auto_JPacket_constructor, 0, // constructor
        properties,
        funcs,
        NULL, // no static properties
        st_funcs);

    JS::RootedObject proto(cx, jsb_JPacket_prototype);
    JS::RootedValue className(cx, std_string_to_jsval(cx, "JPacket"));
    JS_SetProperty(cx, proto, "_className", className);
    JS_SetProperty(cx, proto, "__nativeObj", JS::TrueHandleValue);
    JS_SetProperty(cx, proto, "__is_ref", JS::FalseHandleValue);
    // add the proto and JSClass to the type->js info hash table
    jsb_register_class<JPacket>(cx, jsb_JPacket_class, proto, JS::NullPtr());
}

JSClass  *jsb_TcpClientTest_class;
JSObject *jsb_TcpClientTest_prototype;

bool js_js_common_auto_TcpClientTest_connectReq(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    TcpClientTest* cobj = (TcpClientTest *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_js_common_auto_TcpClientTest_connectReq : Invalid Native Object");
    if (argc == 1) {
        MsgRequest* arg0 = nullptr;
        do {
            if (args.get(0).isNull()) { arg0 = nullptr; break; }
            if (!args.get(0).isObject()) { ok = false; break; }
            js_proxy_t *jsProxy;
            JS::RootedObject tmpObj(cx, args.get(0).toObjectOrNull());
            jsProxy = jsb_get_js_proxy(tmpObj);
            arg0 = (MsgRequest*)(jsProxy ? jsProxy->ptr : NULL);
            JSB_PRECONDITION2( arg0, cx, false, "Invalid Native Object");
        } while (0);
        JSB_PRECONDITION2(ok, cx, false, "js_js_common_auto_TcpClientTest_connectReq : Error processing arguments");
        cobj->connectReq(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_js_common_auto_TcpClientTest_connectReq : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_js_common_auto_TcpClientTest_readRes(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    TcpClientTest* cobj = (TcpClientTest *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_js_common_auto_TcpClientTest_readRes : Invalid Native Object");
    if (argc == 0) {
        cobj->readRes();
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_js_common_auto_TcpClientTest_readRes : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_js_common_auto_TcpClientTest_createRequre(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    TcpClientTest* cobj = (TcpClientTest *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_js_common_auto_TcpClientTest_createRequre : Invalid Native Object");
    if (argc == 2) {
        int arg0 = 0;
        std::string arg1;
        ok &= jsval_to_int32(cx, args.get(0), (int32_t *)&arg0);
        ok &= jsval_to_std_string(cx, args.get(1), &arg1);
        JSB_PRECONDITION2(ok, cx, false, "js_js_common_auto_TcpClientTest_createRequre : Error processing arguments");
        long long ret = cobj->createRequre(arg0, arg1);
        jsval jsret = JSVAL_NULL;
        jsret = long_long_to_jsval(cx, ret);
        args.rval().set(jsret);
        return true;
    }

    JS_ReportError(cx, "js_js_common_auto_TcpClientTest_createRequre : wrong number of arguments: %d, was expecting %d", argc, 2);
    return false;
}
bool js_js_common_auto_TcpClientTest_resetResponeQueue(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    TcpClientTest* cobj = (TcpClientTest *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_js_common_auto_TcpClientTest_resetResponeQueue : Invalid Native Object");
    if (argc == 0) {
        cobj->resetResponeQueue();
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_js_common_auto_TcpClientTest_resetResponeQueue : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_js_common_auto_TcpClientTest_disconnect(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    TcpClientTest* cobj = (TcpClientTest *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_js_common_auto_TcpClientTest_disconnect : Invalid Native Object");
    if (argc == 0) {
        cobj->disconnect();
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_js_common_auto_TcpClientTest_disconnect : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_js_common_auto_TcpClientTest_getCurrentSecend(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    TcpClientTest* cobj = (TcpClientTest *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_js_common_auto_TcpClientTest_getCurrentSecend : Invalid Native Object");
    if (argc == 0) {
        long long ret = cobj->getCurrentSecend();
        jsval jsret = JSVAL_NULL;
        jsret = long_long_to_jsval(cx, ret);
        args.rval().set(jsret);
        return true;
    }

    JS_ReportError(cx, "js_js_common_auto_TcpClientTest_getCurrentSecend : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_js_common_auto_TcpClientTest_get(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    TcpClientTest* cobj = (TcpClientTest *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_js_common_auto_TcpClientTest_get : Invalid Native Object");
    if (argc == 0) {
        MsgResponse* ret = cobj->get();
        jsval jsret = JSVAL_NULL;
        if (ret) {
            jsret = OBJECT_TO_JSVAL(js_get_or_create_jsobject<MsgResponse>(cx, (MsgResponse*)ret));
        } else {
            jsret = JSVAL_NULL;
        };
        args.rval().set(jsret);
        return true;
    }

    JS_ReportError(cx, "js_js_common_auto_TcpClientTest_get : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_js_common_auto_TcpClientTest_sendData(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    TcpClientTest* cobj = (TcpClientTest *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_js_common_auto_TcpClientTest_sendData : Invalid Native Object");
    if (argc == 1) {
        std::string arg0;
        ok &= jsval_to_std_string(cx, args.get(0), &arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_js_common_auto_TcpClientTest_sendData : Error processing arguments");
        cobj->sendData(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_js_common_auto_TcpClientTest_sendData : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_js_common_auto_TcpClientTest_startConnect(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    TcpClientTest* cobj = (TcpClientTest *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_js_common_auto_TcpClientTest_startConnect : Invalid Native Object");
    if (argc == 0) {
        cobj->startConnect();
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_js_common_auto_TcpClientTest_startConnect : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_js_common_auto_TcpClientTest_dataReq(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    TcpClientTest* cobj = (TcpClientTest *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_js_common_auto_TcpClientTest_dataReq : Invalid Native Object");
    if (argc == 1) {
        MsgRequest* arg0 = nullptr;
        do {
            if (args.get(0).isNull()) { arg0 = nullptr; break; }
            if (!args.get(0).isObject()) { ok = false; break; }
            js_proxy_t *jsProxy;
            JS::RootedObject tmpObj(cx, args.get(0).toObjectOrNull());
            jsProxy = jsb_get_js_proxy(tmpObj);
            arg0 = (MsgRequest*)(jsProxy ? jsProxy->ptr : NULL);
            JSB_PRECONDITION2( arg0, cx, false, "Invalid Native Object");
        } while (0);
        JSB_PRECONDITION2(ok, cx, false, "js_js_common_auto_TcpClientTest_dataReq : Error processing arguments");
        cobj->dataReq(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_js_common_auto_TcpClientTest_dataReq : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_js_common_auto_TcpClientTest_resetConnect(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    TcpClientTest* cobj = (TcpClientTest *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_js_common_auto_TcpClientTest_resetConnect : Invalid Native Object");
    if (argc == 0) {
        cobj->resetConnect();
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_js_common_auto_TcpClientTest_resetConnect : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_js_common_auto_TcpClientTest_getCurrentTime(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    TcpClientTest* cobj = (TcpClientTest *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_js_common_auto_TcpClientTest_getCurrentTime : Invalid Native Object");
    if (argc == 0) {
        long long ret = cobj->getCurrentTime();
        jsval jsret = JSVAL_NULL;
        jsret = long_long_to_jsval(cx, ret);
        args.rval().set(jsret);
        return true;
    }

    JS_ReportError(cx, "js_js_common_auto_TcpClientTest_getCurrentTime : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_js_common_auto_TcpClientTest_setHost(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    TcpClientTest* cobj = (TcpClientTest *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_js_common_auto_TcpClientTest_setHost : Invalid Native Object");
    if (argc == 1) {
        std::string arg0;
        ok &= jsval_to_std_string(cx, args.get(0), &arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_js_common_auto_TcpClientTest_setHost : Error processing arguments");
        cobj->setHost(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_js_common_auto_TcpClientTest_setHost : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_js_common_auto_TcpClientTest_init(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    TcpClientTest* cobj = (TcpClientTest *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_js_common_auto_TcpClientTest_init : Invalid Native Object");
    if (argc == 0) {
        cobj->init();
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_js_common_auto_TcpClientTest_init : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_js_common_auto_TcpClientTest_put(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    TcpClientTest* cobj = (TcpClientTest *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_js_common_auto_TcpClientTest_put : Invalid Native Object");
    if (argc == 1) {
        MsgRequest* arg0 = nullptr;
        do {
            if (args.get(0).isNull()) { arg0 = nullptr; break; }
            if (!args.get(0).isObject()) { ok = false; break; }
            js_proxy_t *jsProxy;
            JS::RootedObject tmpObj(cx, args.get(0).toObjectOrNull());
            jsProxy = jsb_get_js_proxy(tmpObj);
            arg0 = (MsgRequest*)(jsProxy ? jsProxy->ptr : NULL);
            JSB_PRECONDITION2( arg0, cx, false, "Invalid Native Object");
        } while (0);
        JSB_PRECONDITION2(ok, cx, false, "js_js_common_auto_TcpClientTest_put : Error processing arguments");
        cobj->put(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_js_common_auto_TcpClientTest_put : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_js_common_auto_TcpClientTest_writeData(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    TcpClientTest* cobj = (TcpClientTest *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_js_common_auto_TcpClientTest_writeData : Invalid Native Object");
    if (argc == 1) {
        MsgRequest* arg0 = nullptr;
        do {
            if (args.get(0).isNull()) { arg0 = nullptr; break; }
            if (!args.get(0).isObject()) { ok = false; break; }
            js_proxy_t *jsProxy;
            JS::RootedObject tmpObj(cx, args.get(0).toObjectOrNull());
            jsProxy = jsb_get_js_proxy(tmpObj);
            arg0 = (MsgRequest*)(jsProxy ? jsProxy->ptr : NULL);
            JSB_PRECONDITION2( arg0, cx, false, "Invalid Native Object");
        } while (0);
        JSB_PRECONDITION2(ok, cx, false, "js_js_common_auto_TcpClientTest_writeData : Error processing arguments");
        int ret = cobj->writeData(arg0);
        jsval jsret = JSVAL_NULL;
        jsret = int32_to_jsval(cx, ret);
        args.rval().set(jsret);
        return true;
    }

    JS_ReportError(cx, "js_js_common_auto_TcpClientTest_writeData : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_js_common_auto_TcpClientTest_disconnectReq(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    TcpClientTest* cobj = (TcpClientTest *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_js_common_auto_TcpClientTest_disconnectReq : Invalid Native Object");
    if (argc == 1) {
        MsgRequest* arg0 = nullptr;
        do {
            if (args.get(0).isNull()) { arg0 = nullptr; break; }
            if (!args.get(0).isObject()) { ok = false; break; }
            js_proxy_t *jsProxy;
            JS::RootedObject tmpObj(cx, args.get(0).toObjectOrNull());
            jsProxy = jsb_get_js_proxy(tmpObj);
            arg0 = (MsgRequest*)(jsProxy ? jsProxy->ptr : NULL);
            JSB_PRECONDITION2( arg0, cx, false, "Invalid Native Object");
        } while (0);
        JSB_PRECONDITION2(ok, cx, false, "js_js_common_auto_TcpClientTest_disconnectReq : Error processing arguments");
        cobj->disconnectReq(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_js_common_auto_TcpClientTest_disconnectReq : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_js_common_auto_TcpClientTest_setPost(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    TcpClientTest* cobj = (TcpClientTest *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_js_common_auto_TcpClientTest_setPost : Invalid Native Object");
    if (argc == 1) {
        int arg0 = 0;
        ok &= jsval_to_int32(cx, args.get(0), (int32_t *)&arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_js_common_auto_TcpClientTest_setPost : Error processing arguments");
        cobj->setPost(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_js_common_auto_TcpClientTest_setPost : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_js_common_auto_TcpClientTest_createRespone(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    TcpClientTest* cobj = (TcpClientTest *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_js_common_auto_TcpClientTest_createRespone : Invalid Native Object");
    if (argc == 2) {
        int arg0 = 0;
        std::string arg1;
        ok &= jsval_to_int32(cx, args.get(0), (int32_t *)&arg0);
        ok &= jsval_to_std_string(cx, args.get(1), &arg1);
        JSB_PRECONDITION2(ok, cx, false, "js_js_common_auto_TcpClientTest_createRespone : Error processing arguments");
        cobj->createRespone(arg0, arg1);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_js_common_auto_TcpClientTest_createRespone : wrong number of arguments: %d, was expecting %d", argc, 2);
    return false;
}
bool js_js_common_auto_TcpClientTest_getInstance(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    if (argc == 0) {

        TcpClientTest* ret = TcpClientTest::getInstance();
        jsval jsret = JSVAL_NULL;
        if (ret) {
        jsret = OBJECT_TO_JSVAL(js_get_or_create_jsobject<TcpClientTest>(cx, (TcpClientTest*)ret));
    } else {
        jsret = JSVAL_NULL;
    };
        args.rval().set(jsret);
        return true;
    }
    if (argc == 1) {
        int arg0 = 0;
        ok &= jsval_to_int32(cx, args.get(0), (int32_t *)&arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_js_common_auto_TcpClientTest_getInstance : Error processing arguments");

        TcpClientTest* ret = TcpClientTest::getInstance(arg0);
        jsval jsret = JSVAL_NULL;
        if (ret) {
        jsret = OBJECT_TO_JSVAL(js_get_or_create_jsobject<TcpClientTest>(cx, (TcpClientTest*)ret));
    } else {
        jsret = JSVAL_NULL;
    };
        args.rval().set(jsret);
        return true;
    }
    JS_ReportError(cx, "js_js_common_auto_TcpClientTest_getInstance : wrong number of arguments");
    return false;
}

bool js_js_common_auto_TcpClientTest_constructor(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    TcpClientTest* cobj = new (std::nothrow) TcpClientTest();

    js_type_class_t *typeClass = js_get_type_from_native<TcpClientTest>(cobj);

    // link the native object with the javascript object
    JS::RootedObject jsobj(cx, jsb_create_weak_jsobject(cx, cobj, typeClass, "TcpClientTest"));
    args.rval().set(OBJECT_TO_JSVAL(jsobj));
    if (JS_HasProperty(cx, jsobj, "_ctor", &ok) && ok)
        ScriptingCore::getInstance()->executeFunctionWithOwner(OBJECT_TO_JSVAL(jsobj), "_ctor", args);
    return true;
}


void js_TcpClientTest_finalize(JSFreeOp *fop, JSObject *obj) {
    CCLOGINFO("jsbindings: finalizing JS object %p (TcpClientTest)", obj);
    js_proxy_t* nproxy;
    js_proxy_t* jsproxy;
    JSContext *cx = ScriptingCore::getInstance()->getGlobalContext();
    JS::RootedObject jsobj(cx, obj);
    jsproxy = jsb_get_js_proxy(jsobj);
    if (jsproxy) {
        TcpClientTest *nobj = static_cast<TcpClientTest *>(jsproxy->ptr);
        nproxy = jsb_get_native_proxy(jsproxy->ptr);

        if (nobj) {
            jsb_remove_proxy(nproxy, jsproxy);
            delete nobj;
        }
        else
            jsb_remove_proxy(nullptr, jsproxy);
    }
}
void js_register_js_common_auto_TcpClientTest(JSContext *cx, JS::HandleObject global) {
    jsb_TcpClientTest_class = (JSClass *)calloc(1, sizeof(JSClass));
    jsb_TcpClientTest_class->name = "TcpClientTest";
    jsb_TcpClientTest_class->addProperty = JS_PropertyStub;
    jsb_TcpClientTest_class->delProperty = JS_DeletePropertyStub;
    jsb_TcpClientTest_class->getProperty = JS_PropertyStub;
    jsb_TcpClientTest_class->setProperty = JS_StrictPropertyStub;
    jsb_TcpClientTest_class->enumerate = JS_EnumerateStub;
    jsb_TcpClientTest_class->resolve = JS_ResolveStub;
    jsb_TcpClientTest_class->convert = JS_ConvertStub;
    jsb_TcpClientTest_class->finalize = js_TcpClientTest_finalize;
    jsb_TcpClientTest_class->flags = JSCLASS_HAS_RESERVED_SLOTS(2);

    static JSPropertySpec properties[] = {
        JS_PS_END
    };

    static JSFunctionSpec funcs[] = {
        JS_FN("connectReq", js_js_common_auto_TcpClientTest_connectReq, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("readRes", js_js_common_auto_TcpClientTest_readRes, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("createRequre", js_js_common_auto_TcpClientTest_createRequre, 2, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("resetResponeQueue", js_js_common_auto_TcpClientTest_resetResponeQueue, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("disconnect", js_js_common_auto_TcpClientTest_disconnect, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("getCurrentSecend", js_js_common_auto_TcpClientTest_getCurrentSecend, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("get", js_js_common_auto_TcpClientTest_get, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("sendData", js_js_common_auto_TcpClientTest_sendData, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("startConnect", js_js_common_auto_TcpClientTest_startConnect, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("dataReq", js_js_common_auto_TcpClientTest_dataReq, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("resetConnect", js_js_common_auto_TcpClientTest_resetConnect, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("getCurrentTime", js_js_common_auto_TcpClientTest_getCurrentTime, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("setHost", js_js_common_auto_TcpClientTest_setHost, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("init", js_js_common_auto_TcpClientTest_init, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("put", js_js_common_auto_TcpClientTest_put, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("writeData", js_js_common_auto_TcpClientTest_writeData, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("disconnectReq", js_js_common_auto_TcpClientTest_disconnectReq, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("setPost", js_js_common_auto_TcpClientTest_setPost, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("createRespone", js_js_common_auto_TcpClientTest_createRespone, 2, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FS_END
    };

    static JSFunctionSpec st_funcs[] = {
        JS_FN("getInstance", js_js_common_auto_TcpClientTest_getInstance, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FS_END
    };

    jsb_TcpClientTest_prototype = JS_InitClass(
        cx, global,
        JS::NullPtr(),
        jsb_TcpClientTest_class,
        js_js_common_auto_TcpClientTest_constructor, 0, // constructor
        properties,
        funcs,
        NULL, // no static properties
        st_funcs);

    JS::RootedObject proto(cx, jsb_TcpClientTest_prototype);
    JS::RootedValue className(cx, std_string_to_jsval(cx, "TcpClientTest"));
    JS_SetProperty(cx, proto, "_className", className);
    JS_SetProperty(cx, proto, "__nativeObj", JS::TrueHandleValue);
    JS_SetProperty(cx, proto, "__is_ref", JS::FalseHandleValue);
    // add the proto and JSClass to the type->js info hash table
    jsb_register_class<TcpClientTest>(cx, jsb_TcpClientTest_class, proto, JS::NullPtr());
}

JSClass  *jsb_Network_class;
JSObject *jsb_Network_prototype;

bool js_js_common_auto_Network_tcp_connect(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    
    do {
        if (argc == 2) {
            const char* arg0 = nullptr;
            std::string arg0_tmp; ok &= jsval_to_std_string(cx, args.get(0), &arg0_tmp); arg0 = arg0_tmp.c_str();
            if (!ok) { ok = true; break; }
            int arg1 = 0;
            ok &= jsval_to_int32(cx, args.get(1), (int32_t *)&arg1);
            if (!ok) { ok = true; break; }
            int ret = Network::tcp_connect(arg0, arg1);
            jsval jsret = JSVAL_NULL;
            jsret = int32_to_jsval(cx, ret);
            args.rval().set(jsret);
            return true;
        }
    } while (0);
    
    do {
        if (argc == 3) {
            const char* arg0 = nullptr;
            std::string arg0_tmp; ok &= jsval_to_std_string(cx, args.get(0), &arg0_tmp); arg0 = arg0_tmp.c_str();
            if (!ok) { ok = true; break; }
            int arg1 = 0;
            ok &= jsval_to_int32(cx, args.get(1), (int32_t *)&arg1);
            if (!ok) { ok = true; break; }
            int arg2 = 0;
            ok &= jsval_to_int32(cx, args.get(2), (int32_t *)&arg2);
            if (!ok) { ok = true; break; }
            int ret = Network::tcp_connect(arg0, arg1, arg2);
            jsval jsret = JSVAL_NULL;
            jsret = int32_to_jsval(cx, ret);
            args.rval().set(jsret);
            return true;
        }
    } while (0);
    
    do {
        if (argc == 4) {
            const char* arg0 = nullptr;
            std::string arg0_tmp; ok &= jsval_to_std_string(cx, args.get(0), &arg0_tmp); arg0 = arg0_tmp.c_str();
            if (!ok) { ok = true; break; }
            int arg1 = 0;
            ok &= jsval_to_int32(cx, args.get(1), (int32_t *)&arg1);
            if (!ok) { ok = true; break; }
            int arg2 = 0;
            ok &= jsval_to_int32(cx, args.get(2), (int32_t *)&arg2);
            if (!ok) { ok = true; break; }
            int arg3 = 0;
            ok &= jsval_to_int32(cx, args.get(3), (int32_t *)&arg3);
            if (!ok) { ok = true; break; }
            int ret = Network::tcp_connect(arg0, arg1, arg2, arg3);
            jsval jsret = JSVAL_NULL;
            jsret = int32_to_jsval(cx, ret);
            args.rval().set(jsret);
            return true;
        }
    } while (0);
    JS_ReportError(cx, "js_js_common_auto_Network_tcp_connect : wrong number of arguments");
    return false;
}
bool js_js_common_auto_Network_setIpv6Enable(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    if (argc == 1) {
        bool arg0;
        arg0 = JS::ToBoolean(args.get(0));
        JSB_PRECONDITION2(ok, cx, false, "js_js_common_auto_Network_setIpv6Enable : Error processing arguments");
        Network::setIpv6Enable(arg0);
        args.rval().setUndefined();
        return true;
    }
    JS_ReportError(cx, "js_js_common_auto_Network_setIpv6Enable : wrong number of arguments");
    return false;
}

bool js_js_common_auto_Network_connect_nonb(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    if (argc == 4) {
        int arg0 = 0;
        const sockaddr* arg1 = nullptr;
        int arg2 = 0;
        int arg3 = 0;
        ok &= jsval_to_int32(cx, args.get(0), (int32_t *)&arg0);
        #pragma warning NO CONVERSION TO NATIVE FOR sockaddr*
		ok = false;
        ok &= jsval_to_int32(cx, args.get(2), (int32_t *)&arg2);
        ok &= jsval_to_int32(cx, args.get(3), (int32_t *)&arg3);
        JSB_PRECONDITION2(ok, cx, false, "js_js_common_auto_Network_connect_nonb : Error processing arguments");

        int ret = Network::connect_nonb(arg0, arg1, arg2, arg3);
        jsval jsret = JSVAL_NULL;
        jsret = int32_to_jsval(cx, ret);
        args.rval().set(jsret);
        return true;
    }
    JS_ReportError(cx, "js_js_common_auto_Network_connect_nonb : wrong number of arguments");
    return false;
}

bool js_js_common_auto_Network_constructor(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    Network* cobj = new (std::nothrow) Network();

    js_type_class_t *typeClass = js_get_type_from_native<Network>(cobj);

    // link the native object with the javascript object
    JS::RootedObject jsobj(cx, jsb_create_weak_jsobject(cx, cobj, typeClass, "Network"));
    args.rval().set(OBJECT_TO_JSVAL(jsobj));
    if (JS_HasProperty(cx, jsobj, "_ctor", &ok) && ok)
        ScriptingCore::getInstance()->executeFunctionWithOwner(OBJECT_TO_JSVAL(jsobj), "_ctor", args);
    return true;
}


void js_Network_finalize(JSFreeOp *fop, JSObject *obj) {
    CCLOGINFO("jsbindings: finalizing JS object %p (Network)", obj);
    js_proxy_t* nproxy;
    js_proxy_t* jsproxy;
    JSContext *cx = ScriptingCore::getInstance()->getGlobalContext();
    JS::RootedObject jsobj(cx, obj);
    jsproxy = jsb_get_js_proxy(jsobj);
    if (jsproxy) {
        Network *nobj = static_cast<Network *>(jsproxy->ptr);
        nproxy = jsb_get_native_proxy(jsproxy->ptr);

        if (nobj) {
            jsb_remove_proxy(nproxy, jsproxy);
            delete nobj;
        }
        else
            jsb_remove_proxy(nullptr, jsproxy);
    }
}
void js_register_js_common_auto_Network(JSContext *cx, JS::HandleObject global) {
    jsb_Network_class = (JSClass *)calloc(1, sizeof(JSClass));
    jsb_Network_class->name = "Network";
    jsb_Network_class->addProperty = JS_PropertyStub;
    jsb_Network_class->delProperty = JS_DeletePropertyStub;
    jsb_Network_class->getProperty = JS_PropertyStub;
    jsb_Network_class->setProperty = JS_StrictPropertyStub;
    jsb_Network_class->enumerate = JS_EnumerateStub;
    jsb_Network_class->resolve = JS_ResolveStub;
    jsb_Network_class->convert = JS_ConvertStub;
    jsb_Network_class->finalize = js_Network_finalize;
    jsb_Network_class->flags = JSCLASS_HAS_RESERVED_SLOTS(2);

    static JSPropertySpec properties[] = {
        JS_PS_END
    };

    static JSFunctionSpec funcs[] = {
        JS_FS_END
    };

    static JSFunctionSpec st_funcs[] = {
        JS_FN("tcp_connect", js_js_common_auto_Network_tcp_connect, 2, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("setIpv6Enable", js_js_common_auto_Network_setIpv6Enable, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("connect_nonb", js_js_common_auto_Network_connect_nonb, 4, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FS_END
    };

    jsb_Network_prototype = JS_InitClass(
        cx, global,
        JS::NullPtr(),
        jsb_Network_class,
        js_js_common_auto_Network_constructor, 0, // constructor
        properties,
        funcs,
        NULL, // no static properties
        st_funcs);

    JS::RootedObject proto(cx, jsb_Network_prototype);
    JS::RootedValue className(cx, std_string_to_jsval(cx, "Network"));
    JS_SetProperty(cx, proto, "_className", className);
    JS_SetProperty(cx, proto, "__nativeObj", JS::TrueHandleValue);
    JS_SetProperty(cx, proto, "__is_ref", JS::FalseHandleValue);
    // add the proto and JSClass to the type->js info hash table
    jsb_register_class<Network>(cx, jsb_Network_class, proto, JS::NullPtr());
}

void register_all_js_common_auto(JSContext* cx, JS::HandleObject obj) {
    // Get the global ns
    JS::RootedObject ns(cx, ScriptingCore::getInstance()->getGlobalObject());

    js_register_js_common_auto_TcpClientTest(cx, ns);
    js_register_js_common_auto_JPacket(cx, ns);
    js_register_js_common_auto_Network(cx, ns);
}


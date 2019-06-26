#include "js_lin_auto.hpp"
#include "scripting/js-bindings/manual/cocos2d_specifics.hpp"
#include "js.h"

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
JSClass  *jsb_PlatformHelper_class;
JSObject *jsb_PlatformHelper_prototype;

bool js_lin_PlatformHelper_showToast(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    if (argc == 1) {
        std::string arg0;
        ok &= jsval_to_std_string(cx, args.get(0), &arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_lin_PlatformHelper_showToast : Error processing arguments");
        PlatformHelper::showToast(arg0);
        args.rval().setUndefined();
        return true;
    }
    if (argc == 2) {
        std::string arg0;
        int arg1 = 0;
        ok &= jsval_to_std_string(cx, args.get(0), &arg0);
        ok &= jsval_to_int32(cx, args.get(1), (int32_t *)&arg1);
        JSB_PRECONDITION2(ok, cx, false, "js_lin_PlatformHelper_showToast : Error processing arguments");
        PlatformHelper::showToast(arg0, arg1);
        args.rval().setUndefined();
        return true;
    }
    JS_ReportError(cx, "js_lin_PlatformHelper_showToast : wrong number of arguments");
    return false;
}


void js_register_lin_PlatformHelper(JSContext *cx, JS::HandleObject global) {
    jsb_PlatformHelper_class = (JSClass *)calloc(1, sizeof(JSClass));
    jsb_PlatformHelper_class->name = "PlatformHelper";
    jsb_PlatformHelper_class->addProperty = JS_PropertyStub;
    jsb_PlatformHelper_class->delProperty = JS_DeletePropertyStub;
    jsb_PlatformHelper_class->getProperty = JS_PropertyStub;
    jsb_PlatformHelper_class->setProperty = JS_StrictPropertyStub;
    jsb_PlatformHelper_class->enumerate = JS_EnumerateStub;
    jsb_PlatformHelper_class->resolve = JS_ResolveStub;
    jsb_PlatformHelper_class->convert = JS_ConvertStub;
    jsb_PlatformHelper_class->flags = JSCLASS_HAS_RESERVED_SLOTS(2);

    static JSPropertySpec properties[] = {
        JS_PS_END
    };

    static JSFunctionSpec funcs[] = {
        JS_FS_END
    };

    static JSFunctionSpec st_funcs[] = {
        JS_FN("showToast", js_lin_PlatformHelper_showToast, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FS_END
    };

    jsb_PlatformHelper_prototype = JS_InitClass(
        cx, global,
        JS::NullPtr(),
        jsb_PlatformHelper_class,
        dummy_constructor<PlatformHelper>, 0, // no constructor
        properties,
        funcs,
        NULL, // no static properties
        st_funcs);

    JS::RootedObject proto(cx, jsb_PlatformHelper_prototype);
    JS::RootedValue className(cx, std_string_to_jsval(cx, "PlatformHelper"));
    JS_SetProperty(cx, proto, "_className", className);
    JS_SetProperty(cx, proto, "__nativeObj", JS::TrueHandleValue);
    JS_SetProperty(cx, proto, "__is_ref", JS::FalseHandleValue);
    // add the proto and JSClass to the type->js info hash table
    jsb_register_class<PlatformHelper>(cx, jsb_PlatformHelper_class, proto, JS::NullPtr());
}

JSClass  *jsb_CommonModel_class;
JSObject *jsb_CommonModel_prototype;

bool js_lin_CommonModel_hallToRoomV(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    CommonModel* cobj = (CommonModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_lin_CommonModel_hallToRoomV : Invalid Native Object");
    if (argc == 0) {
        cobj->hallToRoomV();
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_lin_CommonModel_hallToRoomV : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_lin_CommonModel_getPackName(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    CommonModel* cobj = (CommonModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_lin_CommonModel_getPackName : Invalid Native Object");
    if (argc == 0) {
        std::string ret = cobj->getPackName();
        jsval jsret = JSVAL_NULL;
        jsret = std_string_to_jsval(cx, ret);
        args.rval().set(jsret);
        return true;
    }

    JS_ReportError(cx, "js_lin_CommonModel_getPackName : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_lin_CommonModel_toHall(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    CommonModel* cobj = (CommonModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_lin_CommonModel_toHall : Invalid Native Object");
    if (argc == 0) {
        cobj->toHall();
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_lin_CommonModel_toHall : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_lin_CommonModel_setRoomData(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    CommonModel* cobj = (CommonModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_lin_CommonModel_setRoomData : Invalid Native Object");
    if (argc == 1) {
        std::string arg0;
        ok &= jsval_to_std_string(cx, args.get(0), &arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_lin_CommonModel_setRoomData : Error processing arguments");
        cobj->setRoomData(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_lin_CommonModel_setRoomData : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_lin_CommonModel_init(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    CommonModel* cobj = (CommonModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_lin_CommonModel_init : Invalid Native Object");
    if (argc == 0) {
        cobj->init();
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_lin_CommonModel_init : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_lin_CommonModel_getRoomData(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    CommonModel* cobj = (CommonModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_lin_CommonModel_getRoomData : Invalid Native Object");
    if (argc == 0) {
        std::string ret = cobj->getRoomData();
        jsval jsret = JSVAL_NULL;
        jsret = std_string_to_jsval(cx, ret);
        args.rval().set(jsret);
        return true;
    }

    JS_ReportError(cx, "js_lin_CommonModel_getRoomData : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_lin_CommonModel_hallToRoomH(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    CommonModel* cobj = (CommonModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_lin_CommonModel_hallToRoomH : Invalid Native Object");
    if (argc == 0) {
        cobj->hallToRoomH();
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_lin_CommonModel_hallToRoomH : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_lin_CommonModel_getInstance(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    if (argc == 0) {

        auto ret = CommonModel::getInstance();
        js_type_class_t *typeClass = js_get_type_from_native<CommonModel>(ret);
        JS::RootedObject jsret(cx, jsb_ref_get_or_create_jsobject(cx, ret, typeClass, "CommonModel"));
        args.rval().set(OBJECT_TO_JSVAL(jsret));
        return true;
    }
    JS_ReportError(cx, "js_lin_CommonModel_getInstance : wrong number of arguments");
    return false;
}

bool js_lin_CommonModel_constructor(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    CommonModel* cobj = new (std::nothrow) CommonModel();

    js_type_class_t *typeClass = js_get_type_from_native<CommonModel>(cobj);

    // link the native object with the javascript object
    JS::RootedObject jsobj(cx, jsb_ref_create_jsobject(cx, cobj, typeClass, "CommonModel"));
    args.rval().set(OBJECT_TO_JSVAL(jsobj));
    if (JS_HasProperty(cx, jsobj, "_ctor", &ok) && ok)
        ScriptingCore::getInstance()->executeFunctionWithOwner(OBJECT_TO_JSVAL(jsobj), "_ctor", args);
    return true;
}


void js_register_lin_CommonModel(JSContext *cx, JS::HandleObject global) {
    jsb_CommonModel_class = (JSClass *)calloc(1, sizeof(JSClass));
    jsb_CommonModel_class->name = "CommonModel";
    jsb_CommonModel_class->addProperty = JS_PropertyStub;
    jsb_CommonModel_class->delProperty = JS_DeletePropertyStub;
    jsb_CommonModel_class->getProperty = JS_PropertyStub;
    jsb_CommonModel_class->setProperty = JS_StrictPropertyStub;
    jsb_CommonModel_class->enumerate = JS_EnumerateStub;
    jsb_CommonModel_class->resolve = JS_ResolveStub;
    jsb_CommonModel_class->convert = JS_ConvertStub;
    jsb_CommonModel_class->flags = JSCLASS_HAS_RESERVED_SLOTS(2);

    static JSPropertySpec properties[] = {
        JS_PS_END
    };

    static JSFunctionSpec funcs[] = {
        JS_FN("hallToRoomV", js_lin_CommonModel_hallToRoomV, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("getPackName", js_lin_CommonModel_getPackName, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("toHall", js_lin_CommonModel_toHall, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("setRoomData", js_lin_CommonModel_setRoomData, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("init", js_lin_CommonModel_init, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("getRoomData", js_lin_CommonModel_getRoomData, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("hallToRoomH", js_lin_CommonModel_hallToRoomH, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FS_END
    };

    static JSFunctionSpec st_funcs[] = {
        JS_FN("getInstance", js_lin_CommonModel_getInstance, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FS_END
    };

    jsb_CommonModel_prototype = JS_InitClass(
        cx, global,
        JS::NullPtr(),
        jsb_CommonModel_class,
        js_lin_CommonModel_constructor, 0, // constructor
        properties,
        funcs,
        NULL, // no static properties
        st_funcs);

    JS::RootedObject proto(cx, jsb_CommonModel_prototype);
    JS::RootedValue className(cx, std_string_to_jsval(cx, "CommonModel"));
    JS_SetProperty(cx, proto, "_className", className);
    JS_SetProperty(cx, proto, "__nativeObj", JS::TrueHandleValue);
    JS_SetProperty(cx, proto, "__is_ref", JS::TrueHandleValue);
    // add the proto and JSClass to the type->js info hash table
    jsb_register_class<CommonModel>(cx, jsb_CommonModel_class, proto, JS::NullPtr());
}

JSClass  *jsb_UserModel_class;
JSObject *jsb_UserModel_prototype;

bool js_lin_UserModel_setUserName(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    UserModel* cobj = (UserModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_lin_UserModel_setUserName : Invalid Native Object");
    if (argc == 1) {
        std::string arg0;
        ok &= jsval_to_std_string(cx, args.get(0), &arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_lin_UserModel_setUserName : Error processing arguments");
        cobj->setUserName(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_lin_UserModel_setUserName : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_lin_UserModel_getCurRoomId(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    UserModel* cobj = (UserModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_lin_UserModel_getCurRoomId : Invalid Native Object");
    if (argc == 0) {
        std::string ret = cobj->getCurRoomId();
        jsval jsret = JSVAL_NULL;
        jsret = std_string_to_jsval(cx, ret);
        args.rval().set(jsret);
        return true;
    }

    JS_ReportError(cx, "js_lin_UserModel_getCurRoomId : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_lin_UserModel_setCurRoomId(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    UserModel* cobj = (UserModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_lin_UserModel_setCurRoomId : Invalid Native Object");
    if (argc == 1) {
        std::string arg0;
        ok &= jsval_to_std_string(cx, args.get(0), &arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_lin_UserModel_setCurRoomId : Error processing arguments");
        cobj->setCurRoomId(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_lin_UserModel_setCurRoomId : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_lin_UserModel_getImsi(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    UserModel* cobj = (UserModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_lin_UserModel_getImsi : Invalid Native Object");
    if (argc == 0) {
        std::string ret = cobj->getImsi();
        jsval jsret = JSVAL_NULL;
        jsret = std_string_to_jsval(cx, ret);
        args.rval().set(jsret);
        return true;
    }

    JS_ReportError(cx, "js_lin_UserModel_getImsi : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_lin_UserModel_setChannel(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    UserModel* cobj = (UserModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_lin_UserModel_setChannel : Invalid Native Object");
    if (argc == 1) {
        std::string arg0;
        ok &= jsval_to_std_string(cx, args.get(0), &arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_lin_UserModel_setChannel : Error processing arguments");
        cobj->setChannel(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_lin_UserModel_setChannel : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_lin_UserModel_getSex(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    UserModel* cobj = (UserModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_lin_UserModel_getSex : Invalid Native Object");
    if (argc == 0) {
        int ret = cobj->getSex();
        jsval jsret = JSVAL_NULL;
        jsret = int32_to_jsval(cx, ret);
        args.rval().set(jsret);
        return true;
    }

    JS_ReportError(cx, "js_lin_UserModel_getSex : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_lin_UserModel_setImsi(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    UserModel* cobj = (UserModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_lin_UserModel_setImsi : Invalid Native Object");
    if (argc == 1) {
        std::string arg0;
        ok &= jsval_to_std_string(cx, args.get(0), &arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_lin_UserModel_setImsi : Error processing arguments");
        cobj->setImsi(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_lin_UserModel_setImsi : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_lin_UserModel_getMoney(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    UserModel* cobj = (UserModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_lin_UserModel_getMoney : Invalid Native Object");
    if (argc == 0) {
        int ret = cobj->getMoney();
        jsval jsret = JSVAL_NULL;
        jsret = int32_to_jsval(cx, ret);
        args.rval().set(jsret);
        return true;
    }

    JS_ReportError(cx, "js_lin_UserModel_getMoney : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_lin_UserModel_getLevel(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    UserModel* cobj = (UserModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_lin_UserModel_getLevel : Invalid Native Object");
    if (argc == 0) {
        int ret = cobj->getLevel();
        jsval jsret = JSVAL_NULL;
        jsret = int32_to_jsval(cx, ret);
        args.rval().set(jsret);
        return true;
    }

    JS_ReportError(cx, "js_lin_UserModel_getLevel : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_lin_UserModel_getIp(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    UserModel* cobj = (UserModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_lin_UserModel_getIp : Invalid Native Object");
    if (argc == 0) {
        std::string ret = cobj->getIp();
        jsval jsret = JSVAL_NULL;
        jsret = std_string_to_jsval(cx, ret);
        args.rval().set(jsret);
        return true;
    }

    JS_ReportError(cx, "js_lin_UserModel_getIp : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_lin_UserModel_getAccount(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    UserModel* cobj = (UserModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_lin_UserModel_getAccount : Invalid Native Object");
    if (argc == 0) {
        std::string ret = cobj->getAccount();
        jsval jsret = JSVAL_NULL;
        jsret = std_string_to_jsval(cx, ret);
        args.rval().set(jsret);
        return true;
    }

    JS_ReportError(cx, "js_lin_UserModel_getAccount : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_lin_UserModel_getUid(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    UserModel* cobj = (UserModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_lin_UserModel_getUid : Invalid Native Object");
    if (argc == 0) {
        int ret = cobj->getUid();
        jsval jsret = JSVAL_NULL;
        jsret = int32_to_jsval(cx, ret);
        args.rval().set(jsret);
        return true;
    }

    JS_ReportError(cx, "js_lin_UserModel_getUid : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_lin_UserModel_getUserName(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    UserModel* cobj = (UserModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_lin_UserModel_getUserName : Invalid Native Object");
    if (argc == 0) {
        std::string ret = cobj->getUserName();
        jsval jsret = JSVAL_NULL;
        jsret = std_string_to_jsval(cx, ret);
        args.rval().set(jsret);
        return true;
    }

    JS_ReportError(cx, "js_lin_UserModel_getUserName : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_lin_UserModel_setLevel(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    UserModel* cobj = (UserModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_lin_UserModel_setLevel : Invalid Native Object");
    if (argc == 1) {
        int arg0 = 0;
        ok &= jsval_to_int32(cx, args.get(0), (int32_t *)&arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_lin_UserModel_setLevel : Error processing arguments");
        cobj->setLevel(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_lin_UserModel_setLevel : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_lin_UserModel_setAccount(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    UserModel* cobj = (UserModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_lin_UserModel_setAccount : Invalid Native Object");
    if (argc == 1) {
        std::string arg0;
        ok &= jsval_to_std_string(cx, args.get(0), &arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_lin_UserModel_setAccount : Error processing arguments");
        cobj->setAccount(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_lin_UserModel_setAccount : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_lin_UserModel_setExp(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    UserModel* cobj = (UserModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_lin_UserModel_setExp : Invalid Native Object");
    if (argc == 1) {
        int arg0 = 0;
        ok &= jsval_to_int32(cx, args.get(0), (int32_t *)&arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_lin_UserModel_setExp : Error processing arguments");
        cobj->setExp(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_lin_UserModel_setExp : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_lin_UserModel_getChannel(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    UserModel* cobj = (UserModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_lin_UserModel_getChannel : Invalid Native Object");
    if (argc == 0) {
        std::string ret = cobj->getChannel();
        jsval jsret = JSVAL_NULL;
        jsret = std_string_to_jsval(cx, ret);
        args.rval().set(jsret);
        return true;
    }

    JS_ReportError(cx, "js_lin_UserModel_getChannel : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_lin_UserModel_getExp(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    UserModel* cobj = (UserModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_lin_UserModel_getExp : Invalid Native Object");
    if (argc == 0) {
        int ret = cobj->getExp();
        jsval jsret = JSVAL_NULL;
        jsret = int32_to_jsval(cx, ret);
        args.rval().set(jsret);
        return true;
    }

    JS_ReportError(cx, "js_lin_UserModel_getExp : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_lin_UserModel_setUid(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    UserModel* cobj = (UserModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_lin_UserModel_setUid : Invalid Native Object");
    if (argc == 1) {
        int arg0 = 0;
        ok &= jsval_to_int32(cx, args.get(0), (int32_t *)&arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_lin_UserModel_setUid : Error processing arguments");
        cobj->setUid(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_lin_UserModel_setUid : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_lin_UserModel_setMoney(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    UserModel* cobj = (UserModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_lin_UserModel_setMoney : Invalid Native Object");
    if (argc == 1) {
        int arg0 = 0;
        ok &= jsval_to_int32(cx, args.get(0), (int32_t *)&arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_lin_UserModel_setMoney : Error processing arguments");
        cobj->setMoney(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_lin_UserModel_setMoney : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_lin_UserModel_setSex(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    UserModel* cobj = (UserModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_lin_UserModel_setSex : Invalid Native Object");
    if (argc == 1) {
        int arg0 = 0;
        ok &= jsval_to_int32(cx, args.get(0), (int32_t *)&arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_lin_UserModel_setSex : Error processing arguments");
        cobj->setSex(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_lin_UserModel_setSex : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_lin_UserModel_setIp(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    UserModel* cobj = (UserModel *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_lin_UserModel_setIp : Invalid Native Object");
    if (argc == 1) {
        std::string arg0;
        ok &= jsval_to_std_string(cx, args.get(0), &arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_lin_UserModel_setIp : Error processing arguments");
        cobj->setIp(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_lin_UserModel_setIp : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_lin_UserModel_getInstance(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    if (argc == 0) {

        auto ret = UserModel::getInstance();
        js_type_class_t *typeClass = js_get_type_from_native<UserModel>(ret);
        JS::RootedObject jsret(cx, jsb_ref_get_or_create_jsobject(cx, ret, typeClass, "UserModel"));
        args.rval().set(OBJECT_TO_JSVAL(jsret));
        return true;
    }
    JS_ReportError(cx, "js_lin_UserModel_getInstance : wrong number of arguments");
    return false;
}

bool js_lin_UserModel_constructor(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    UserModel* cobj = new (std::nothrow) UserModel();

    js_type_class_t *typeClass = js_get_type_from_native<UserModel>(cobj);

    // link the native object with the javascript object
    JS::RootedObject jsobj(cx, jsb_ref_create_jsobject(cx, cobj, typeClass, "UserModel"));
    args.rval().set(OBJECT_TO_JSVAL(jsobj));
    if (JS_HasProperty(cx, jsobj, "_ctor", &ok) && ok)
        ScriptingCore::getInstance()->executeFunctionWithOwner(OBJECT_TO_JSVAL(jsobj), "_ctor", args);
    return true;
}


void js_register_lin_UserModel(JSContext *cx, JS::HandleObject global) {
    jsb_UserModel_class = (JSClass *)calloc(1, sizeof(JSClass));
    jsb_UserModel_class->name = "UserModel";
    jsb_UserModel_class->addProperty = JS_PropertyStub;
    jsb_UserModel_class->delProperty = JS_DeletePropertyStub;
    jsb_UserModel_class->getProperty = JS_PropertyStub;
    jsb_UserModel_class->setProperty = JS_StrictPropertyStub;
    jsb_UserModel_class->enumerate = JS_EnumerateStub;
    jsb_UserModel_class->resolve = JS_ResolveStub;
    jsb_UserModel_class->convert = JS_ConvertStub;
    jsb_UserModel_class->flags = JSCLASS_HAS_RESERVED_SLOTS(2);

    static JSPropertySpec properties[] = {
        JS_PS_END
    };

    static JSFunctionSpec funcs[] = {
        JS_FN("setUserName", js_lin_UserModel_setUserName, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("getCurRoomId", js_lin_UserModel_getCurRoomId, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("setCurRoomId", js_lin_UserModel_setCurRoomId, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("getImsi", js_lin_UserModel_getImsi, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("setChannel", js_lin_UserModel_setChannel, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("getSex", js_lin_UserModel_getSex, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("setImsi", js_lin_UserModel_setImsi, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("getMoney", js_lin_UserModel_getMoney, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("getLevel", js_lin_UserModel_getLevel, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("getIp", js_lin_UserModel_getIp, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("getAccount", js_lin_UserModel_getAccount, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("getUid", js_lin_UserModel_getUid, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("getUserName", js_lin_UserModel_getUserName, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("setLevel", js_lin_UserModel_setLevel, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("setAccount", js_lin_UserModel_setAccount, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("setExp", js_lin_UserModel_setExp, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("getChannel", js_lin_UserModel_getChannel, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("getExp", js_lin_UserModel_getExp, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("setUid", js_lin_UserModel_setUid, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("setMoney", js_lin_UserModel_setMoney, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("setSex", js_lin_UserModel_setSex, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("setIp", js_lin_UserModel_setIp, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FS_END
    };

    static JSFunctionSpec st_funcs[] = {
        JS_FN("getInstance", js_lin_UserModel_getInstance, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FS_END
    };

    jsb_UserModel_prototype = JS_InitClass(
        cx, global,
        JS::NullPtr(),
        jsb_UserModel_class,
        js_lin_UserModel_constructor, 0, // constructor
        properties,
        funcs,
        NULL, // no static properties
        st_funcs);

    JS::RootedObject proto(cx, jsb_UserModel_prototype);
    JS::RootedValue className(cx, std_string_to_jsval(cx, "UserModel"));
    JS_SetProperty(cx, proto, "_className", className);
    JS_SetProperty(cx, proto, "__nativeObj", JS::TrueHandleValue);
    JS_SetProperty(cx, proto, "__is_ref", JS::TrueHandleValue);
    // add the proto and JSClass to the type->js info hash table
    jsb_register_class<UserModel>(cx, jsb_UserModel_class, proto, JS::NullPtr());
}

JSClass  *jsb_SocketRequest_class;
JSObject *jsb_SocketRequest_prototype;


void js_register_lin_SocketRequest(JSContext *cx, JS::HandleObject global) {
    jsb_SocketRequest_class = (JSClass *)calloc(1, sizeof(JSClass));
    jsb_SocketRequest_class->name = "SocketRequest";
    jsb_SocketRequest_class->addProperty = JS_PropertyStub;
    jsb_SocketRequest_class->delProperty = JS_DeletePropertyStub;
    jsb_SocketRequest_class->getProperty = JS_PropertyStub;
    jsb_SocketRequest_class->setProperty = JS_StrictPropertyStub;
    jsb_SocketRequest_class->enumerate = JS_EnumerateStub;
    jsb_SocketRequest_class->resolve = JS_ResolveStub;
    jsb_SocketRequest_class->convert = JS_ConvertStub;
    jsb_SocketRequest_class->flags = JSCLASS_HAS_RESERVED_SLOTS(2);

    static JSPropertySpec properties[] = {
        JS_PS_END
    };

    static JSFunctionSpec funcs[] = {
        JS_FS_END
    };

    JSFunctionSpec *st_funcs = NULL;

    jsb_SocketRequest_prototype = JS_InitClass(
        cx, global,
        JS::NullPtr(),
        jsb_SocketRequest_class,
        dummy_constructor<SocketRequest>, 0, // no constructor
        properties,
        funcs,
        NULL, // no static properties
        st_funcs);

    JS::RootedObject proto(cx, jsb_SocketRequest_prototype);
    JS::RootedValue className(cx, std_string_to_jsval(cx, "SocketRequest"));
    JS_SetProperty(cx, proto, "_className", className);
    JS_SetProperty(cx, proto, "__nativeObj", JS::TrueHandleValue);
    JS_SetProperty(cx, proto, "__is_ref", JS::FalseHandleValue);
    // add the proto and JSClass to the type->js info hash table
    jsb_register_class<SocketRequest>(cx, jsb_SocketRequest_class, proto, JS::NullPtr());
}

JSClass  *jsb_SocketResponse_class;
JSObject *jsb_SocketResponse_prototype;

bool js_lin_SocketResponse_release(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    SocketResponse* cobj = (SocketResponse *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_lin_SocketResponse_release : Invalid Native Object");
    if (argc == 0) {
        cobj->release();
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_lin_SocketResponse_release : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_lin_SocketResponse_parseDatta(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    SocketResponse* cobj = (SocketResponse *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_lin_SocketResponse_parseDatta : Invalid Native Object");
    if (argc == 0) {
        std::string ret = cobj->parseDatta();
        jsval jsret = JSVAL_NULL;
        jsret = std_string_to_jsval(cx, ret);
        args.rval().set(jsret);
        return true;
    }

    JS_ReportError(cx, "js_lin_SocketResponse_parseDatta : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_lin_SocketResponse_get_cmd(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    SocketResponse* cobj = (SocketResponse *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_lin_SocketResponse_get_cmd : Invalid Native Object");

    jsval jsret = int32_to_jsval(cx, cobj->cmd);
    args.rval().set(jsret);
    return true;
}
bool js_lin_SocketResponse_set_cmd(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    SocketResponse* cobj = (SocketResponse *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_lin_SocketResponse_set_cmd : Invalid Native Object");

    bool ok = true;
    int arg0 = 0;
    ok &= jsval_to_int32(cx, args.get(0), (int32_t *)&arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_lin_SocketResponse_set_cmd : Error processing new value");
    cobj->cmd = arg0;
    return true;
}
bool js_lin_SocketResponse_get_data(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    SocketResponse* cobj = (SocketResponse *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_lin_SocketResponse_get_data : Invalid Native Object");

    jsval jsret = std_string_to_jsval(cx, cobj->data);
    args.rval().set(jsret);
    return true;
}
bool js_lin_SocketResponse_set_data(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    SocketResponse* cobj = (SocketResponse *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_lin_SocketResponse_set_data : Invalid Native Object");

    bool ok = true;
    std::string arg0;
    ok &= jsval_to_std_string(cx, args.get(0), &arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_lin_SocketResponse_set_data : Error processing new value");
    cobj->data = arg0;
    return true;
}
bool js_lin_SocketResponse_get_id(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    SocketResponse* cobj = (SocketResponse *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_lin_SocketResponse_get_id : Invalid Native Object");

    jsval jsret = int32_to_jsval(cx, cobj->id);
    args.rval().set(jsret);
    return true;
}
bool js_lin_SocketResponse_set_id(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    SocketResponse* cobj = (SocketResponse *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_lin_SocketResponse_set_id : Invalid Native Object");

    bool ok = true;
    int arg0 = 0;
    ok &= jsval_to_int32(cx, args.get(0), (int32_t *)&arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_lin_SocketResponse_set_id : Error processing new value");
    cobj->id = arg0;
    return true;
}
bool js_lin_SocketResponse_get_seqNo(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    SocketResponse* cobj = (SocketResponse *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_lin_SocketResponse_get_seqNo : Invalid Native Object");

    jsval jsret = int32_to_jsval(cx, cobj->seqNo);
    args.rval().set(jsret);
    return true;
}
bool js_lin_SocketResponse_set_seqNo(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    SocketResponse* cobj = (SocketResponse *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_lin_SocketResponse_set_seqNo : Invalid Native Object");

    bool ok = true;
    int arg0 = 0;
    ok &= jsval_to_int32(cx, args.get(0), (int32_t *)&arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_lin_SocketResponse_set_seqNo : Error processing new value");
    cobj->seqNo = arg0;
    return true;
}
bool js_lin_SocketResponse_get_svrid(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    SocketResponse* cobj = (SocketResponse *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_lin_SocketResponse_get_svrid : Invalid Native Object");

    jsval jsret = int32_to_jsval(cx, cobj->svrid);
    args.rval().set(jsret);
    return true;
}
bool js_lin_SocketResponse_set_svrid(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    SocketResponse* cobj = (SocketResponse *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_lin_SocketResponse_set_svrid : Invalid Native Object");

    bool ok = true;
    int arg0 = 0;
    ok &= jsval_to_int32(cx, args.get(0), (int32_t *)&arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_lin_SocketResponse_set_svrid : Error processing new value");
    cobj->svrid = arg0;
    return true;
}
bool js_lin_SocketResponse_get_time(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    SocketResponse* cobj = (SocketResponse *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_lin_SocketResponse_get_time : Invalid Native Object");

    jsval jsret = int32_to_jsval(cx, cobj->time);
    args.rval().set(jsret);
    return true;
}
bool js_lin_SocketResponse_set_time(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject jsthis(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(jsthis);
    SocketResponse* cobj = (SocketResponse *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_lin_SocketResponse_set_time : Invalid Native Object");

    bool ok = true;
    int arg0 = 0;
    ok &= jsval_to_int32(cx, args.get(0), (int32_t *)&arg0);
    JSB_PRECONDITION2(ok, cx, false, "js_lin_SocketResponse_set_time : Error processing new value");
    cobj->time = arg0;
    return true;
}

void js_register_lin_SocketResponse(JSContext *cx, JS::HandleObject global) {
    jsb_SocketResponse_class = (JSClass *)calloc(1, sizeof(JSClass));
    jsb_SocketResponse_class->name = "SocketResponse";
    jsb_SocketResponse_class->addProperty = JS_PropertyStub;
    jsb_SocketResponse_class->delProperty = JS_DeletePropertyStub;
    jsb_SocketResponse_class->getProperty = JS_PropertyStub;
    jsb_SocketResponse_class->setProperty = JS_StrictPropertyStub;
    jsb_SocketResponse_class->enumerate = JS_EnumerateStub;
    jsb_SocketResponse_class->resolve = JS_ResolveStub;
    jsb_SocketResponse_class->convert = JS_ConvertStub;
    jsb_SocketResponse_class->flags = JSCLASS_HAS_RESERVED_SLOTS(2);

    static JSPropertySpec properties[] = {
        JS_PSGS("cmd", js_lin_SocketResponse_get_cmd, js_lin_SocketResponse_set_cmd, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_PSGS("data", js_lin_SocketResponse_get_data, js_lin_SocketResponse_set_data, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_PSGS("id", js_lin_SocketResponse_get_id, js_lin_SocketResponse_set_id, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_PSGS("seqNo", js_lin_SocketResponse_get_seqNo, js_lin_SocketResponse_set_seqNo, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_PSGS("svrid", js_lin_SocketResponse_get_svrid, js_lin_SocketResponse_set_svrid, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_PSGS("time", js_lin_SocketResponse_get_time, js_lin_SocketResponse_set_time, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_PS_END
    };

    static JSFunctionSpec funcs[] = {
        JS_FN("release", js_lin_SocketResponse_release, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("parseDatta", js_lin_SocketResponse_parseDatta, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FS_END
    };

    JSFunctionSpec *st_funcs = NULL;

    jsb_SocketResponse_prototype = JS_InitClass(
        cx, global,
        JS::NullPtr(),
        jsb_SocketResponse_class,
        dummy_constructor<SocketResponse>, 0, // no constructor
        properties,
        funcs,
        NULL, // no static properties
        st_funcs);

    JS::RootedObject proto(cx, jsb_SocketResponse_prototype);
    JS::RootedValue className(cx, std_string_to_jsval(cx, "SocketResponse"));
    JS_SetProperty(cx, proto, "_className", className);
    JS_SetProperty(cx, proto, "__nativeObj", JS::TrueHandleValue);
    JS_SetProperty(cx, proto, "__is_ref", JS::FalseHandleValue);
    // add the proto and JSClass to the type->js info hash table
    jsb_register_class<SocketResponse>(cx, jsb_SocketResponse_class, proto, JS::NullPtr());
}

JSClass  *jsb_SocketClient_class;
JSObject *jsb_SocketClient_prototype;

bool js_lin_SocketClient_connect_req(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    SocketClient* cobj = (SocketClient *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_lin_SocketClient_connect_req : Invalid Native Object");
    if (argc == 0) {
        cobj->connect_req();
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_lin_SocketClient_connect_req : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_lin_SocketClient_getJsonStr(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    SocketClient* cobj = (SocketClient *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_lin_SocketClient_getJsonStr : Invalid Native Object");
    if (argc == 1) {
        SocketRequest* arg0 = nullptr;
        do {
            if (args.get(0).isNull()) { arg0 = nullptr; break; }
            if (!args.get(0).isObject()) { ok = false; break; }
            js_proxy_t *jsProxy;
            JS::RootedObject tmpObj(cx, args.get(0).toObjectOrNull());
            jsProxy = jsb_get_js_proxy(tmpObj);
            arg0 = (SocketRequest*)(jsProxy ? jsProxy->ptr : NULL);
            JSB_PRECONDITION2( arg0, cx, false, "Invalid Native Object");
        } while (0);
        JSB_PRECONDITION2(ok, cx, false, "js_lin_SocketClient_getJsonStr : Error processing arguments");
        std::string ret = cobj->getJsonStr(arg0);
        jsval jsret = JSVAL_NULL;
        jsret = std_string_to_jsval(cx, ret);
        args.rval().set(jsret);
        return true;
    }

    JS_ReportError(cx, "js_lin_SocketClient_getJsonStr : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_lin_SocketClient_seCurRoomId(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    SocketClient* cobj = (SocketClient *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_lin_SocketClient_seCurRoomId : Invalid Native Object");
    if (argc == 1) {
        std::string arg0;
        ok &= jsval_to_std_string(cx, args.get(0), &arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_lin_SocketClient_seCurRoomId : Error processing arguments");
        cobj->seCurRoomId(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_lin_SocketClient_seCurRoomId : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_lin_SocketClient_setConnectState(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    SocketClient* cobj = (SocketClient *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_lin_SocketClient_setConnectState : Invalid Native Object");
    if (argc == 1) {
        bool arg0;
        arg0 = JS::ToBoolean(args.get(0));
        JSB_PRECONDITION2(ok, cx, false, "js_lin_SocketClient_setConnectState : Error processing arguments");
        cobj->setConnectState(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_lin_SocketClient_setConnectState : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_lin_SocketClient_get(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    SocketClient* cobj = (SocketClient *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_lin_SocketClient_get : Invalid Native Object");
    if (argc == 0) {
        SocketResponse* ret = cobj->get();
        jsval jsret = JSVAL_NULL;
        if (ret) {
            jsret = OBJECT_TO_JSVAL(js_get_or_create_jsobject<SocketResponse>(cx, (SocketResponse*)ret));
        } else {
            jsret = JSVAL_NULL;
        };
        args.rval().set(jsret);
        return true;
    }

    JS_ReportError(cx, "js_lin_SocketClient_get : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_lin_SocketClient_onError(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    SocketClient* cobj = (SocketClient *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_lin_SocketClient_onError : Invalid Native Object");
    if (argc == 2) {
        cocos2d::network::SIOClient* arg0 = nullptr;
        std::string arg1;
        do {
            if (args.get(0).isNull()) { arg0 = nullptr; break; }
            if (!args.get(0).isObject()) { ok = false; break; }
            js_proxy_t *jsProxy;
            JS::RootedObject tmpObj(cx, args.get(0).toObjectOrNull());
            jsProxy = jsb_get_js_proxy(tmpObj);
            arg0 = (cocos2d::network::SIOClient*)(jsProxy ? jsProxy->ptr : NULL);
            JSB_PRECONDITION2( arg0, cx, false, "Invalid Native Object");
        } while (0);
        ok &= jsval_to_std_string(cx, args.get(1), &arg1);
        JSB_PRECONDITION2(ok, cx, false, "js_lin_SocketClient_onError : Error processing arguments");
        cobj->onError(arg0, arg1);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_lin_SocketClient_onError : wrong number of arguments: %d, was expecting %d", argc, 2);
    return false;
}
bool js_lin_SocketClient_onClose(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    SocketClient* cobj = (SocketClient *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_lin_SocketClient_onClose : Invalid Native Object");
    if (argc == 1) {
        cocos2d::network::SIOClient* arg0 = nullptr;
        do {
            if (args.get(0).isNull()) { arg0 = nullptr; break; }
            if (!args.get(0).isObject()) { ok = false; break; }
            js_proxy_t *jsProxy;
            JS::RootedObject tmpObj(cx, args.get(0).toObjectOrNull());
            jsProxy = jsb_get_js_proxy(tmpObj);
            arg0 = (cocos2d::network::SIOClient*)(jsProxy ? jsProxy->ptr : NULL);
            JSB_PRECONDITION2( arg0, cx, false, "Invalid Native Object");
        } while (0);
        JSB_PRECONDITION2(ok, cx, false, "js_lin_SocketClient_onClose : Error processing arguments");
        cobj->onClose(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_lin_SocketClient_onClose : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_lin_SocketClient_getCurRoomId(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    SocketClient* cobj = (SocketClient *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_lin_SocketClient_getCurRoomId : Invalid Native Object");
    if (argc == 0) {
        std::string ret = cobj->getCurRoomId();
        jsval jsret = JSVAL_NULL;
        jsret = std_string_to_jsval(cx, ret);
        args.rval().set(jsret);
        return true;
    }

    JS_ReportError(cx, "js_lin_SocketClient_getCurRoomId : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_lin_SocketClient_create_response(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    SocketClient* cobj = (SocketClient *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_lin_SocketClient_create_response : Invalid Native Object");
    if (argc == 2) {
        int arg0 = 0;
        std::string arg1;
        ok &= jsval_to_int32(cx, args.get(0), (int32_t *)&arg0);
        ok &= jsval_to_std_string(cx, args.get(1), &arg1);
        JSB_PRECONDITION2(ok, cx, false, "js_lin_SocketClient_create_response : Error processing arguments");
        cobj->create_response(arg0, arg1);
        args.rval().setUndefined();
        return true;
    }
    if (argc == 3) {
        int arg0 = 0;
        std::string arg1;
        int arg2 = 0;
        ok &= jsval_to_int32(cx, args.get(0), (int32_t *)&arg0);
        ok &= jsval_to_std_string(cx, args.get(1), &arg1);
        ok &= jsval_to_int32(cx, args.get(2), (int32_t *)&arg2);
        JSB_PRECONDITION2(ok, cx, false, "js_lin_SocketClient_create_response : Error processing arguments");
        cobj->create_response(arg0, arg1, arg2);
        args.rval().setUndefined();
        return true;
    }
    if (argc == 4) {
        int arg0 = 0;
        std::string arg1;
        int arg2 = 0;
        int arg3 = 0;
        ok &= jsval_to_int32(cx, args.get(0), (int32_t *)&arg0);
        ok &= jsval_to_std_string(cx, args.get(1), &arg1);
        ok &= jsval_to_int32(cx, args.get(2), (int32_t *)&arg2);
        ok &= jsval_to_int32(cx, args.get(3), (int32_t *)&arg3);
        JSB_PRECONDITION2(ok, cx, false, "js_lin_SocketClient_create_response : Error processing arguments");
        cobj->create_response(arg0, arg1, arg2, arg3);
        args.rval().setUndefined();
        return true;
    }
    if (argc == 5) {
        int arg0 = 0;
        std::string arg1;
        int arg2 = 0;
        int arg3 = 0;
        int arg4 = 0;
        ok &= jsval_to_int32(cx, args.get(0), (int32_t *)&arg0);
        ok &= jsval_to_std_string(cx, args.get(1), &arg1);
        ok &= jsval_to_int32(cx, args.get(2), (int32_t *)&arg2);
        ok &= jsval_to_int32(cx, args.get(3), (int32_t *)&arg3);
        ok &= jsval_to_int32(cx, args.get(4), (int32_t *)&arg4);
        JSB_PRECONDITION2(ok, cx, false, "js_lin_SocketClient_create_response : Error processing arguments");
        cobj->create_response(arg0, arg1, arg2, arg3, arg4);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_lin_SocketClient_create_response : wrong number of arguments: %d, was expecting %d", argc, 2);
    return false;
}
bool js_lin_SocketClient_reset_response_queue(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    SocketClient* cobj = (SocketClient *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_lin_SocketClient_reset_response_queue : Invalid Native Object");
    if (argc == 0) {
        cobj->reset_response_queue();
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_lin_SocketClient_reset_response_queue : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_lin_SocketClient_init(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    SocketClient* cobj = (SocketClient *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_lin_SocketClient_init : Invalid Native Object");
    if (argc == 0) {
        cobj->init();
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_lin_SocketClient_init : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_lin_SocketClient_isConnect(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    SocketClient* cobj = (SocketClient *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_lin_SocketClient_isConnect : Invalid Native Object");
    if (argc == 0) {
        bool ret = cobj->isConnect();
        jsval jsret = JSVAL_NULL;
        jsret = BOOLEAN_TO_JSVAL(ret);
        args.rval().set(jsret);
        return true;
    }

    JS_ReportError(cx, "js_lin_SocketClient_isConnect : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_lin_SocketClient_send_Data(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    SocketClient* cobj = (SocketClient *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_lin_SocketClient_send_Data : Invalid Native Object");
    if (argc == 2) {
        std::string arg0;
        int arg1 = 0;
        ok &= jsval_to_std_string(cx, args.get(0), &arg0);
        ok &= jsval_to_int32(cx, args.get(1), (int32_t *)&arg1);
        JSB_PRECONDITION2(ok, cx, false, "js_lin_SocketClient_send_Data : Error processing arguments");
        cobj->send_Data(arg0, arg1);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_lin_SocketClient_send_Data : wrong number of arguments: %d, was expecting %d", argc, 2);
    return false;
}
bool js_lin_SocketClient_put(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    SocketClient* cobj = (SocketClient *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_lin_SocketClient_put : Invalid Native Object");
    if (argc == 1) {
        SocketRequest* arg0 = nullptr;
        do {
            if (args.get(0).isNull()) { arg0 = nullptr; break; }
            if (!args.get(0).isObject()) { ok = false; break; }
            js_proxy_t *jsProxy;
            JS::RootedObject tmpObj(cx, args.get(0).toObjectOrNull());
            jsProxy = jsb_get_js_proxy(tmpObj);
            arg0 = (SocketRequest*)(jsProxy ? jsProxy->ptr : NULL);
            JSB_PRECONDITION2( arg0, cx, false, "Invalid Native Object");
        } while (0);
        JSB_PRECONDITION2(ok, cx, false, "js_lin_SocketClient_put : Error processing arguments");
        cobj->put(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_lin_SocketClient_put : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_lin_SocketClient_data_req(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    SocketClient* cobj = (SocketClient *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_lin_SocketClient_data_req : Invalid Native Object");
    if (argc == 1) {
        SocketRequest* arg0 = nullptr;
        do {
            if (args.get(0).isNull()) { arg0 = nullptr; break; }
            if (!args.get(0).isObject()) { ok = false; break; }
            js_proxy_t *jsProxy;
            JS::RootedObject tmpObj(cx, args.get(0).toObjectOrNull());
            jsProxy = jsb_get_js_proxy(tmpObj);
            arg0 = (SocketRequest*)(jsProxy ? jsProxy->ptr : NULL);
            JSB_PRECONDITION2( arg0, cx, false, "Invalid Native Object");
        } while (0);
        JSB_PRECONDITION2(ok, cx, false, "js_lin_SocketClient_data_req : Error processing arguments");
        cobj->data_req(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_lin_SocketClient_data_req : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_lin_SocketClient_disconnect_req(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    SocketClient* cobj = (SocketClient *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_lin_SocketClient_disconnect_req : Invalid Native Object");
    if (argc == 1) {
        SocketRequest* arg0 = nullptr;
        do {
            if (args.get(0).isNull()) { arg0 = nullptr; break; }
            if (!args.get(0).isObject()) { ok = false; break; }
            js_proxy_t *jsProxy;
            JS::RootedObject tmpObj(cx, args.get(0).toObjectOrNull());
            jsProxy = jsb_get_js_proxy(tmpObj);
            arg0 = (SocketRequest*)(jsProxy ? jsProxy->ptr : NULL);
            JSB_PRECONDITION2( arg0, cx, false, "Invalid Native Object");
        } while (0);
        JSB_PRECONDITION2(ok, cx, false, "js_lin_SocketClient_disconnect_req : Error processing arguments");
        cobj->disconnect_req(arg0);
        args.rval().setUndefined();
        return true;
    }

    JS_ReportError(cx, "js_lin_SocketClient_disconnect_req : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_lin_SocketClient_getInstance(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    if (argc == 0) {

        auto ret = SocketClient::getInstance();
        js_type_class_t *typeClass = js_get_type_from_native<SocketClient>(ret);
        JS::RootedObject jsret(cx, jsb_ref_get_or_create_jsobject(cx, ret, typeClass, "SocketClient"));
        args.rval().set(OBJECT_TO_JSVAL(jsret));
        return true;
    }
    if (argc == 1) {
        std::string arg0;
        ok &= jsval_to_std_string(cx, args.get(0), &arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_lin_SocketClient_getInstance : Error processing arguments");

        auto ret = SocketClient::getInstance(arg0);
        js_type_class_t *typeClass = js_get_type_from_native<SocketClient>(ret);
        JS::RootedObject jsret(cx, jsb_ref_get_or_create_jsobject(cx, ret, typeClass, "SocketClient"));
        args.rval().set(OBJECT_TO_JSVAL(jsret));
        return true;
    }
    JS_ReportError(cx, "js_lin_SocketClient_getInstance : wrong number of arguments");
    return false;
}

bool js_lin_SocketClient_constructor(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    SocketClient* cobj = new (std::nothrow) SocketClient();

    js_type_class_t *typeClass = js_get_type_from_native<SocketClient>(cobj);

    // link the native object with the javascript object
    JS::RootedObject jsobj(cx, jsb_ref_create_jsobject(cx, cobj, typeClass, "SocketClient"));
    args.rval().set(OBJECT_TO_JSVAL(jsobj));
    if (JS_HasProperty(cx, jsobj, "_ctor", &ok) && ok)
        ScriptingCore::getInstance()->executeFunctionWithOwner(OBJECT_TO_JSVAL(jsobj), "_ctor", args);
    return true;
}


void js_register_lin_SocketClient(JSContext *cx, JS::HandleObject global) {
    jsb_SocketClient_class = (JSClass *)calloc(1, sizeof(JSClass));
    jsb_SocketClient_class->name = "SocketClient";
    jsb_SocketClient_class->addProperty = JS_PropertyStub;
    jsb_SocketClient_class->delProperty = JS_DeletePropertyStub;
    jsb_SocketClient_class->getProperty = JS_PropertyStub;
    jsb_SocketClient_class->setProperty = JS_StrictPropertyStub;
    jsb_SocketClient_class->enumerate = JS_EnumerateStub;
    jsb_SocketClient_class->resolve = JS_ResolveStub;
    jsb_SocketClient_class->convert = JS_ConvertStub;
    jsb_SocketClient_class->flags = JSCLASS_HAS_RESERVED_SLOTS(2);

    static JSPropertySpec properties[] = {
        JS_PS_END
    };

    static JSFunctionSpec funcs[] = {
        JS_FN("connect_req", js_lin_SocketClient_connect_req, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("getJsonStr", js_lin_SocketClient_getJsonStr, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("seCurRoomId", js_lin_SocketClient_seCurRoomId, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("setConnectState", js_lin_SocketClient_setConnectState, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("get", js_lin_SocketClient_get, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("onError", js_lin_SocketClient_onError, 2, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("onClose", js_lin_SocketClient_onClose, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("getCurRoomId", js_lin_SocketClient_getCurRoomId, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("create_response", js_lin_SocketClient_create_response, 2, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("reset_response_queue", js_lin_SocketClient_reset_response_queue, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("init", js_lin_SocketClient_init, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("isConnect", js_lin_SocketClient_isConnect, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("send_Data", js_lin_SocketClient_send_Data, 2, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("put", js_lin_SocketClient_put, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("data_req", js_lin_SocketClient_data_req, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("disconnect_req", js_lin_SocketClient_disconnect_req, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FS_END
    };

    static JSFunctionSpec st_funcs[] = {
        JS_FN("getInstance", js_lin_SocketClient_getInstance, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FS_END
    };

    jsb_SocketClient_prototype = JS_InitClass(
        cx, global,
        JS::NullPtr(),
        jsb_SocketClient_class,
        js_lin_SocketClient_constructor, 0, // constructor
        properties,
        funcs,
        NULL, // no static properties
        st_funcs);

    JS::RootedObject proto(cx, jsb_SocketClient_prototype);
    JS::RootedValue className(cx, std_string_to_jsval(cx, "SocketClient"));
    JS_SetProperty(cx, proto, "_className", className);
    JS_SetProperty(cx, proto, "__nativeObj", JS::TrueHandleValue);
    JS_SetProperty(cx, proto, "__is_ref", JS::TrueHandleValue);
    // add the proto and JSClass to the type->js info hash table
    jsb_register_class<SocketClient>(cx, jsb_SocketClient_class, proto, JS::NullPtr());
}

void register_all_lin(JSContext* cx, JS::HandleObject obj) {
    // Get the global ns
    JS::RootedObject ns(cx, ScriptingCore::getInstance()->getGlobalObject());

    js_register_lin_SocketRequest(cx, ns);
    js_register_lin_SocketResponse(cx, ns);
    js_register_lin_SocketClient(cx, ns);
    js_register_lin_PlatformHelper(cx, ns);
    js_register_lin_CommonModel(cx, ns);
    js_register_lin_UserModel(cx, ns);
}


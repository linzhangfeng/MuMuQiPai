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
        JS_FN("init", js_lin_CommonModel_init, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("toHall", js_lin_CommonModel_toHall, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
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

void register_all_lin(JSContext* cx, JS::HandleObject obj) {
    // Get the global ns
    JS::RootedObject ns(cx, ScriptingCore::getInstance()->getGlobalObject());

    js_register_lin_CommonModel(cx, ns);
    js_register_lin_PlatformHelper(cx, ns);
}


#include "jsb_soomla_auto.hpp"
#include "cocos2d_specifics.hpp"
#include "jsb_soomla.h"

template<class T>
static bool dummy_constructor(JSContext *cx, uint32_t argc, jsval *vp) {
    JS::RootedValue initializing(cx);
    bool isNewValid = true;
    JSObject* global = ScriptingCore::getInstance()->getGlobalObject();
	isNewValid = JS_GetProperty(cx, global, "initializing", &initializing) && JSVAL_TO_BOOLEAN(initializing);
	if (isNewValid)
	{
		TypeTest<T> t;
		js_type_class_t *typeClass = nullptr;
		std::string typeName = t.s_name();
		auto typeMapIter = _js_global_type_map.find(typeName);
		CCASSERT(typeMapIter != _js_global_type_map.end(), "Can't find the class type!");
		typeClass = typeMapIter->second;
		CCASSERT(typeClass, "The value is null.");

		JSObject *_tmp = JS_NewObject(cx, typeClass->jsclass, typeClass->proto, typeClass->parentProto);
		JS_SET_RVAL(cx, vp, OBJECT_TO_JSVAL(_tmp));
		return true;
	}

    JS_ReportError(cx, "Don't use `new cc.XXX`, please use `cc.XXX.create` instead! ");
    return false;
}

static bool empty_constructor(JSContext *cx, uint32_t argc, jsval *vp) {
	return false;
}

static bool js_is_native_obj(JSContext *cx, JS::HandleObject obj, JS::HandleId id, JS::MutableHandleValue vp)
{
	vp.set(BOOLEAN_TO_JSVAL(true));
	return true;	
}
JSClass  *jsb_Soomla_JSBinding_class;
JSObject *jsb_Soomla_JSBinding_prototype;

bool js_soomla_JSBinding_callNative(JSContext *cx, uint32_t argc, jsval *vp)
{
	jsval *argv = JS_ARGV(cx, vp);
	bool ok = true;
	if (argc == 1) {
		const char* arg0;
		std::string arg0_tmp; ok &= jsval_to_std_string(cx, argv[0], &arg0_tmp); arg0 = arg0_tmp.c_str();
		JSB_PRECONDITION2(ok, cx, false, "js_soomla_JSBinding_callNative : Error processing arguments");
		std::string ret = Soomla::JSBinding::callNative(arg0);
		jsval jsret = JSVAL_NULL;
		jsret = std_string_to_jsval(cx, ret);
		JS_SET_RVAL(cx, vp, jsret);
		return true;
	}
	JS_ReportError(cx, "js_soomla_JSBinding_callNative : wrong number of arguments");
	return false;
}

bool js_soomla_JSBinding_listen(JSContext *cx, uint32_t argc, jsval *vp)
{
	jsval *argv = JS_ARGV(cx, vp);
	bool ok = true;
	if (argc == 1) {
		std::function<void (std::basic_string<char>)> arg0;
		do {
			std::shared_ptr<JSFunctionWrapper> func(new JSFunctionWrapper(cx, JS_THIS_OBJECT(cx, vp), argv[0]));
			auto lambda = [=](std::basic_string<char> larg0) -> void {
				jsval largv[1];
				do {
					if (larg0) {
						js_proxy_t *jsProxy = js_get_or_create_proxy<std::basic_string<char>>(cx, (std::basic_string<char>)larg0);
						largv[0] = OBJECT_TO_JSVAL(jsProxy->obj);
					} else {
						largv[0] = JSVAL_NULL;
					}
				} while (0);
				jsval rval;
				bool ok = func->invoke(1, &largv[0], rval);
				if (!ok && JS_IsExceptionPending(cx)) {
					JS_ReportPendingException(cx);
				}
			};
			arg0 = lambda;
		} while(0)
		;
		JSB_PRECONDITION2(ok, cx, false, "js_soomla_JSBinding_listen : Error processing arguments");
		Soomla::JSBinding::listen(arg0);
		JS_SET_RVAL(cx, vp, JSVAL_VOID);
		return true;
	}
	JS_ReportError(cx, "js_soomla_JSBinding_listen : wrong number of arguments");
	return false;
}



void js_Soomla_JSBinding_finalize(JSFreeOp *fop, JSObject *obj) {
    CCLOGINFO("jsbindings: finalizing JS object %p (JSBinding)", obj);
}

void js_register_soomla_JSBinding(JSContext *cx, JSObject *global) {
	jsb_Soomla_JSBinding_class = (JSClass *)calloc(1, sizeof(JSClass));
	jsb_Soomla_JSBinding_class->name = "CCSoomlaNdkBridge";
	jsb_Soomla_JSBinding_class->addProperty = JS_PropertyStub;
	jsb_Soomla_JSBinding_class->delProperty = JS_DeletePropertyStub;
	jsb_Soomla_JSBinding_class->getProperty = JS_PropertyStub;
	jsb_Soomla_JSBinding_class->setProperty = JS_StrictPropertyStub;
	jsb_Soomla_JSBinding_class->enumerate = JS_EnumerateStub;
	jsb_Soomla_JSBinding_class->resolve = JS_ResolveStub;
	jsb_Soomla_JSBinding_class->convert = JS_ConvertStub;
	jsb_Soomla_JSBinding_class->finalize = js_Soomla_JSBinding_finalize;
	jsb_Soomla_JSBinding_class->flags = JSCLASS_HAS_RESERVED_SLOTS(2);

	static JSPropertySpec properties[] = {
		{"__nativeObj", 0, JSPROP_ENUMERATE | JSPROP_PERMANENT, JSOP_WRAPPER(js_is_native_obj), JSOP_NULLWRAPPER},
		{0, 0, 0, JSOP_NULLWRAPPER, JSOP_NULLWRAPPER}
	};

	static JSFunctionSpec funcs[] = {
        JS_FS_END
	};

	static JSFunctionSpec st_funcs[] = {
		JS_FN("callNative", js_soomla_JSBinding_callNative, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
		JS_FN("listen", js_soomla_JSBinding_listen, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
		JS_FS_END
	};

	jsb_Soomla_JSBinding_prototype = JS_InitClass(
		cx, global,
		NULL, // parent proto
		jsb_Soomla_JSBinding_class,
		dummy_constructor<Soomla::JSBinding>, 0, // no constructor
		properties,
		funcs,
		NULL, // no static properties
		st_funcs);
	// make the class enumerable in the registered namespace
//	bool found;
//FIXME: Removed in Firefox v27	
//	JS_SetPropertyAttributes(cx, global, "CCSoomlaNdkBridge", JSPROP_ENUMERATE | JSPROP_READONLY, &found);

	// add the proto and JSClass to the type->js info hash table
	TypeTest<Soomla::JSBinding> t;
	js_type_class_t *p;
	std::string typeName = t.s_name();
	if (_js_global_type_map.find(typeName) == _js_global_type_map.end())
	{
		p = (js_type_class_t *)malloc(sizeof(js_type_class_t));
		p->jsclass = jsb_Soomla_JSBinding_class;
		p->proto = jsb_Soomla_JSBinding_prototype;
		p->parentProto = NULL;
		_js_global_type_map.insert(std::make_pair(typeName, p));
	}
}

void register_all_soomla(JSContext* cx, JSObject* obj) {
	// first, try to get the ns
	JS::RootedValue nsval(cx);
	JS::RootedObject ns(cx);
	JS_GetProperty(cx, obj, "Soomla", &nsval);
	if (nsval == JSVAL_VOID) {
		ns = JS_NewObject(cx, NULL, NULL, NULL);
		nsval = OBJECT_TO_JSVAL(ns);
		JS_SetProperty(cx, obj, "Soomla", nsval);
	} else {
		JS_ValueToObject(cx, nsval, &ns);
	}
	obj = ns;

	js_register_soomla_JSBinding(cx, obj);
}


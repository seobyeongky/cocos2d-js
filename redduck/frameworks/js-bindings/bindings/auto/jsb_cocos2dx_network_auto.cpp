#include "jsb_cocos2dx_network_auto.hpp"
#include "cocos2d_specifics.hpp"
#include "WebFile.h"

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
JSClass  *jsb_network_WebFile_class;
JSObject *jsb_network_WebFile_prototype;

bool js_cocos2dx_network_WebFile_clearStorage(JSContext *cx, uint32_t argc, jsval *vp)
{
	jsval *argv = JS_ARGV(cx, vp);
	bool ok = true;
	if (argc == 1) {
		std::function<void (bool)> arg0;
		do {
			std::shared_ptr<JSFunctionWrapper> func(new JSFunctionWrapper(cx, JS_THIS_OBJECT(cx, vp), argv[0]));
			auto lambda = [=](bool larg0) -> void {
				jsval largv[1];
				largv[0] = BOOLEAN_TO_JSVAL(larg0);
				jsval rval;
				bool ok = func->invoke(1, &largv[0], rval);
				if (!ok && JS_IsExceptionPending(cx)) {
					JS_ReportPendingException(cx);
				}
			};
			arg0 = lambda;
		} while(0)
		;
		JSB_PRECONDITION2(ok, cx, false, "js_cocos2dx_network_WebFile_clearStorage : Error processing arguments");
		network::WebFile::clearStorage(arg0);
		JS_SET_RVAL(cx, vp, JSVAL_VOID);
		return true;
	}
	JS_ReportError(cx, "js_cocos2dx_network_WebFile_clearStorage : wrong number of arguments");
	return false;
}

bool js_cocos2dx_network_WebFile_get(JSContext *cx, uint32_t argc, jsval *vp)
{
	jsval *argv = JS_ARGV(cx, vp);
	bool ok = true;
	if (argc == 3) {
		const char* arg0;
		const char* arg1;
		std::function<void (bool)> arg2;
		std::string arg0_tmp; ok &= jsval_to_std_string(cx, argv[0], &arg0_tmp); arg0 = arg0_tmp.c_str();
		std::string arg1_tmp; ok &= jsval_to_std_string(cx, argv[1], &arg1_tmp); arg1 = arg1_tmp.c_str();
		do {
			std::shared_ptr<JSFunctionWrapper> func(new JSFunctionWrapper(cx, JS_THIS_OBJECT(cx, vp), argv[2]));
			auto lambda = [=](bool larg0) -> void {
				jsval largv[1];
				largv[0] = BOOLEAN_TO_JSVAL(larg0);
				jsval rval;
				bool ok = func->invoke(1, &largv[0], rval);
				if (!ok && JS_IsExceptionPending(cx)) {
					JS_ReportPendingException(cx);
				}
			};
			arg2 = lambda;
		} while(0)
		;
		JSB_PRECONDITION2(ok, cx, false, "js_cocos2dx_network_WebFile_get : Error processing arguments");
		network::WebFile::get(arg0, arg1, arg2);
		JS_SET_RVAL(cx, vp, JSVAL_VOID);
		return true;
	}
	JS_ReportError(cx, "js_cocos2dx_network_WebFile_get : wrong number of arguments");
	return false;
}



void js_network_WebFile_finalize(JSFreeOp *fop, JSObject *obj) {
    CCLOGINFO("jsbindings: finalizing JS object %p (WebFile)", obj);
}

void js_register_cocos2dx_network_WebFile(JSContext *cx, JSObject *global) {
	jsb_network_WebFile_class = (JSClass *)calloc(1, sizeof(JSClass));
	jsb_network_WebFile_class->name = "WebFile";
	jsb_network_WebFile_class->addProperty = JS_PropertyStub;
	jsb_network_WebFile_class->delProperty = JS_DeletePropertyStub;
	jsb_network_WebFile_class->getProperty = JS_PropertyStub;
	jsb_network_WebFile_class->setProperty = JS_StrictPropertyStub;
	jsb_network_WebFile_class->enumerate = JS_EnumerateStub;
	jsb_network_WebFile_class->resolve = JS_ResolveStub;
	jsb_network_WebFile_class->convert = JS_ConvertStub;
	jsb_network_WebFile_class->finalize = js_network_WebFile_finalize;
	jsb_network_WebFile_class->flags = JSCLASS_HAS_RESERVED_SLOTS(2);

	static JSPropertySpec properties[] = {
		{"__nativeObj", 0, JSPROP_ENUMERATE | JSPROP_PERMANENT, JSOP_WRAPPER(js_is_native_obj), JSOP_NULLWRAPPER},
		{0, 0, 0, JSOP_NULLWRAPPER, JSOP_NULLWRAPPER}
	};

	static JSFunctionSpec funcs[] = {
        JS_FS_END
	};

	static JSFunctionSpec st_funcs[] = {
		JS_FN("clearStorage", js_cocos2dx_network_WebFile_clearStorage, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
		JS_FN("get", js_cocos2dx_network_WebFile_get, 3, JSPROP_PERMANENT | JSPROP_ENUMERATE),
		JS_FS_END
	};

	jsb_network_WebFile_prototype = JS_InitClass(
		cx, global,
		NULL, // parent proto
		jsb_network_WebFile_class,
		dummy_constructor<network::WebFile>, 0, // no constructor
		properties,
		funcs,
		NULL, // no static properties
		st_funcs);
	// make the class enumerable in the registered namespace
//	bool found;
//FIXME: Removed in Firefox v27	
//	JS_SetPropertyAttributes(cx, global, "WebFile", JSPROP_ENUMERATE | JSPROP_READONLY, &found);

	// add the proto and JSClass to the type->js info hash table
	TypeTest<network::WebFile> t;
	js_type_class_t *p;
	std::string typeName = t.s_name();
	if (_js_global_type_map.find(typeName) == _js_global_type_map.end())
	{
		p = (js_type_class_t *)malloc(sizeof(js_type_class_t));
		p->jsclass = jsb_network_WebFile_class;
		p->proto = jsb_network_WebFile_prototype;
		p->parentProto = NULL;
		_js_global_type_map.insert(std::make_pair(typeName, p));
	}
}

void register_all_cocos2dx_network(JSContext* cx, JSObject* obj) {
	// first, try to get the ns
	JS::RootedValue nsval(cx);
	JS::RootedObject ns(cx);
	JS_GetProperty(cx, obj, "cc", &nsval);
	if (nsval == JSVAL_VOID) {
		ns = JS_NewObject(cx, NULL, NULL, NULL);
		nsval = OBJECT_TO_JSVAL(ns);
		JS_SetProperty(cx, obj, "cc", nsval);
	} else {
		JS_ValueToObject(cx, nsval, &ns);
	}
	obj = ns;

	js_register_cocos2dx_network_WebFile(cx, obj);
}


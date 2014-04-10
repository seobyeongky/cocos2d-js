#ifndef __soomla_h__
#define __soomla_h__

#include "jsapi.h"
#include "jsfriendapi.h"


extern JSClass  *jsb_Soomla_JSBinding_class;
extern JSObject *jsb_Soomla_JSBinding_prototype;

bool js_soomla_JSBinding_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_soomla_JSBinding_finalize(JSContext *cx, JSObject *obj);
void js_register_soomla_JSBinding(JSContext *cx, JSObject *global);
void register_all_soomla(JSContext* cx, JSObject* obj);
bool js_soomla_JSBinding_callNative(JSContext *cx, uint32_t argc, jsval *vp);
bool js_soomla_JSBinding_listen(JSContext *cx, uint32_t argc, jsval *vp);
#endif


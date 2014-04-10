#ifndef __cocos2dx_network_h__
#define __cocos2dx_network_h__

#include "jsapi.h"
#include "jsfriendapi.h"


extern JSClass  *jsb_network_WebFile_class;
extern JSObject *jsb_network_WebFile_prototype;

bool js_cocos2dx_network_WebFile_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_network_WebFile_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_network_WebFile(JSContext *cx, JSObject *global);
void register_all_cocos2dx_network(JSContext* cx, JSObject* obj);
bool js_cocos2dx_network_WebFile_clearStorage(JSContext *cx, uint32_t argc, jsval *vp);
bool js_cocos2dx_network_WebFile_get(JSContext *cx, uint32_t argc, jsval *vp);
#endif


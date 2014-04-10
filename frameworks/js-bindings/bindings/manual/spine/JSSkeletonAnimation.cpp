#include "../js_bindings_opengl.h"
#include "../cocos2d_specifics.hpp"
#include "spine-cocos2dx.h"
#include "JSSkeletonAnimation.h"

// #include <spine/spine-cocos2dx.h>

namespace spine {
    JSSkeletonAnimation::JSSkeletonAnimation (const char* skeletonDataFile, spAtlas* atlas, float scale)
    : SkeletonAnimation(skeletonDataFile, atlas, scale) {}

    JSSkeletonAnimation::JSSkeletonAnimation (const char* skeletonDataFile, const char* atlasFile, float scale)
    : SkeletonAnimation(skeletonDataFile, atlasFile, scale) {}

    JSSkeletonAnimation* JSSkeletonAnimation::createWithFile (const char* skeletonDataFile, spAtlas* atlas, float scale) {
        JSSkeletonAnimation* node = new JSSkeletonAnimation(skeletonDataFile, atlas, scale);
        node->autorelease();
        return node;
    }

    JSSkeletonAnimation* JSSkeletonAnimation::createWithFile (const char* skeletonDataFile, const char* atlasFile, float scale) {
        JSSkeletonAnimation* node = new JSSkeletonAnimation(skeletonDataFile, atlasFile, scale);
        node->autorelease();
        return node;
    }    

    void JSSkeletonAnimation::onAnimationStateEvent (int trackIndex, spEventType type, spEvent* event, int loopCount) {
        js_proxy_t* proxy = NULL;
        JSContext *cx = ScriptingCore::getInstance()->getGlobalContext();
        proxy = js_get_or_create_proxy<cocos2d::Node>(cx, this);

        if( proxy ) {
            JSObject *jsObj = proxy->obj;
            if (jsObj) {
                bool found;
                JSB_AUTOCOMPARTMENT_WITH_GLOBAL_OBJCET
                JS_HasProperty(cx, jsObj, "onAnimationStateEvent", &found);
                if (found == true) {
                    JS::RootedValue rval(cx);
                    JS::RootedValue fval(cx);
                    jsval valArr[8];
                    int argc = 3;
                    valArr[0] = INT_TO_JSVAL(trackIndex);
                    valArr[1] = INT_TO_JSVAL(type);                    
                    valArr[2] = INT_TO_JSVAL(loopCount);
                    if (event) {
                        argc += 4;
                        valArr[3] = c_string_to_jsval(cx,event->data->name);
                        valArr[4] = INT_TO_JSVAL(event->intValue);
                        valArr[5] = DOUBLE_TO_JSVAL(event->data->floatValue);
                        valArr[6] = c_string_to_jsval(cx,event->stringValue);
                    }

                    JS_GetProperty(cx, jsObj, "onAnimationStateEvent", &fval);
                    JS_CallFunctionValue(cx, jsObj, fval, argc, valArr, rval.address());
                }
            }
        }
    }
}
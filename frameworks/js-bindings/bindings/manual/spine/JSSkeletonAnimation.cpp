#include "../js_bindings_opengl.h"
#include "../cocos2d_specifics.hpp"
#include "spine-cocos2dx.h"
#include "JSSkeletonAnimation.h"

// #include <spine/spine-cocos2dx.h>

namespace spine {
    
    // T for contents pointer type, V for tag type
    template <typename T, typename V>
    class Cache
    {
    public:
        struct chunk_t
        {
            std::string path;
            T *         ptr;
            V           tag;
            chunk_t() : path(), ptr(nullptr), tag() {}
            chunk_t(std::string path_, T * ptr_, V tag_) :
                path(path_), ptr(ptr_), tag(tag_) {}
        };
        
        void add (const char * file, T * ptr, V tag)
        {
            list.push_back(chunk_t(file,ptr,tag));
        }
        
        T * get(const char * file, V tag)
        {
            for (auto it : list)
            {
                if (strcmp(it.path.c_str(), file) == 0 && tag == it.tag)
                    return it.ptr;
            }
            return nullptr;
        }
        
        void clear()
        {
            for (auto it : list)
            {
                delete it.ptr;
            }
            list.clear();
        }
        
    private:
        std::vector<chunk_t> list;
    };
    
    Cache<spSkeletonData, float> skeletonDataCache;
    Cache<spAtlas, int> atlasCache;
    
    void JSSkeletonAnimation::clearCache()
    {
        skeletonDataCache.clear();
        atlasCache.clear();
    }
    
    spAtlas * getAtlas(const char * path)
    {
        auto atlas = atlasCache.get(path, 0);
        if (!atlas)
        {
            atlas = spAtlas_readAtlasFile(path);
            CCAssert(atlas, "Error reading atlas file.");
            atlasCache.add(path, atlas, 0);
        }
        return atlas;
    }

    spSkeletonData * getSkeletonData(const char * path, spAtlas * atlas, float wonderScale)
    {
        auto skeletonData = skeletonDataCache.get(path, wonderScale);
        if (!skeletonData)
        {
            spSkeletonJson* json = spSkeletonJson_create(atlas);
            json->scale = wonderScale;
            skeletonData = spSkeletonJson_readSkeletonDataFile(json, path);
            CCAssert(skeletonData, json->error ? json->error : "Error reading skeleton data file.");
            spSkeletonJson_dispose(json);
            skeletonDataCache.add(path, skeletonData, wonderScale);
        }
        return skeletonData;
    }
    
    JSSkeletonAnimation::JSSkeletonAnimation (spSkeletonData * skeletonData, bool isOwnsSkeletonData)
    : SkeletonAnimation(skeletonData, isOwnsSkeletonData) {}
    
    JSSkeletonAnimation::JSSkeletonAnimation (const char* skeletonDataFile, spAtlas* atlas, float scale)
    : SkeletonAnimation(skeletonDataFile, atlas, scale) {}

    JSSkeletonAnimation::JSSkeletonAnimation (const char* skeletonDataFile, const char* atlasFile, float scale)
    : SkeletonAnimation(skeletonDataFile, atlasFile, scale) {}

    JSSkeletonAnimation* JSSkeletonAnimation::createWithFile (const char* skeletonDataFile, spAtlas* atlas, float scale) {
        float wonderScale = scale == 0 ? (1 / Director::getInstance()->getContentScaleFactor()) : scale;
        auto skeletonData = getSkeletonData(skeletonDataFile, atlas, wonderScale);

        JSSkeletonAnimation* node = new JSSkeletonAnimation(skeletonData, false);
        node->autorelease();
        return node;
    }

    JSSkeletonAnimation* JSSkeletonAnimation::createWithFile (const char* skeletonDataFile, const char* atlasFile, float scale) {
    
        auto atlas = getAtlas(atlasFile);
        
        float wonderScale = scale == 0 ? (1 / Director::getInstance()->getContentScaleFactor()) : scale;
        auto skeletonData = getSkeletonData(skeletonDataFile, atlas, wonderScale);

        JSSkeletonAnimation* node = new JSSkeletonAnimation(skeletonData, false);
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
    
    void JSSkeletonAnimation::bounds_update() {
        spSkeletonBounds_update(_bounds,skeleton,1);
    }
    
    void JSSkeletonAnimation::supportBounds() {
        _bounds = spSkeletonBounds_create();
    }
    
    const std::string& JSSkeletonAnimation::bounds_containsPoint( const Vec2 & p ) {
        return bounds_containsPoint(p.x,p.y);
    }
    const std::string& JSSkeletonAnimation::bounds_containsPoint( float x, float y ) {
        
        Vec2 pos = getPosition();
        
        x = (x - pos.x)/getScaleX();
        y = (y - pos.y)/getScaleY();
        
        if (spSkeletonBounds_aabbContainsPoint( _bounds, x, y )) {
            spBoundingBoxAttachment* attachment = spSkeletonBounds_containsPoint( _bounds, x, y );
            if (attachment) {
                _buf = attachment->super.name;
                return _buf;
            }
        }
        spBoundingBoxAttachment* attachment = spSkeletonBounds_containsPoint( _bounds, x, y );
        if (attachment) {
            _buf = attachment->super.name;
            return _buf;
        }
        _buf = "";
        return _buf;
    }
}
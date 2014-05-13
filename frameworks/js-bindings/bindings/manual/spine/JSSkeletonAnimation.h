#ifndef JSSKELETON_ANIMATION_H_
#define JSSKELETON_ANIMATION_H_

#include <spine/CCSkeletonAnimation.h>

namespace spine {

class JSSkeletonAnimation: public SkeletonAnimation {
public:
    static void clearCache();
    
	static JSSkeletonAnimation* createWithFile (const char* skeletonDataFile, spAtlas* atlas, float scale = 0);
	static JSSkeletonAnimation* createWithFile (const char* skeletonDataFile, const char* atlasFile, float scale = 0);
    
	virtual void onAnimationStateEvent (int trackIndex, spEventType type, spEvent* event, int loopCount);

    JSSkeletonAnimation (spSkeletonData * skeletonData, bool isOwnsSkeletonData = false);
	JSSkeletonAnimation (const char* skeletonDataFile, spAtlas* atlas, float scale = 0);
	JSSkeletonAnimation (const char* skeletonDataFile, const char* atlasFile, float scale = 0);
    
    void supportBounds();
    void bounds_update();
    
    const std::string& bounds_containsPoint( Vector2 p );
    const std::string& bounds_containsPoint( float x, float y );
    
protected:
	JSSkeletonAnimation () {}
    std::string _buf;
    
    
    spSkeletonBounds* _bounds;
};

}

#endif
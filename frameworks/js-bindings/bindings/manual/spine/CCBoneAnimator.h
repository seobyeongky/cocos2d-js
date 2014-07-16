//
//  CCBoneAnimator.h
//  cocos2d_libs
//
//  Created by gameegg on 2014. 7. 14..
//
//

#ifndef __cocos2d_libs__CCBoneAnimator__
#define __cocos2d_libs__CCBoneAnimator__

#include <iostream>
#include <cocos2d.h>
#include "JSSkeletonAnimation.h"
#include <spine/CCSkeletonAnimation.h>

using namespace cocos2d;
using namespace spine;

class BoneAnimator : public JSSkeletonAnimation
{
public:
    BoneAnimator(std::string path);
    static BoneAnimator* createWithAttrs(std::string path, std::string anim_name, std::string bone_name, bool loop = false, float scale_x = 1, float scale_y = 1);
    void setBoneName(std::string bone_name);
    
private:
    void _update(float dt);
    
    std::string _bone_name;
    Point _prev_p;
    float _prev_scale_x;
    float _prev_scale_y;
    float _prev_rotation;
    
    float _anim_scale_x;
    float _anim_scale_y;
    
//    void onAnimationStateEvent(int trackIndex, spEventType type, spEvent *event, int loopCount);
};

#endif /* defined(__cocos2d_libs__CCBoneAnimator__) */
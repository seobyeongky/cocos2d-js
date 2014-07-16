//
//  CCBoneAnimator.cpp
//  cocos2d_libs
//
//  Created by gameegg on 2014. 7. 14..
//
//

#include "CCBoneAnimator.h"

BoneAnimator::BoneAnimator(std::string path) : JSSkeletonAnimation(std::string(path+".json").c_str(),std::string(path+".atlas").c_str())
{
    _prev_p = Vec2();
    _prev_rotation = 0;
    _prev_scale_x = 1;
    _prev_scale_y = 1;
}
BoneAnimator* BoneAnimator::createWithAttrs(std::string path, std::string anim_name, std::string bone_name, bool loop, float anim_scale_x, float anim_scale_y)
{
    auto node = new BoneAnimator(path);
    node->setAnimation(0, anim_name.c_str(), loop);
    
    node->setBoneName(bone_name);
    node->_anim_scale_x = anim_scale_x;
    node->_anim_scale_y = anim_scale_y;
    node->schedule(schedule_selector(BoneAnimator::_update));
    return node;
}

void BoneAnimator::setBoneName(std::string bone_name)
{
    _bone_name = bone_name;
}

void BoneAnimator::_update(float dt)
{
    log("bone_animator_update");
    if(_parent && _bone_name.length() != 0){
        log("bone_animator_real_update");
        spBone* bone = findBone(_bone_name.c_str());
        
        Vec2 p = Vec2(bone->worldX, bone->worldY);
        float r = bone->worldRotation;
        float sx = bone->worldScaleX;
        float sy = bone->worldScaleY;
        
        Vec2 dp =(p-_prev_p);
        dp.x *= _anim_scale_x;
        dp.y *= _anim_scale_y;
        _parent->setPosition(_parent->getPosition()+ dp);
        _parent->setRotation(_parent->getRotation()+r - _prev_rotation);
        _parent->setScaleX(_parent->getScaleX()*(sx / _prev_scale_x)*_anim_scale_x);
        _parent->setScaleY(_parent->getScaleY()*(sy / _prev_scale_y)*_anim_scale_y);
        
        
        _prev_p = p;
        _prev_rotation = r;
        _prev_scale_x = sx;
        _prev_scale_y = sy;
        
    }
}

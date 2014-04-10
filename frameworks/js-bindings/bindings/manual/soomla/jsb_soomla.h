//
// Created by Fedor Shubin on 1/22/14.
//


#ifndef __JSBinding_H_
#define __JSBinding_H_

#ifdef COCOS2D_JAVASCRIPT

#include "cocos2d.h"

// Define a namespace to manage your code and make your code clearly
namespace Soomla {
    class JSBinding: public cocos2d::CCObject
    {
    public:
        static std::string callNative(const char *params);
        // fixme listen해제하는 것도 필요...
        static void listen(std::function<void(std::string)> callback);
    };
}

#endif //COCOS2D_JAVASCRIPT

#endif //__JSBinding_H_

//
// Created by Fedor Shubin on 1/22/14.
//

#ifdef COCOS2D_JAVASCRIPT

#include "jsb_soomla.h"
#include "CCSoomlaError.h"
#include "jansson.h"
#include "CCSoomlaJsonHelper.h"
#include "CCSoomlaNdkBridge.h"
#include "ScriptingCore.h"
#include "CCDirector.h"

using namespace cocos2d;

std::string Soomla::JSBinding::callNative(const char *params) {
    std::string result;
    result.assign(params);

    CCLog("callNative: in >> %s", params);

    json_error_t error;
    json_t *root;
    root = json_loads(params, 0, &error);

    if (!root) {
        CCLog("error: at line #%d: %s", error.line, error.text);
        return result;
    }

    cocos2d::CCObject *dataToPass = CCSoomlaJsonHelper::getCCObjectFromJson(root);
    CCDictionary *dictToPass = dynamic_cast<CCDictionary *>(dataToPass);
    CC_ASSERT(dictToPass);

    soomla::CCSoomlaError *soomlaError = NULL;
    CCDictionary *retParams = (CCDictionary *) soomla::CCSoomlaNdkBridge::callNative(dictToPass, &soomlaError);

    CCDictionary *resultParams = CCDictionary::create();
    if (soomlaError != NULL) {
        retParams = CCDictionary::create();
        retParams->setObject(CCInteger::create(soomlaError->getCode()), "code");
        retParams->setObject(CCString::create(soomlaError->getInfo()), "info");

        resultParams->setObject(CCBool::create(false), "success");
    } else {
        resultParams->setObject(CCBool::create(true), "success");
    }
    resultParams->setObject(retParams, "result");

    root = CCSoomlaJsonHelper::getJsonFromCCObject(resultParams);
    char *dump = json_dumps(root, JSON_COMPACT | JSON_ENSURE_ASCII);
    CCLog("callNative: out >> %s", dump);
    result = dump;
    free(dump);
    
    return result;
}

void Soomla::JSBinding::listen(std::function<void(std::string)> callback)
{
    auto dispatcher = Director::getInstance()->getEventDispatcher();

    dispatcher->addCustomEventListener("soomla", [callback](EventCustom * ev){
        auto params = (CCDictionary *)(ev->getUserData());
        json_t *root = CCSoomlaJsonHelper::getJsonFromCCObject(params);
        char *dump = json_dumps(root, JSON_COMPACT | JSON_ENSURE_ASCII);
        callback(dump);
    });
}

/*
void Soomla::JSBinding::callCallback(CCDictionary *params) {
    json_t *root = CCSoomlaJsonHelper::getJsonFromCCObject(params);
    char *dump = json_dumps(root, JSON_COMPACT | JSON_ENSURE_ASCII);
    CCLog("callCallback: in >> %s", dump);

    std::string jsonParams = dump;
    free(dump);

    JSContext* cx = ScriptingCore::getInstance()->getGlobalContext();

    jsval retval;
    jsval v[] = {
            v[0] = STRING_TO_JSVAL(JS_NewStringCopyZ(cx, jsonParams.c_str()))
    };
    ScriptingCore::getInstance()->executeFunctionWithOwner(OBJECT_TO_JSVAL(ScriptingCore::getInstance()->getGlobalObject()),
            "easyNDKCallBack", 1, v, &retval);
}
*/

#endif // COCOS2D_JAVASCRIPT
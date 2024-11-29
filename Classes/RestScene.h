#pragma once

#include "cocos2d.h"

/*
 * Class Name:     RestScene
 * Class Function: ≥ı º≥°æ∞¿‡
 */
class RestScene : public cocos2d::Scene {
public:
    static cocos2d::Scene* createScene();

    void checkAndShowReturnButton(bool isRestClicked, bool isSmithClicked, cocos2d::Node* returnButton);

    virtual bool init();

    CREATE_FUNC(RestScene);

};
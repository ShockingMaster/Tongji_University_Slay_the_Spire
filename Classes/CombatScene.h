#pragma once
#include "CombatDeck.h"
#include "cocos2d.h"
#include "CombatNode.h"

class CombatScene : public cocos2d::Scene {
public:

    CombatNode combat_node_;

    CombatDeck combat_deck_;

    // 创建场景的静态函数
    static cocos2d::Scene* createScene();

    // 初始化函数
    virtual bool init();

    // CREATE_FUNC宏自动调用默认的 init() 方法
    CREATE_FUNC(CombatScene);
};

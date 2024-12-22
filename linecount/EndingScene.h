#pragma once

#include "cocos2d.h"

class EndingScene : public cocos2d::Scene {
public:
    // 胜利标志，true 表示胜利，false 表示失败
    bool victory;

    // 创建场景的静态函数
    static EndingScene* create(bool isVictory);

    // 初始化函数
    virtual bool init(bool isVictory);

    // 析构函数
    virtual ~EndingScene() {}
};

#pragma once

#include "cocos2d.h"

class ChestScene : public cocos2d::Scene {
public:

    // 创建场景的静态函数
    static cocos2d::Scene* createScene();

    // 初始化函数
    virtual bool init();

    // CREATE_FUNC宏自动调用默认的 init() 方法
    CREATE_FUNC(ChestScene);
};



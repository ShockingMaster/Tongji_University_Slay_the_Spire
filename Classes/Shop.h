#pragma once

#ifndef __SHOP_SCENE_H__
#define __SHOP_SCENE_H__

#include "cocos2d.h"

class Shop : public cocos2d::Scene {
public:
 
    // 创建场景的静态函数
    static cocos2d::Scene* createScene();

    // 初始化函数
    virtual bool init();

    // CREATE_FUNC宏自动调用默认的 init() 方法
    CREATE_FUNC(Shop);
};

#endif // __SHOP_SCENE_H__


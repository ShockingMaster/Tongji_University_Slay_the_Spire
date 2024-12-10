#pragma once
#ifndef _SETTINGS_SCENE_H_
#define _SETTINGS_SCENE_H_

#include "cocos2d.h"

 /*
  * Class Name:     SettingsScene
  * Class Function: 设置场景类
  */
class SettingsScene : public cocos2d::Scene {
public:
    // 创建场景
    static cocos2d::Scene* createScene();

    // 初始化场景
    virtual bool init();

    // 实现 SettingsScene 类的 create 方法
    CREATE_FUNC(SettingsScene);
};

#endif // !_SETTINGS_SCENE_H_
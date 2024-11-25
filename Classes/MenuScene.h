#pragma once                      // 确保头文件只被包含一次
#ifndef _MENU_SCENE_H_             
#define _MENU_SCENE_H_

#include "cocos2d.h"              

/**
 * MenuScene类：游戏主菜单场景类，继承自cocos2d::Scene。
 * 用于创建并管理游戏的主菜单界面。
 */
class MenuScene : public cocos2d::Scene {
public:
    /**
     * 创建场景函数：静态函数，返回一个新的MenuScene场景实例。
     * @return 创建并返回一个新的场景实例
     */
    static cocos2d::Scene* createScene();

    /**
     * 初始化函数：初始化菜单场景，设置背景、按钮等元素。
     * @return 如果初始化成功则返回true，失败则返回false
     */
    virtual bool init();

    // 使用CREATE_FUNC宏自动生成创建函数（create()）
    CREATE_FUNC(MenuScene);
};

#endif  // _MENU_SCENE_H_

#include "Shop.h"
#include "AudioPlayer.h"
#include "NodeConnection.h"
#include "Player.h"
#include "HeaderBar.h"

// 创建场景
cocos2d::Scene* Shop::createScene() {
    auto scene = Scene::create();          // 创建一个空白场景
    auto layer = Shop::create();     // 创建 StartScene 层
    scene->addChild(layer);                // 将层添加到场景
    return scene;
}


// 初始化方法
bool Shop::init() {
    if (!Scene::init()) {
        return false;
    }

 

    return true;
}

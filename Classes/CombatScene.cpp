#include "CombatScene.h"
#include "AudioPlayer.h"
#include "NodeConnection.h"
#include "Player.h"
#include "HeaderBar.h"
#include "HoverButton.h"
#include "RewardLayer.h"


// 创建场景
cocos2d::Scene* CombatScene::createScene() {
    auto scene = Scene::create();          // 创建一个空白场景
    auto layer = CombatScene::create();
    scene->addChild(layer);                // 将层添加到场景
    return scene;
}


bool CombatScene::init() {
    if (!Scene::init()) {
        return false;
    }
    Player* player = Player::getInstance();
    auto headbar = HeaderBar::create(player);
    headbar->setPosition(Vec2(0, 1150));          // 设置位置（在屏幕上部）
    this->addChild(headbar);
    headbar->setLocalZOrder(100);
    // 背景图像
    const auto background = Sprite::create("scene3.png");
    background->setPosition(Vec2(1100, 500));
    background->setScale(1.05f);
    this->addChild(background);
    return true;
}


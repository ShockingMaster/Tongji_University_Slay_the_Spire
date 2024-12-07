#include "CombatScene.h"
#include "AudioPlayer.h"
#include "NodeConnection.h"
#include "Player.h"
#include "HeaderBar.h"
#include "HoverButton.h"
#include "RewardLayer.h"


// ��������
cocos2d::Scene* CombatScene::createScene() {
    auto scene = Scene::create();          // ����һ���հ׳���
    auto layer = CombatScene::create();
    scene->addChild(layer);                // ������ӵ�����
    return scene;
}


bool CombatScene::init() {
    if (!Scene::init()) {
        return false;
    }
    Player* player = Player::getInstance();
    auto headbar = HeaderBar::create(player);
    headbar->setPosition(Vec2(0, 1150));          // ����λ�ã�����Ļ�ϲ���
    this->addChild(headbar);
    headbar->setLocalZOrder(100);
    // ����ͼ��
    const auto background = Sprite::create("scene3.png");
    background->setPosition(Vec2(1100, 500));
    background->setScale(1.05f);
    this->addChild(background);
    return true;
}


#include "Shop.h"
#include "AudioPlayer.h"
#include "NodeConnection.h"
#include "Player.h"
#include "HeaderBar.h"

// ��������
cocos2d::Scene* Shop::createScene() {
    auto scene = Scene::create();          // ����һ���հ׳���
    auto layer = Shop::create();     // ���� StartScene ��
    scene->addChild(layer);                // ������ӵ�����
    return scene;
}


// ��ʼ������
bool Shop::init() {
    if (!Scene::init()) {
        return false;
    }

 

    return true;
}

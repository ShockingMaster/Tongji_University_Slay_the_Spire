#include "ShopScene.h"
#include "AudioPlayer.h"
#include "NodeConnection.h"
#include "Player.h"
#include "HeaderBar.h"

// ��������
cocos2d::Scene* ShopScene::createScene() {
    auto scene = Scene::create();          // ����һ���հ׳���
    auto layer = ShopScene::create();     // ���� StartScene ��
    scene->addChild(layer);                // ������ӵ�����
    return scene;
}


// ��ʼ������
bool ShopScene::init() {
    if (!Scene::init()) {
        return false;
    }
    
 

    return true;
}

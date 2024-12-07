#include "ShopScene.h"
#include "AudioPlayer.h"
#include "NodeConnection.h"
#include "Player.h"
#include "HeaderBar.h"

// ��������
cocos2d::Scene* ShopScene::createScene() {
    auto scene = Scene::create();          // ����һ���հ׳���
    auto layer = ShopScene::create();     
    scene->addChild(layer);                // ������ӵ�����
    return scene;
}


bool ShopScene::init() {
    if (!Scene::init()) {
        return false;
    }

    // ���ű�����Ч
    audioPlayer("../Resources/event.ogg", true);


    Player* player = Player::getInstance();
    auto headbar = HeaderBar::create(player);
    headbar->setPosition(Vec2(0, 1150));          // ����λ�ã�����Ļ�ϲ���
    this->addChild(headbar);
    headbar->setLocalZOrder(100);
    // ����ͼ��
    const auto background = Sprite::create("walkscene.jpg");
    background->setPosition(Vec2(1100, 300));
    this->addChild(background);

    // ��ӭ��ǩ
    auto continueLabel = Label::createWithSystemFont(u8"����ӭ�����̵꣡��\nһ������ߵ�����\n�����������ǰ", "Fonts/Kreon-Bold.ttf", 65);
    continueLabel->setPosition(Vec2(600, 800));
    continueLabel->setColor(Color3B::WHITE);
    this->addChild(continueLabel);

    // ���˱���ͼ��
    const auto background0 = Sprite::create("merchant-removebg-preview.png");
    background0->setScale(1.0f);
    background0->setOpacity(200);
    background0->setPosition(Vec2(1500, 600));
    this->addChild(background0);

    // ʹ�� DelayTime �������ӳ�
    auto delay = DelayTime::create(3.0f); 

    // ����һ���ص����������ӳ�ʱ���ִ��
    auto showLayerCallback = CallFunc::create([this]() {
        // �����²㣬�����ñ���ͼ
        auto newLayer = Sprite::create("fin.png");
        newLayer->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2,
            Director::getInstance()->getVisibleSize().height / 2 + 190));
        newLayer->setScale(0.9f);
        this->addChild(newLayer, 100); // 100�ǲ㼶��ȷ��������Ԫ��֮��

        // �������ذ�ť
        auto backButton = MenuItemImage::create(
            "cancelButton.png",    // ��ͨ״̬ͼƬ
            "cancelButtonOutline.png",   // ����״̬ͼƬ
            [this](Ref* sender) { // �����ťʱִ��
                // ���ص���һ������
                Director::getInstance()->popScene();  // ������ǰ����������ǰһ������
            }
        );

        // ���ð�ťλ��
        backButton->setPosition(Vec2(Director::getInstance()->getVisibleSize().width - 100, 100)); 

        // �����˵�����Ӱ�ť
        auto menu = Menu::create(backButton, nullptr);
        menu->setPosition(Vec2::ZERO);  // �˵���ê������Ϊ (0, 0)
        newLayer->addChild(menu);  // ����ť��ӵ� newLayer

        });


    // ����һ��˳����
    auto sequence = Sequence::create(delay, showLayerCallback, nullptr);

    // ִ�ж���
    this->runAction(sequence);

    return true;
}

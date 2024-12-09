#include "ChestScene.h"
#include "AudioPlayer.h"
#include "NodeConnection.h"
#include "Player.h"
#include "HeaderBar.h"
#include "HoverButton.h"
#include "RewardLayer.h"


// ��������
cocos2d::Scene* ChestScene::createScene() {
    auto scene = Scene::create();          // ����һ���հ׳���
    auto layer = ChestScene::create();
    scene->addChild(layer);                // ������ӵ�����
    return scene;
}


bool ChestScene::init() {
    if (!Scene::init()) {
        return false;
    }

    // ���ű�����Ч
    audioPlayer("chest.ogg", true);
    // ����ͼ��
    const auto background = Sprite::create("scene3.png");
    background->setPosition(Vec2(1100, 500));
    this->addChild(background);

    // �������䲢����λ��
    auto chest = Sprite::create("largeChest.png");
    chest->setPosition(Vec2(1100, 700));
    this->addChild(chest);

    // ��������¼�������
    auto mouseListener = EventListenerMouse::create();

    // ����������Ļص�����
    mouseListener->onMouseDown = [chest](Event* event) {
        // ��ȡ�������λ��
        EventMouse* mouseEvent = (EventMouse*)event;
        Vec2 clickPosition = mouseEvent->getLocationInView();

        // ��ȡ����ľ�������
        Rect chestRect = chest->getBoundingBox();

        // �ж���������Ƿ��ڱ����������
        if (chestRect.containsPoint(clickPosition)) {
            // ���䱻��������ſ��������Ч
            audioPlayer("SOTE_SFX_ChestOpen_v2", false);

            // ���������ͼ��Ϊ�Ѵ�״̬
            chest->setTexture("largeChestOpened.png");  // �滻Ϊ�Ѵ򿪵ı���ͼ��

            // ���� RewardLayer ����ʾ����
            RewardLayer* rewardLayer = RewardLayer::create();
            Director::getInstance()->getRunningScene()->addChild(rewardLayer);  // �� RewardLayer ��ӵ���ǰ����
        }
        };


    // ע������¼�������
    _eventDispatcher->addEventListenerWithSceneGraphPriority(mouseListener, this);

    // ���� HoverButton
    auto startButton = HoverButton::create(
        "button1 (1).png",  // Ĭ��ͼƬ
        "button1 (2).png",  // ��ť��ͣʱ��ͼƬ
        "button1 (3).png"   // ��ť���ʱ��ͼƬ
    );

    // ���ð�ťλ��
    startButton->setPosition(Vec2(1800, 500));
    this->addChild(startButton);

    // ��Ӱ�ť����¼�������
    startButton->addClickEventListener([=](Ref* sender) {
        // ִ�� popScene ������������һ������
        Director::getInstance()->popScene();
        });

    return true;
}


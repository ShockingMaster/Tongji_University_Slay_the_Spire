#include "RestScene.h"
#include "HoverButton.h"  // ���� HoverButton �࣬���ڰ�ť����ͣЧ��
#include "AudioPlayer.h"
#include "StartScene.h"
#include "HelloWorldScene.h"
#include "const.h"
#include "Player.h"
#include "HeaderBar.h"


extern Player player;
using namespace std;
using namespace cocos2d;

Scene* RestScene::createScene() {
    // ����һ���µĿճ���
    auto scene = Scene::create();
    // ���� RestScene �㲢������ӵ�������
    auto layer = RestScene::create();
    scene->addChild(layer);
    return scene;
}

bool RestScene::init() {
    if (!Scene::init()) {
        return false;  // �����ʼ��ʧ���򷵻� false
    }

    // ���ű�������
    audioPlayer("../Resources/rest.ogg", true);
    auto headbar = HeaderBar::create(&player);     
    headbar->setPosition(Vec2(0, 1150));          // ����λ�ã�����Ļ�ϲ���
    this->addChild(headbar);
    headbar->setLocalZOrder(100);  // �� headbar �� Z ˳������Ϊ 100��ȷ����λ�����ϲ�
    /* ����������������һ������ͼ��
    auto background = Sprite::create("../Resources/scene1.png");
    background->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2,
        Director::getInstance()->getVisibleSize().height / 2));
    this->addChild(background);*/

    // ������Ϣ��ť
    auto restButton = HoverButton::create("sleep.png", "sleep.png", "sleep.png");
    restButton->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 4,
        Director::getInstance()->getVisibleSize().height / 2));
    restButton->addClickEventListener([this,headbar](Ref* sender) {
        // �ָ��������ֵ�� 30%
        player.health_ += player.fullhealth_ * 0.3f;
        // ȷ��Ѫ�����������Ѫ��
        if (player.health_ > player.fullhealth_) {
            player.health_ = player.fullhealth_;  // ����ָ����Ѫ���������Ѫ������Ϊ���Ѫ��
        }
        // ����ͷ���е�Ѫ����ʾ
        headbar->updateHeader(&player);

        // �������ذ�ť
        auto returnButton = HoverButton::create("button4(1).png", "button4(2).png", "button4(1).png");
        returnButton->setPosition(Vec2(1000, 300));
        returnButton->addClickEventListener([this](Ref* sender) {
            // ����������ݲ����ص�ͼ
            Director::getInstance()->popScene();
            });
        returnButton->setVisible(false);
        this->addChild(returnButton);

        // ���� "����" ��ǩ
        auto continueLabel = Label::createWithSystemFont(u8"����", "Fonts/FangZhengZhaoGeYuan.ttf", 40);
        continueLabel->setPosition(Vec2(1000, 300));
        continueLabel->setColor(Color3B::WHITE);
        this->addChild(continueLabel);

        // ��ʾ���ذ�ť
        auto showReturnButton = CallFunc::create([returnButton]() {
            returnButton->setVisible(true);
            });
        this->runAction(Sequence::create(showReturnButton, nullptr));
        });


    this->addChild(restButton);

    // �������쿨�ư�ť
    auto smithButton = HoverButton::create("smith.png", "smith.png", "smith.png");
    smithButton->setPosition(Vec2(3 * Director::getInstance()->getVisibleSize().width / 4,
        Director::getInstance()->getVisibleSize().height / 2));
    smithButton->addClickEventListener([this](Ref* sender) {
        // ����һ�ſ���
       // �������ذ�ť
        auto returnButton = HoverButton::create("button4(1).png", "button4(2).png", "button4(1).png");
        returnButton->setPosition(Vec2(1000, 300));
        returnButton->addClickEventListener([this](Ref* sender) {
            // �������������ťʱִ�е��߼�
            Director::getInstance()->popScene(); 
            });
        returnButton->setVisible(false); // ��ʼʱ���ط��ذ�ť
        this->addChild(returnButton);

        // ���� "����" ��ǩ
        auto continueLabel = Label::createWithSystemFont(u8"����", "Fonts/FangZhengZhaoGeYuan.ttf", 40);
        continueLabel->setPosition(Vec2(1000, 300));
        continueLabel->setColor(Color3B::WHITE);
        this->addChild(continueLabel);


        // ��ʾ���ذ�ť
        auto showReturnButton = CallFunc::create([returnButton]() {
            returnButton->setVisible(true);
            });
        this->runAction(Sequence::create(showReturnButton, nullptr));
        });
    this->addChild(smithButton);
    auto InstructLabel = Label::createWithSystemFont(u8"��ӭ������Ϣ����", "Fonts/FangZhengZhaoGeYuan.ttf", 60);
    InstructLabel->setPosition(Vec2(restButton->getPosition().x+500, restButton->getPosition().y + 500));
    InstructLabel->setColor(Color3B::WHITE);
    this->addChild(InstructLabel);
    // ��Ӱ�ť�Ϸ���˵������
    auto restLabel = Label::createWithSystemFont(u8"��Ϣ �ظ��������ֵ��30%", "Fonts/FangZhengZhaoGeYuan.ttf", 50);
    restLabel->setPosition(Vec2(restButton->getPosition().x, restButton->getPosition().y + 200));
    restLabel->setColor(Color3B::WHITE);
    this->addChild(restLabel);

    auto smithLabel = Label::createWithSystemFont(u8"���� ����һ�ſ���", "Fonts/FangZhengZhaoGeYuan.ttf", 50);
    smithLabel->setPosition(Vec2(smithButton->getPosition().x, smithButton->getPosition().y + 200));
    smithLabel->setColor(Color3B::WHITE);
    this->addChild(smithLabel);

    return true; // ��ʼ���ɹ������� true
}

#include "RestScene.h"
#include "HoverButton.h"  // ���� HoverButton �࣬���ڰ�ť����ͣЧ��
#include "AudioPlayer.h"
#include "StartScene.h"
#include "HelloWorldScene.h"
#include "const.h"
#include "Player.h"
#include "HeaderBar.h"



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
    Player* player = Player::getInstance();
    // ���ű�������
    audioPlayer("../Resources/rest.ogg", true);
    auto headbar = HeaderBar::create(player);
    headbar->setPosition(Vec2(0, 1150));          // ����λ�ã�����Ļ�ϲ���
    this->addChild(headbar);
    headbar->setLocalZOrder(100);  // �� headbar �� Z ˳������Ϊ 100��ȷ����λ�����ϲ�
    auto background = Sprite::create("../Resources/room.jpg");
    background->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2,
        Director::getInstance()->getVisibleSize().height / 2+200));
    background->setScale(1.4f);
    background->setOpacity(100);
    this->addChild(background);

    // ������Ϣ��ť
    auto restButton = HoverButton::create("sleep.png", "sleep.png", "sleep.png");
    restButton->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 4,
        Director::getInstance()->getVisibleSize().height / 2));

    restButton->addClickEventListener([this, headbar](Ref* sender) {
        audioPlayer("addhealth.ogg", false); 
        Player* player = Player::getInstance();
        // �ָ��������ֵ�� 30%
        player->health_ += player->fullhealth_ * 0.3f;
        // ȷ��Ѫ�����������Ѫ��
        if (player->health_ > player->fullhealth_) {
            player->health_ = player->fullhealth_;  // ����ָ����Ѫ���������Ѫ������Ϊ���Ѫ��
        }
        // ����ͷ���е�Ѫ����ʾ
        headbar->updateHeader(player);

        // �������ذ�ť
        auto returnButton = HoverButton::create("button4(1).png", "button4(2).png", "button4(1).png");
        returnButton->setPosition(Vec2(1000, 300));
        returnButton->addClickEventListener([this](Ref* sender) {
            // ����������ݲ����ص�ͼ
            audioPlayer("clickSoundEffect.mp3", false);
            Director::getInstance()->popScene();
            });
        returnButton->setVisible(false);
        this->addChild(returnButton);

        // ���� "����" ��ǩ
        auto continueLabel = Label::createWithSystemFont(u8"����", "Fonts/Kreon-Bold.ttf", 40);
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
            audioPlayer("clickSoundEffect.mp3", false);
            Director::getInstance()->popScene();
            });
        returnButton->setVisible(false); // ��ʼʱ���ط��ذ�ť
        this->addChild(returnButton);

        // ���� "����" ��ǩ
        auto continueLabel = Label::createWithSystemFont(u8"����", "Fonts/Kreon-Bold.ttf", 40);
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
   
    // ��Ӱ�ť�Ϸ���˵������
    auto restLabel = Label::createWithSystemFont(u8"��Ϣ �ظ��������ֵ��30%", "Fonts/Kreon-Bold.ttf", 60);
    restLabel->setPosition(Vec2(restButton->getPosition().x, restButton->getPosition().y + 150));
    restLabel->setColor(Color3B::WHITE);
    this->addChild(restLabel);

    auto smithLabel = Label::createWithSystemFont(u8"���� ����һ�ſ���", "Fonts/Kreon-Bold.ttf", 60);
    smithLabel->setPosition(Vec2(smithButton->getPosition().x, smithButton->getPosition().y + 150));
    smithLabel->setColor(Color3B::WHITE);
    this->addChild(smithLabel);

    return true; // ��ʼ���ɹ������� true
}

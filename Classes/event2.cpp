/*���߽�һ����·����׼��Ҫ��ͷʱ������ͻȻ��ǽ�ڴ��컨�����ѵ�һ������������
������������ǽ���ϣ���ʼ����˵����
����������֪���ģ��Ҿ������ߡ���
�������ı䣬�Ҿ����㿴���µĵ�·����
���������Ҫ��������ͨ������ͱ��������ɳ�����*/
#include "event2.h"
#include "HoverButton.h"  
#include "AudioPlayer.h"
#include "const.h"
#include "EventSystem.h"
#include "Card.h"
#include "HeaderBar.h"
#include "CardLayer.h"
using namespace std;
using namespace cocos2d;

Scene* event2::createScene() {
    // ����һ���µĿճ���
    auto scene = Scene::create();
    // ���� event1 �㲢������ӵ�������
    auto layer = event2::create();
    scene->addChild(layer);
    return scene;
}

bool event2::init() {
    if (!Scene::init()) {
        return false;  // �����ʼ��ʧ���򷵻� false
    }
    const auto backgroun = Sprite::create("walkscene.jpg");
    backgroun->setPosition(Vec2(1100, 100));
    this->addChild(backgroun);
    // ������Ƶ��ʾͷ��

    audioPlayer("event.ogg", true);
    auto player = EventSystem::getInstance();
    auto headbar = HeaderBar::create(player);
    headbar->updateHeader(player);
    headbar->setPosition(Vec2(0, 750));          // ����λ�ã�����Ļ�ϲ���
    this->addChild(headbar);

    headbar->setLocalZOrder(100);  // �� headbar �� Z ˳������Ϊ 100��ȷ����λ�����ϲ�
    // ���ñ���ͼƬ
    auto background = Sprite::create("livingWall.jpg");
    background->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2 - 550,
        Director::getInstance()->getVisibleSize().height / 2 - 100));
    this->addChild(background);

    // �����¼��ı���ǩ
    auto continueLabel = Label::createWithSystemFont(u8"���߽�һ����·����׼��Ҫ��ͷʱ\n����ͻȻ��ǽ�ڴ��컨�����ѵ�һ������������\n������������ǽ���ϣ���ʼ����˵����\n����������֪���ģ��Ҿ������ߡ���\n�������ı䣬�Ҿ����㿴���µĵ�·����\n���������Ҫ��������ͨ������ͱ��������ɳ���", "Fonts/FangZhengZhaoGeYuan.ttf", 45);
    continueLabel->setPosition(Vec2(1300, 600));
    continueLabel->setColor(Color3B::WHITE);
    this->addChild(continueLabel);
     
    // ������һ�� HoverButton
    auto button1 = HoverButton::create(
        "button4(1).png", "button4(2).png", "button4(1).png"
    );
    button1->setPosition(Vec2(1600, 100));
    button1->addClickEventListener([=](Ref* sender) {
        // ������Ӱ�ť����¼��Ĵ������
        auto player= EventSystem::getInstance();
        auto cardLayer = CardLayer::create(player->cards_, 2);
        Director::getInstance()->getRunningScene()->addChild(cardLayer);
        auto returnButton = HoverButton::create("button4(1).png", "button4(2).png", "button4(1).png");
        returnButton->setPosition(Vec2(1000, 100));
        returnButton->addClickEventListener([this](Ref* sender) {
            // �������������ťʱִ�е��߼�
            audioPlayer("clickSoundEffect.mp3", false);
            Director::getInstance()->popScene();
            });
        this->addChild(returnButton);

        // ���� "����" ��ǩ
        auto continueLabel = Label::createWithSystemFont(u8"����", "Fonts/Kreon-Bold.ttf", 40);
        continueLabel->setPosition(Vec2(1000, 100));
        continueLabel->setColor(Color3B::WHITE);
        this->addChild(continueLabel);
        });
    this->addChild(button1);
    auto continueLabel1 = Label::createWithSystemFont(u8"      ����\n�Ƴ���������һ����", "Fonts/FangZhengZhaoGeYuan.ttf", 45);
    continueLabel1->setPosition(Vec2(1200, 100));
    continueLabel1->setColor(Color3B::WHITE);
    this->addChild(continueLabel1);
    // �����ڶ��� HoverButton���뿪
    auto button2 = HoverButton::create(
        "button4(1).png", "button4(2).png", "button4(1).png"
    );
    button2->setPosition(Vec2(1600, 300));
    button2->addClickEventListener([=](Ref* sender) {
        // ������Ӱ�ť����¼��Ĵ������
        // ���磬���ص���һ���� shared_ptr<Player> player= Player::getInstance();
        auto cardLayer = CardLayer::create(player->cards_, 3); 
        Director::getInstance()->getRunningScene()->addChild(cardLayer);
        auto returnButton = HoverButton::create("button4(1).png", "button4(2).png", "button4(1).png");
        returnButton->setPosition(Vec2(1000, 100));
        returnButton->addClickEventListener([this](Ref* sender) {
            // �������������ťʱִ�е��߼�
            audioPlayer("clickSoundEffect.mp3", false);
            Director::getInstance()->popScene();
            });
        this->addChild(returnButton);

        // ���� "����" ��ǩ
        auto continueLabel = Label::createWithSystemFont(u8"����", "Fonts/Kreon-Bold.ttf", 40);
        continueLabel->setPosition(Vec2(1000, 100));
        continueLabel->setColor(Color3B::WHITE);
        this->addChild(continueLabel);
        });
    this->addChild(button2);
    auto continueLabel2 = Label::createWithSystemFont(u8"      �ɳ�\n������������һ���� ", "Fonts/FangZhengZhaoGeYuan.ttf", 45);
    continueLabel2->setPosition(Vec2(1200, 300));
    continueLabel2->setColor(Color3B::WHITE);
    this->addChild(continueLabel2);

    return true; // ��ʼ���ɹ������� true
}


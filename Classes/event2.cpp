/*���߽�һ����·����׼��Ҫ��ͷʱ������ͻȻ��ǽ�ڴ��컨�����ѵ�һ������������
������������ǽ���ϣ���ʼ����˵����
����������֪���ģ��Ҿ������ߡ���
�������ı䣬�Ҿ����㿴���µĵ�·����
���������Ҫ��������ͨ������ͱ��������ɳ�����*/
#include "event2.h"
#include "HoverButton.h"  
#include "AudioPlayer.h"
#include "const.h"
#include "Player.h"
#include "HeaderBar.h"

using namespace std;
using namespace cocos2d;

Scene* event2::createScene() {
    // ����һ���µĿճ���
    auto scene = Scene::create();
    // ���� event1 �㲢�������ӵ�������
    auto layer = event2::create();
    scene->addChild(layer);
    return scene;
}

bool event2::init() {
    if (!Scene::init()) {
        return false;  // �����ʼ��ʧ���򷵻� false
    }

    // ������Ƶ��ʾͷ��
    audioPlayer("event.ogg", true);
    Player* player = Player::getInstance();
    auto headbar = HeaderBar::create(player);
    headbar->setPosition(Vec2(0, 1150));          // ����λ�ã�����Ļ�ϲ���
    this->addChild(headbar);
    headbar->setLocalZOrder(100);  // �� headbar �� Z ˳������Ϊ 100��ȷ����λ�����ϲ�
    // ���ñ���ͼƬ
    auto background = Sprite::create("../Resources/livingWall.jpg");
    background->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2 - 600,
        Director::getInstance()->getVisibleSize().height / 2 + 300));
    this->addChild(background);

    // �����¼��ı���ǩ
    auto continueLabel = Label::createWithSystemFont(u8"���߽�һ����·����׼��Ҫ��ͷʱ\n����ͻȻ��ǽ�ڴ��컨�����ѵ�һ������������\n������������ǽ���ϣ���ʼ����˵����\n����������֪���ģ��Ҿ������ߡ���\n�������ı䣬�Ҿ����㿴���µĵ�·����\n���������Ҫ��������ͨ������ͱ��������ɳ���", "Fonts/Kreon-Bold.ttf", 45);
    continueLabel->setPosition(Vec2(1500, 1000));
    continueLabel->setColor(Color3B::WHITE);
    this->addChild(continueLabel);

    // ������һ�� HoverButton
    auto button1 = HoverButton::create(
        "button4(1).png", "button4(2).png", "button4(1).png"
    );
    button1->setPosition(Vec2(1400, 500));
    button1->addClickEventListener([=](Ref* sender) {
        // �������Ӱ�ť����¼��Ĵ�������
        auto delay = DelayTime::create(2.0f);
        auto pop = CallFunc::create([]() {
            Director::getInstance()->popScene(); // �ӳٺ�ִ��popScene()
            });
        auto sequence = Sequence::create(delay, pop, nullptr); // ���ӳٺ�popScene��ϳ�һ�����ж���
        this->runAction(sequence); // ���������������
        });
    this->addChild(button1);
    auto continueLabel1 = Label::createWithSystemFont(u8"      ����\n�Ƴ���������һ����", "Fonts/Kreon-Bold.ttf", 45);
    continueLabel1->setPosition(Vec2(1000, 500));
    continueLabel1->setColor(Color3B::WHITE);
    this->addChild(continueLabel1);
    // �����ڶ��� HoverButton���뿪
    auto button2 = HoverButton::create(
        "button4(1).png", "button4(2).png", "button4(1).png"
    );
    button2->setPosition(Vec2(1400, 700));
    button2->addClickEventListener([=](Ref* sender) {
        // �������Ӱ�ť����¼��Ĵ�������
        // ���磬���ص���һ����
        auto delay = DelayTime::create(2.0f);
        auto pop = CallFunc::create([]() {
            Director::getInstance()->popScene(); // �ӳٺ�ִ��popScene()
            });
        auto sequence = Sequence::create(delay, pop, nullptr); // ���ӳٺ�popScene��ϳ�һ�����ж���
        this->runAction(sequence); // ���������������
        });
    this->addChild(button2);
    auto continueLabel2 = Label::createWithSystemFont(u8"      �ɳ�\n������������һ���� ", "Fonts/Kreon-Bold.ttf", 45);
    continueLabel2->setPosition(Vec2(1000, 700));
    continueLabel2->setColor(Color3B::WHITE);
    this->addChild(continueLabel2);

    return true; // ��ʼ���ɹ������� true
}

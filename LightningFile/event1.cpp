#include "event1.h"
#include "HoverButton.h"  
#include "AudioPlayer.h"
#include "const.h"
#include "Player.h"
#include "HeaderBar.h"

using namespace std;
using namespace cocos2d;

Scene* event1::createScene() {
    // ����һ���µĿճ���
    auto scene = Scene::create();
    // ���� event1 �㲢������ӵ�������
    auto layer = event1::create();
    scene->addChild(layer);
    return scene;
}

bool event1::init() {
    if (!Scene::init()) {
        return false;  // �����ʼ��ʧ���򷵻� false
    }
    // ����ͼ��
    const auto backgroun = Sprite::create("walkscene.jpg");
    backgroun->setPosition(Vec2(1100, 100));
    this->addChild(backgroun);
    // ������Ƶ��ʾͷ��
    audioPlayer("event.ogg", true);
    shared_ptr<Player> player = Player::getInstance();
    auto headbar = HeaderBar::create(player);
    headbar->setPosition(Vec2(0, 750));          // ����λ�ã�����Ļ�ϲ���
    this->addChild(headbar);
    headbar->setLocalZOrder(100);  // �� headbar �� Z ˳������Ϊ 100��ȷ����λ�����ϲ�
    // ���ñ���ͼƬ
    auto background = Sprite::create("addict.jpg");
    background->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2 -550,
        Director::getInstance()->getVisibleSize().height / 2 - 100));
    this->addChild(background);

    // �����¼��ı���ǩ
    auto continueLabel = Label::createWithSystemFont(u8"����Ҫ��һȺ���Ŷ�������Ա�͵͵Ǳ�й�ȥ\n��ʱһ��Ƥ������ĳ��������ܵ��������ǰ�� \n�����ܸ��ҵ��ʲô��, ���ѣ��������ˡ���һ��СǮ�ͺã���\n����ֻ����Ҫ�Ҹ��ط���ҹ���������вƱ����Խ���������\n�������������ģ�����û��Σ��", "Fonts/Kreon-Bold.ttf", 45);
    continueLabel->setPosition(Vec2(1350, 600));
    continueLabel->setColor(Color3B::WHITE);
    this->addChild(continueLabel);

    // ������һ�� HoverButton���������
    auto button1 = HoverButton::create(
        "button4(1).png", "button4(2).png", "button4(1).png"
    );
    button1->setPosition(Vec2(1500, 100));
    button1->addClickEventListener([=](Ref* sender) {
        // ������Ӱ�ť����¼��Ĵ������
        // ������ٽ�Ҳ��������
        if (player->coins_ <= 85) {
            auto continueLabel3 = Label::createWithSystemFont(u8"��Ҳ���", "Fonts/Kreon-Bold.ttf", 45);
            continueLabel3->setPosition(Vec2(1200, 200));
            continueLabel3->setColor(Color3B::WHITE);
            this->addChild(continueLabel3);
        }
        else {
            player->coins_ -= 85;
            headbar->updateHeader(player);
            audioPlayer("gold.ogg", false);
            //��������߼�������
            auto delay = DelayTime::create(2.0f); // ����һ��2����ӳٶ���
            auto pop = CallFunc::create([]() {
                Director::getInstance()->popScene(); // �ӳٺ�ִ��popScene()
                });
            auto sequence = Sequence::create(delay, pop, nullptr); // ���ӳٺ�popScene��ϳ�һ�����ж���
            this->runAction(sequence); // ���������������

        }
        });
    this->addChild(button1);
    auto continueLabel1= Label::createWithSystemFont(u8"����85���\n��ȡһ������", "Fonts/Kreon-Bold.ttf", 45);
    continueLabel1->setPosition(Vec2(1200, 100));
    continueLabel1->setColor(Color3B::WHITE);
    this->addChild(continueLabel1);
    // �����ڶ��� HoverButton���뿪
    auto button2 = HoverButton::create(
        "button4(1).png", "button4(2).png", "button4(1).png"
    );
    button2->setPosition(Vec2(1500, 300));
    button2->addClickEventListener([=](Ref* sender) {
        // ������Ӱ�ť����¼��Ĵ������
        // ���磬���ص���һ����
        auto delay = DelayTime::create(2.0f); 
        auto pop = CallFunc::create([]() {
            Director::getInstance()->popScene(); // �ӳٺ�ִ��popScene()
            });
        auto sequence = Sequence::create(delay, pop, nullptr); // ���ӳٺ�popScene��ϳ�һ�����ж���
        this->runAction(sequence); // ���������������
        });
    this->addChild(button2);
    auto continueLabel2 = Label::createWithSystemFont(u8"����ᣬ�뿪", "Fonts/Kreon-Bold.ttf", 45);
    continueLabel2->setPosition(Vec2(1200, 300));
    continueLabel2->setColor(Color3B::WHITE);
    this->addChild(continueLabel2);

    return true; // ��ʼ���ɹ������� true
}


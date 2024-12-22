#include <locale>                        // ���뱾�ػ�֧��
#include <codecvt>                       // �������ת��
#include "MenuScene.h"                   // ����˵�����
#include "SettingsScene.h"               // �������ó���
#include "SelectionScene.h"              // ����ѡ�񳡾�
#include "HoverButton.h"                 // ����HoverButton�࣬���ڰ�ť����ͣЧ��
#include "Enum.h"
#include "Player.h"
#include "AudioPlayer.h"

using namespace std;                    // ʹ�ñ�׼�����ռ�
using namespace cocos2d;                // ʹ��Cocos2d�����ռ�

/**
 * MenuScene�ࣺ��Ϸ���˵�������������ʼ��Ϸ����Ϸ���ú��˳���Ϸ��ť��
 */
Scene* MenuScene::createScene() {
    // ����һ���µĳ������󣬲���ӵ�ǰ������Ϊ�ӽڵ�
    auto scene = Scene::create();
    auto layer = MenuScene::create();
    scene->addChild(layer);
    return scene;
}

/**
 * ��ʼ����������ʼ���˵���������ӱ����Ͱ�ť��
 */
bool MenuScene::init() {
    if (!Scene::init()) {
        return false;  // �����ʼ��ʧ�ܣ��򷵻�false
    }

    // ���ű�������
    audioPlayer("start.ogg", true);

    // ��ȡ��Ļ�ߴ�
    const auto screenSize = cocos2d::Director::getInstance()->getVisibleSize();

    // ��ӱ���ͼƬ
    const auto background0 = Sprite::create("title4.png");
    background0->setPosition(Vec2(screenSize.width / 2+150, screenSize.height / 2-200));
    this->addChild(background0);

    const auto background1 = Sprite::create("title5.png");
    background1->setPosition(Vec2(screenSize.width / 2+150, screenSize.height / 2-200));
    this->addChild(background1);

    const auto background2 = Sprite::create("title6.png");
    background2->setPosition(Vec2(screenSize.width / 2+150, screenSize.height / 2-200));
    this->addChild(background2);

    // ������ʾ������Ƶı�ǩ
    

    shared_ptr<Player> player = Player::getInstance();
    auto playerNameLabel = Label::createWithTTF("Player: "+player->name_,
        "Fonts/FangZhengZhaoGeYuan.ttf", 40);
    playerNameLabel->setPosition(Vec2(20, screenSize.height + 150)); // ���������Ͻ�λ��
    playerNameLabel->setAnchorPoint(Vec2(0, 1));                      // ����ê��Ϊ���Ͻ�
    playerNameLabel->setTextColor(cocos2d::Color4B(255, 255, 255, 255)); // �����ı���ɫΪ��ɫ
    this->addChild(playerNameLabel);

    // ��ť���
    const float buttonSpacing = 100.0f;  // ��ť֮��ļ��
    const float buttonStartY = screenSize.height / 2 + buttonSpacing;  // ��һ����ť����ʼY����

    // ���������ÿ�ʼ��Ϸ��ť
    auto startGameButton = HoverButton::create("button2.png", "button2.png", "button2.png");
    startGameButton->setTitleText(u8"��ʼ��Ϸ");
    startGameButton->setScale(1.5f);
    startGameButton->setTitleFontSize(50);
    startGameButton->setPosition(Vec2(screenSize.width - 500, buttonStartY-200));
    startGameButton->addTouchEventListener([](Ref* sender, ui::Widget::TouchEventType type) {
        if (type == ui::Widget::TouchEventType::ENDED) {
            CCLOG("Start Game clicked!"); // ��ӡ��־
            // ���֮����г�ʼ��EventSystem
            EventSystem::getInstance()->init();

            audioPlayer("ClickSoundEffect.mp3", false);  // ���ŵ����Ч
            auto selectionScene = SelectionScene::createScene(); // ����ѡ�񳡾�
            Director::getInstance()->replaceScene(TransitionFade::create(1.0f, selectionScene, Color3B::BLACK)); // �����л�
        }
        });
    this->addChild(startGameButton);

    // ������������Ϸ���ð�ť
    auto settingsButton = HoverButton::create("button2.png", "button2.png", "button2.png");
    settingsButton->setTitleText(u8"��Ϸ����");
    settingsButton->setTitleFontSize(50);
    settingsButton->setScale(1.5f);
    settingsButton->setPosition(Vec2(screenSize.width - 500, buttonStartY - buttonSpacing-200));
    settingsButton->addTouchEventListener([](Ref* sender, ui::Widget::TouchEventType type) {
        if (type == ui::Widget::TouchEventType::ENDED) {
            CCLOG("Settings clicked!"); // ��ӡ��־
            audioPlayer("ClickSoundEffect.mp3", false);  // ���ŵ����Ч
            auto scene = SettingsScene::createScene();  // �������ó���
            Director::getInstance()->replaceScene(TransitionFade::create(1.0f, scene, Color3B::BLACK)); // �����л�
        }
        });
    this->addChild(settingsButton);

    // �����������˳���Ϸ��ť
    auto exitGameButton = HoverButton::create("button2.png", "button2.png", "button2.png");
    exitGameButton->setTitleText(u8"�˳���Ϸ");
    exitGameButton->setTitleFontSize(50);
    exitGameButton->setScale(1.5f);
    exitGameButton->setPosition(Vec2(screenSize.width - 500, buttonStartY -200-buttonSpacing * 2));
    exitGameButton->addTouchEventListener([this](Ref* sender, ui::Widget::TouchEventType type) {
        if (type == ui::Widget::TouchEventType::ENDED) {
            audioPlayer("ClickSoundEffect.mp3", false);  // ���ŵ����Ч

            // ��������Ч��
            auto blackScreen = LayerColor::create(Color4B(0, 0, 0, 255)); // ������ɫ���ǲ�
            blackScreen->setOpacity(0);  // ����Ϊ��ȫ͸��
            this->addChild(blackScreen, 100); // ��Ӻ����㵽��ǰ����

            // ִ�н���Ч����ͣ�����˳�
            auto fadeIn = FadeIn::create(0.5f); // ���䵽����
            auto delay = DelayTime::create(2.0f); // ͣ��2��
            auto exitCallback = CallFunc::create([]() {
                Director::getInstance()->end(); // �˳���Ϸ
                });

            // ˳��ִ�У����� -> ͣ�� -> �˳�
            auto sequence = Sequence::create(fadeIn, delay, exitCallback, nullptr);
            blackScreen->runAction(sequence);  // ���ж�������
        }
        });
    this->addChild(exitGameButton);

    return true; // ��ʼ���ɹ�������true
}

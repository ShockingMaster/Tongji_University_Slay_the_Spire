#include "RestScene.h"
#include "HoverButton.h"  // 引入 HoverButton 类，用于按钮的悬停效果
#include "AudioPlayer.h"
#include "StartScene.h"
#include "HelloWorldScene.h"
#include "Enum.h"
#include "Player.h"
#include "HeaderBar.h"
#include "CardLayer.h"

using namespace std;
using namespace cocos2d;

Scene* RestScene::createScene() {
    // 创建一个新的空场景
    auto scene = Scene::create();
    // 创建 RestScene 层并将其添加到场景中
    auto layer = RestScene::create();
    scene->addChild(layer);
    return scene;
}

bool RestScene::init() {
    if (!Scene::init()) {
        return false;  // 如果初始化失败则返回 false
    }
    


    // 播放背景音乐
    audioPlayer("rest.ogg", true);
    auto player = EventSystem::getInstance();
    auto headbar = HeaderBar::create(player);
    headbar->setPosition(Vec2(0, 750));          // 设置位置（在屏幕上部）
    this->addChild(headbar);
    headbar->setLocalZOrder(100);  // 将 headbar 的 Z 顺序设置为 100，确保它位于最上层
    auto background = Sprite::create("room.jpg");
    background->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2,
        Director::getInstance()->getVisibleSize().height / 2));
    background->setScale(1.4f);
    background->setOpacity(100);
    this->addChild(background);
    auto smithButton = HoverButton::create("smith.png", "smith.png", "smith.png");

    // 创建休息按钮
    auto restButton = HoverButton::create("sleep.png", "sleep.png", "sleep.png");
    restButton->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 4,
        Director::getInstance()->getVisibleSize().height / 2));

    restButton->addClickEventListener([this, headbar, restButton,smithButton](Ref* sender) {
        restButton->setEnabled(false);
        smithButton->setEnabled(false);
        audioPlayer("addhealth.ogg", false); 
        auto player = EventSystem::getInstance();
        // 恢复最大生命值的 30%
        player->changeHealth(0.3f * player->getFullHealth());
        // 更新头栏中的血量显示
        headbar->updateHeader(player);

        // 创建返回按钮
        auto returnButton = HoverButton::create("button4(1).png", "button4(2).png", "button4(1).png");
        returnButton->setPosition(Vec2(1000, 100));
        returnButton->addClickEventListener([this](Ref* sender) {
            // 传递玩家数据并返回地图
            audioPlayer("clickSoundEffect.mp3", false);
            Director::getInstance()->popScene();
            });
        returnButton->setVisible(false);
        this->addChild(returnButton);

        // 创建 "继续" 标签
        auto continueLabel = Label::createWithSystemFont(u8"继续", "Fonts/Kreon-Bold.ttf", 40);
        continueLabel->setPosition(Vec2(1000, 100));
        continueLabel->setColor(Color3B::WHITE);
        this->addChild(continueLabel);

        // 显示返回按钮
        auto showReturnButton = CallFunc::create([returnButton]() {
            returnButton->setVisible(true);
            });
        this->runAction(Sequence::create(showReturnButton, nullptr));
        });


    this->addChild(restButton);

    // 创建锻造卡牌按钮
    smithButton->setPosition(Vec2(3 * Director::getInstance()->getVisibleSize().width / 4,
        Director::getInstance()->getVisibleSize().height / 2));
    smithButton->addClickEventListener([this,smithButton,restButton](Ref* sender) {
        // 升级一张卡牌
        restButton->setEnabled(false);
        smithButton->setEnabled(false);
        auto player = EventSystem::getInstance();
        auto cardLayer = CardLayer::create(player->cards_, 3); 
        Director::getInstance()->getRunningScene()->addChild(cardLayer); // 使用更高的层级，确保在 Header
       // 创建返回按钮
        auto returnButton = HoverButton::create("button4(1).png", "button4(2).png", "button4(1).png");
        returnButton->setPosition(Vec2(1000, 100));
        returnButton->addClickEventListener([this](Ref* sender) {
            // 点击“继续”按钮时执行的逻辑
            audioPlayer("clickSoundEffect.mp3", false);
            Director::getInstance()->popScene();
            });
        returnButton->setVisible(false); // 初始时隐藏返回按钮
        this->addChild(returnButton);

        // 创建 "继续" 标签
        auto continueLabel = Label::createWithSystemFont(u8"继续", "Fonts/Kreon-Bold.ttf", 40);
        continueLabel->setPosition(Vec2(1000, 100));
        continueLabel->setColor(Color3B::WHITE);
        this->addChild(continueLabel);


        // 显示返回按钮
        auto showReturnButton = CallFunc::create([returnButton]() {
            returnButton->setVisible(true);
            });
        this->runAction(Sequence::create(showReturnButton, nullptr));
        });
    this->addChild(smithButton);
   
    // 添加按钮上方的说明文字
    auto restLabel = Label::createWithSystemFont(u8"休息 回复最大生命值的30%", "Fonts/Kreon-Bold.ttf", 60);
    restLabel->setPosition(Vec2(restButton->getPosition().x, restButton->getPosition().y -150));
    restLabel->setColor(Color3B::WHITE);
    this->addChild(restLabel);

    auto smithLabel = Label::createWithSystemFont(u8"锻造 升级一张卡牌", "Fonts/Kreon-Bold.ttf", 60);
    smithLabel->setPosition(Vec2(smithButton->getPosition().x, smithButton->getPosition().y-150));
    smithLabel->setColor(Color3B::WHITE);
    this->addChild(smithLabel);

    return true; // 初始化成功，返回 true
}

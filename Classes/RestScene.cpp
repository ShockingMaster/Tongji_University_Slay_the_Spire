#include "RestScene.h"
#include "HoverButton.h"  // 引入 HoverButton 类，用于按钮的悬停效果
#include "AudioPlayer.h"
#include "StartScene.h"
#include "HelloWorldScene.h"
#include "const.h"
#include "Player.h"
#include "HeaderBar.h"



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
    Player* player = Player::getInstance();
    // 播放背景音乐
    audioPlayer("../Resources/rest.ogg", true);
    auto headbar = HeaderBar::create(player);
    headbar->setPosition(Vec2(0, 1150));          // 设置位置（在屏幕上部）
    this->addChild(headbar);
    headbar->setLocalZOrder(100);  // 将 headbar 的 Z 顺序设置为 100，确保它位于最上层
    auto background = Sprite::create("../Resources/room.jpg");
    background->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2,
        Director::getInstance()->getVisibleSize().height / 2+200));
    background->setScale(1.4f);
    background->setOpacity(100);
    this->addChild(background);

    // 创建休息按钮
    auto restButton = HoverButton::create("sleep.png", "sleep.png", "sleep.png");
    restButton->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 4,
        Director::getInstance()->getVisibleSize().height / 2));

    restButton->addClickEventListener([this, headbar](Ref* sender) {
        audioPlayer("addhealth.ogg", false); 
        Player* player = Player::getInstance();
        // 恢复最大生命值的 30%
        player->health_ += player->fullhealth_ * 0.3f;
        // 确保血量不超过最大血量
        if (player->health_ > player->fullhealth_) {
            player->health_ = player->fullhealth_;  // 如果恢复后的血量超过最大血量，设为最大血量
        }
        // 更新头栏中的血量显示
        headbar->updateHeader(player);

        // 创建返回按钮
        auto returnButton = HoverButton::create("button4(1).png", "button4(2).png", "button4(1).png");
        returnButton->setPosition(Vec2(1000, 300));
        returnButton->addClickEventListener([this](Ref* sender) {
            // 传递玩家数据并返回地图
            audioPlayer("clickSoundEffect.mp3", false);
            Director::getInstance()->popScene();
            });
        returnButton->setVisible(false);
        this->addChild(returnButton);

        // 创建 "继续" 标签
        auto continueLabel = Label::createWithSystemFont(u8"继续", "Fonts/Kreon-Bold.ttf", 40);
        continueLabel->setPosition(Vec2(1000, 300));
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
    auto smithButton = HoverButton::create("smith.png", "smith.png", "smith.png");
    smithButton->setPosition(Vec2(3 * Director::getInstance()->getVisibleSize().width / 4,
        Director::getInstance()->getVisibleSize().height / 2));
    smithButton->addClickEventListener([this](Ref* sender) {
        // 升级一张卡牌
       // 创建返回按钮
        auto returnButton = HoverButton::create("button4(1).png", "button4(2).png", "button4(1).png");
        returnButton->setPosition(Vec2(1000, 300));
        returnButton->addClickEventListener([this](Ref* sender) {
            // 点击“继续”按钮时执行的逻辑
            audioPlayer("clickSoundEffect.mp3", false);
            Director::getInstance()->popScene();
            });
        returnButton->setVisible(false); // 初始时隐藏返回按钮
        this->addChild(returnButton);

        // 创建 "继续" 标签
        auto continueLabel = Label::createWithSystemFont(u8"继续", "Fonts/Kreon-Bold.ttf", 40);
        continueLabel->setPosition(Vec2(1000, 300));
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
    restLabel->setPosition(Vec2(restButton->getPosition().x, restButton->getPosition().y + 150));
    restLabel->setColor(Color3B::WHITE);
    this->addChild(restLabel);

    auto smithLabel = Label::createWithSystemFont(u8"锻造 升级一张卡牌", "Fonts/Kreon-Bold.ttf", 60);
    smithLabel->setPosition(Vec2(smithButton->getPosition().x, smithButton->getPosition().y + 150));
    smithLabel->setColor(Color3B::WHITE);
    this->addChild(smithLabel);

    return true; // 初始化成功，返回 true
}

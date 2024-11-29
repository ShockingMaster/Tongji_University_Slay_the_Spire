#include "RestScene.h"
#include "HoverButton.h"  // 引入 HoverButton 类，用于按钮的悬停效果
#include "AudioPlayer.h"
#include "StartScene.h"
#include "HelloWorldScene.h"
#include "const.h"
#include "Player.h"


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
    audioPlayer("../Resources/rest.ogg", true);

    /* 创建背景（假设有一个背景图）
    auto background = Sprite::create("../Resources/scene1.png");
    background->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2,
        Director::getInstance()->getVisibleSize().height / 2));
    this->addChild(background);*/

    // 创建休息按钮
    auto restButton = HoverButton::create("sleep.png", "sleep.png", "sleep.png");
    restButton->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 4,
        Director::getInstance()->getVisibleSize().height / 2));
    restButton->addClickEventListener([this](Ref* sender) {
        // 恢复最大生命值的 30%
       // 创建返回按钮
        auto returnButton = HoverButton::create("button4(1).png", "button4(2).png", "button4(1).png");
        returnButton->setPosition(Vec2(1000, 300));
        returnButton->addClickEventListener([this](Ref* sender) {
            // 点击“继续”按钮时执行的逻辑
            Director::getInstance()->popScene();
            });
        returnButton->setVisible(false); // 初始时隐藏返回按钮
        this->addChild(returnButton);

        // 创建 "继续" 标签
        auto continueLabel = Label::createWithSystemFont(u8"继续", "Fonts/FangZhengZhaoGeYuan.ttf", 40);
        continueLabel->setPosition(Vec2(1000,300)); 
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
            Director::getInstance()->popScene(); 
            });
        returnButton->setVisible(false); // 初始时隐藏返回按钮
        this->addChild(returnButton);

        // 创建 "继续" 标签
        auto continueLabel = Label::createWithSystemFont(u8"继续", "Fonts/FangZhengZhaoGeYuan.ttf", 40);
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
    auto InstructLabel = Label::createWithSystemFont(u8"欢迎来到休息房间", "Fonts/FangZhengZhaoGeYuan.ttf", 60);
    InstructLabel->setPosition(Vec2(restButton->getPosition().x+500, restButton->getPosition().y + 500));
    InstructLabel->setColor(Color3B::WHITE);
    this->addChild(InstructLabel);
    // 添加按钮上方的说明文字
    auto restLabel = Label::createWithSystemFont(u8"休息 回复最大生命值的30%", "Fonts/FangZhengZhaoGeYuan.ttf", 50);
    restLabel->setPosition(Vec2(restButton->getPosition().x, restButton->getPosition().y + 200));
    restLabel->setColor(Color3B::WHITE);
    this->addChild(restLabel);

    auto smithLabel = Label::createWithSystemFont(u8"锻造 升级一张卡牌", "Fonts/FangZhengZhaoGeYuan.ttf", 50);
    smithLabel->setPosition(Vec2(smithButton->getPosition().x, smithButton->getPosition().y + 200));
    smithLabel->setColor(Color3B::WHITE);
    this->addChild(smithLabel);

    return true; // 初始化成功，返回 true
}

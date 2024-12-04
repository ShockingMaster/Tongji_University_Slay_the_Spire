#include "event3.h"
#include "HoverButton.h"  
#include "AudioPlayer.h"
#include "const.h"
#include "Player.h"
#include "HeaderBar.h"
#include "CardLayer.h"
using namespace std;
using namespace cocos2d;

Scene* event3::createScene() {
    // 创建一个新的空场景
    auto scene = Scene::create();
    // 创建 event1 层并将其添加到场景中
    auto layer = event3::create();
    scene->addChild(layer);
    return scene;
}
bool event3::init() {
    if (!Scene::init()) {
        return false;  // 如果初始化失败则返回 false
    }
    const auto backgroun = Sprite::create("walkscene.jpg");
    backgroun->setPosition(Vec2(1100, 300));
    this->addChild(backgroun);
    // 播放音频显示头栏
    audioPlayer("event.ogg", true);
    Player* player = Player::getInstance();
    auto headbar = HeaderBar::create(player);
    headbar->setPosition(Vec2(0, 1150));          // 设置位置（在屏幕上部）
    this->addChild(headbar);
    headbar->setLocalZOrder(100);  // 将 headbar 的 Z 顺序设置为 100，确保它位于最上层
    // 设置背景图片
    auto background = Sprite::create("../Resources/nopants.png");
    background->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2 - 600,
        Director::getInstance()->getVisibleSize().height / 2 -200));
    this->addChild(background);

    // 创建事件文本标签
    auto continueLabel = Label::createWithSystemFont(u8"你发现地上有一具冒险者尸体。\n尽管他随身携带的东西好像都还在\n你实在是不想知道这里究竟发生了什么……\n他的裤子都被偷了！", "../Resources/Fonts/FangZhengZhaoGeYuan.ttf", 45);
    continueLabel->setPosition(Vec2(1500, 1000));
    continueLabel->setColor(Color3B::WHITE);
    this->addChild(continueLabel);

    // 创建第一个 HoverButton
    auto button1 = HoverButton::create(
        "button4(1).png", "button4(2).png", "button4(1).png"
    );
    button1->setPosition(Vec2(1600, 500));
    button1->addClickEventListener([=](Ref* sender) {
        // 这里添加按钮点击事件的处理代码
        
        auto returnButton = HoverButton::create("button4(1).png", "button4(2).png", "button4(1).png");
        returnButton->setPosition(Vec2(1000, 300));
        returnButton->addClickEventListener([this](Ref* sender) {
            // 点击“继续”按钮时执行的逻辑
            audioPlayer("clickSoundEffect.mp3", false);
            Director::getInstance()->popScene();
            });
        this->addChild(returnButton);

        // 创建 "继续" 标签
        auto continueLabel = Label::createWithSystemFont(u8"继续", "Fonts/Kreon-Bold.ttf", 40);
        continueLabel->setPosition(Vec2(1000, 300));
        continueLabel->setColor(Color3B::WHITE);
        this->addChild(continueLabel);
        });
    this->addChild(button1);  
    auto continueLabel1 = Label::createWithSystemFont(u8"搜索 寻找尸体上的东西\n25%遇见回来的怪物。", "Fonts/../Resources/Fonts/FangZhengZhaoGeYuan.ttf", 45);
    continueLabel1->setPosition(Vec2(1200, 500));
    continueLabel1->setColor(Color3B::WHITE);
    this->addChild(continueLabel1);
    // 创建第二个 HoverButton：离开
    auto button2 = HoverButton::create(
        "button4(1).png", "button4(2).png", "button4(1).png"
    );
    button2->setPosition(Vec2(1600, 700));
    button2->addClickEventListener([=](Ref* sender) {
        // 这里添加按钮点击事件的处理代码
        // 例如，返回到上一场景 Player* player = Player::getInstance();
        
        auto returnButton = HoverButton::create("button4(1).png", "button4(2).png", "button4(1).png");
        returnButton->setPosition(Vec2(1000, 300));
        returnButton->addClickEventListener([this](Ref* sender) {
            // 点击“继续”按钮时执行的逻辑
            audioPlayer("clickSoundEffect.mp3", false);
            Director::getInstance()->popScene();
            });
        this->addChild(returnButton);

        // 创建 "继续" 标签
        auto continueLabel = Label::createWithSystemFont(u8"继续", "Fonts/Kreon-Bold.ttf", 40);
        continueLabel->setPosition(Vec2(1000, 300));
        continueLabel->setColor(Color3B::WHITE);
        this->addChild(continueLabel);
        });
    this->addChild(button2);
    auto continueLabel2 = Label::createWithSystemFont(u8"      离开\n你一声不发地离开了。 ", "../Resources/Fonts/FangZhengZhaoGeYuan.ttf", 45);
    continueLabel2->setPosition(Vec2(1200, 700));
    continueLabel2->setColor(Color3B::WHITE);
    this->addChild(continueLabel2);

    return true; // 初始化成功，返回 true
}


/*你走进一条死路，正准备要回头时，四周突然有墙壁从天花板上哐地一下砸了下来！
三张脸出现在墙壁上，开始对你说话：
“忘记你所知道的，我就让你走。”
“有所改变，我就让你看见新的道路。”
“如果你想要从我这里通过，你就必须有所成长。”*/
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
    // 创建一个新的空场景
    auto scene = Scene::create();
    // 创建 event1 层并将其添加到场景中
    auto layer = event2::create();
    scene->addChild(layer);
    return scene;
}

bool event2::init() {
    if (!Scene::init()) {
        return false;  // 如果初始化失败则返回 false
    }
    const auto backgroun = Sprite::create("walkscene.jpg");
    backgroun->setPosition(Vec2(1100, 100));
    this->addChild(backgroun);
    // 播放音频显示头栏

    audioPlayer("event.ogg", true);
    auto player = EventSystem::getInstance();
    auto headbar = HeaderBar::create(player);
    headbar->updateHeader(player);
    headbar->setPosition(Vec2(0, 750));          // 设置位置（在屏幕上部）
    this->addChild(headbar);

    headbar->setLocalZOrder(100);  // 将 headbar 的 Z 顺序设置为 100，确保它位于最上层
    // 设置背景图片
    auto background = Sprite::create("livingWall.jpg");
    background->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2 - 550,
        Director::getInstance()->getVisibleSize().height / 2 - 100));
    this->addChild(background);

    // 创建事件文本标签
    auto continueLabel = Label::createWithSystemFont(u8"你走进一条死路，正准备要回头时\n四周突然有墙壁从天花板上哐地一下砸了下来！\n三张脸出现在墙壁上，开始对你说话：\n“忘记你所知道的，我就让你走。”\n“有所改变，我就让你看见新的道路。”\n“如果你想要从我这里通过，你就必须有所成长。", "Fonts/FangZhengZhaoGeYuan.ttf", 45);
    continueLabel->setPosition(Vec2(1300, 600));
    continueLabel->setColor(Color3B::WHITE);
    this->addChild(continueLabel);
     
    // 创建第一个 HoverButton
    auto button1 = HoverButton::create(
        "button4(1).png", "button4(2).png", "button4(1).png"
    );
    button1->setPosition(Vec2(1600, 100));
    button1->addClickEventListener([=](Ref* sender) {
        // 这里添加按钮点击事件的处理代码
        auto player= EventSystem::getInstance();
        auto cardLayer = CardLayer::create(player->cards_, 2);
        Director::getInstance()->getRunningScene()->addChild(cardLayer);
        auto returnButton = HoverButton::create("button4(1).png", "button4(2).png", "button4(1).png");
        returnButton->setPosition(Vec2(1000, 100));
        returnButton->addClickEventListener([this](Ref* sender) {
            // 点击“继续”按钮时执行的逻辑
            audioPlayer("clickSoundEffect.mp3", false);
            Director::getInstance()->popScene();
            });
        this->addChild(returnButton);

        // 创建 "继续" 标签
        auto continueLabel = Label::createWithSystemFont(u8"继续", "Fonts/Kreon-Bold.ttf", 40);
        continueLabel->setPosition(Vec2(1000, 100));
        continueLabel->setColor(Color3B::WHITE);
        this->addChild(continueLabel);
        });
    this->addChild(button1);
    auto continueLabel1 = Label::createWithSystemFont(u8"      遗忘\n移除你牌组中一张牌", "Fonts/FangZhengZhaoGeYuan.ttf", 45);
    continueLabel1->setPosition(Vec2(1200, 100));
    continueLabel1->setColor(Color3B::WHITE);
    this->addChild(continueLabel1);
    // 创建第二个 HoverButton：离开
    auto button2 = HoverButton::create(
        "button4(1).png", "button4(2).png", "button4(1).png"
    );
    button2->setPosition(Vec2(1600, 300));
    button2->addClickEventListener([=](Ref* sender) {
        // 这里添加按钮点击事件的处理代码
        // 例如，返回到上一场景 shared_ptr<Player> player= Player::getInstance();
        auto cardLayer = CardLayer::create(player->cards_, 3); 
        Director::getInstance()->getRunningScene()->addChild(cardLayer);
        auto returnButton = HoverButton::create("button4(1).png", "button4(2).png", "button4(1).png");
        returnButton->setPosition(Vec2(1000, 100));
        returnButton->addClickEventListener([this](Ref* sender) {
            // 点击“继续”按钮时执行的逻辑
            audioPlayer("clickSoundEffect.mp3", false);
            Director::getInstance()->popScene();
            });
        this->addChild(returnButton);

        // 创建 "继续" 标签
        auto continueLabel = Label::createWithSystemFont(u8"继续", "Fonts/Kreon-Bold.ttf", 40);
        continueLabel->setPosition(Vec2(1000, 100));
        continueLabel->setColor(Color3B::WHITE);
        this->addChild(continueLabel);
        });
    this->addChild(button2);
    auto continueLabel2 = Label::createWithSystemFont(u8"      成长\n升级你牌组中一张牌 ", "Fonts/FangZhengZhaoGeYuan.ttf", 45);
    continueLabel2->setPosition(Vec2(1200, 300));
    continueLabel2->setColor(Color3B::WHITE);
    this->addChild(continueLabel2);

    return true; // 初始化成功，返回 true
}


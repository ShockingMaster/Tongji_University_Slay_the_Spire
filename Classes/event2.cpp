/*你走进一条死路，正准备要回头时，四周突然有墙壁从天花板上哐地一下砸了下来！
三张脸出现在墙壁上，开始对你说话：
“忘记你所知道的，我就让你走。”
“有所改变，我就让你看见新的道路。”
“如果你想要从我这里通过，你就必须有所成长。”*/
#include "event2.h"
#include "HoverButton.h"  
#include "AudioPlayer.h"
#include "const.h"
#include "Player.h"
#include "HeaderBar.h"

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

    // 播放音频显示头栏
    audioPlayer("event.ogg", true);
    Player* player = Player::getInstance();
    auto headbar = HeaderBar::create(player);
    headbar->setPosition(Vec2(0, 1150));          // 设置位置（在屏幕上部）
    this->addChild(headbar);
    headbar->setLocalZOrder(100);  // 将 headbar 的 Z 顺序设置为 100，确保它位于最上层
    // 设置背景图片
    auto background = Sprite::create("../Resources/livingWall.jpg");
    background->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2 - 600,
        Director::getInstance()->getVisibleSize().height / 2 + 300));
    this->addChild(background);

    // 创建事件文本标签
    auto continueLabel = Label::createWithSystemFont(u8"你走进一条死路，正准备要回头时\n四周突然有墙壁从天花板上哐地一下砸了下来！\n三张脸出现在墙壁上，开始对你说话：\n“忘记你所知道的，我就让你走。”\n“有所改变，我就让你看见新的道路。”\n“如果你想要从我这里通过，你就必须有所成长。", "Fonts/Kreon-Bold.ttf", 45);
    continueLabel->setPosition(Vec2(1500, 1000));
    continueLabel->setColor(Color3B::WHITE);
    this->addChild(continueLabel);

    // 创建第一个 HoverButton
    auto button1 = HoverButton::create(
        "button4(1).png", "button4(2).png", "button4(1).png"
    );
    button1->setPosition(Vec2(1400, 500));
    button1->addClickEventListener([=](Ref* sender) {
        // 这里添加按钮点击事件的处理代码
        auto delay = DelayTime::create(2.0f);
        auto pop = CallFunc::create([]() {
            Director::getInstance()->popScene(); // 延迟后执行popScene()
            });
        auto sequence = Sequence::create(delay, pop, nullptr); // 将延迟和popScene组合成一个序列动作
        this->runAction(sequence); // 运行这个动作序列
        });
    this->addChild(button1);
    auto continueLabel1 = Label::createWithSystemFont(u8"      遗忘\n移除你牌组中一张牌", "Fonts/Kreon-Bold.ttf", 45);
    continueLabel1->setPosition(Vec2(1000, 500));
    continueLabel1->setColor(Color3B::WHITE);
    this->addChild(continueLabel1);
    // 创建第二个 HoverButton：离开
    auto button2 = HoverButton::create(
        "button4(1).png", "button4(2).png", "button4(1).png"
    );
    button2->setPosition(Vec2(1400, 700));
    button2->addClickEventListener([=](Ref* sender) {
        // 这里添加按钮点击事件的处理代码
        // 例如，返回到上一场景
        auto delay = DelayTime::create(2.0f);
        auto pop = CallFunc::create([]() {
            Director::getInstance()->popScene(); // 延迟后执行popScene()
            });
        auto sequence = Sequence::create(delay, pop, nullptr); // 将延迟和popScene组合成一个序列动作
        this->runAction(sequence); // 运行这个动作序列
        });
    this->addChild(button2);
    auto continueLabel2 = Label::createWithSystemFont(u8"      成长\n升级你牌组中一张牌 ", "Fonts/Kreon-Bold.ttf", 45);
    continueLabel2->setPosition(Vec2(1000, 700));
    continueLabel2->setColor(Color3B::WHITE);
    this->addChild(continueLabel2);

    return true; // 初始化成功，返回 true
}


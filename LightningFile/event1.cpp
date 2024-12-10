#include "event1.h"
#include "HoverButton.h"  
#include "AudioPlayer.h"
#include "const.h"
#include "Player.h"
#include "HeaderBar.h"

using namespace std;
using namespace cocos2d;

Scene* event1::createScene() {
    // 创建一个新的空场景
    auto scene = Scene::create();
    // 创建 event1 层并将其添加到场景中
    auto layer = event1::create();
    scene->addChild(layer);
    return scene;
}

bool event1::init() {
    if (!Scene::init()) {
        return false;  // 如果初始化失败则返回 false
    }
    // 背景图像
    const auto backgroun = Sprite::create("walkscene.jpg");
    backgroun->setPosition(Vec2(1100, 100));
    this->addChild(backgroun);
    // 播放音频显示头栏
    audioPlayer("event.ogg", true);
    shared_ptr<Player> player = Player::getInstance();
    auto headbar = HeaderBar::create(player);
    headbar->setPosition(Vec2(0, 750));          // 设置位置（在屏幕上部）
    this->addChild(headbar);
    headbar->setLocalZOrder(100);  // 将 headbar 的 Z 顺序设置为 100，确保它位于最上层
    // 设置背景图片
    auto background = Sprite::create("addict.jpg");
    background->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2 -550,
        Director::getInstance()->getVisibleSize().height / 2 - 100));
    this->addChild(background);

    // 创建事件文本标签
    auto continueLabel = Label::createWithSystemFont(u8"你想要从一群披着斗篷的人旁边偷偷潜行过去\n这时一个皮肤发红的赤裸男人跑到了你的面前。 \n“你能给我点儿什么吗, 朋友？求求你了……一点小钱就好？”\n“我只是需要找个地方过夜，我身上有财宝可以交换……”\n他看起来疯疯癫癫的，但并没有危险", "Fonts/Kreon-Bold.ttf", 45);
    continueLabel->setPosition(Vec2(1350, 600));
    continueLabel->setColor(Color3B::WHITE);
    this->addChild(continueLabel);

    // 创建第一个 HoverButton：给他金币
    auto button1 = HoverButton::create(
        "button4(1).png", "button4(2).png", "button4(1).png"
    );
    button1->setPosition(Vec2(1500, 100));
    button1->addClickEventListener([=](Ref* sender) {
        // 这里添加按钮点击事件的处理代码
        // 比如减少金币并获得遗物
        if (player->coins_ <= 85) {
            auto continueLabel3 = Label::createWithSystemFont(u8"金币不足", "Fonts/Kreon-Bold.ttf", 45);
            continueLabel3->setPosition(Vec2(1200, 200));
            continueLabel3->setColor(Color3B::WHITE);
            this->addChild(continueLabel3);
        }
        else {
            player->coins_ -= 85;
            headbar->updateHeader(player);
            audioPlayer("gold.ogg", false);
            //获得遗物逻辑待更新
            auto delay = DelayTime::create(2.0f); // 创建一个2秒的延迟动作
            auto pop = CallFunc::create([]() {
                Director::getInstance()->popScene(); // 延迟后执行popScene()
                });
            auto sequence = Sequence::create(delay, pop, nullptr); // 将延迟和popScene组合成一个序列动作
            this->runAction(sequence); // 运行这个动作序列

        }
        });
    this->addChild(button1);
    auto continueLabel1= Label::createWithSystemFont(u8"给他85金币\n换取一个遗物", "Fonts/Kreon-Bold.ttf", 45);
    continueLabel1->setPosition(Vec2(1200, 100));
    continueLabel1->setColor(Color3B::WHITE);
    this->addChild(continueLabel1);
    // 创建第二个 HoverButton：离开
    auto button2 = HoverButton::create(
        "button4(1).png", "button4(2).png", "button4(1).png"
    );
    button2->setPosition(Vec2(1500, 300));
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
    auto continueLabel2 = Label::createWithSystemFont(u8"不理会，离开", "Fonts/Kreon-Bold.ttf", 45);
    continueLabel2->setPosition(Vec2(1200, 300));
    continueLabel2->setColor(Color3B::WHITE);
    this->addChild(continueLabel2);

    return true; // 初始化成功，返回 true
}


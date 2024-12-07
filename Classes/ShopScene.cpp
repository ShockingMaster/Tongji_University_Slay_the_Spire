#include "ShopScene.h"
#include "AudioPlayer.h"
#include "NodeConnection.h"
#include "Player.h"
#include "HeaderBar.h"

// 创建场景
cocos2d::Scene* ShopScene::createScene() {
    auto scene = Scene::create();          // 创建一个空白场景
    auto layer = ShopScene::create();     
    scene->addChild(layer);                // 将层添加到场景
    return scene;
}


bool ShopScene::init() {
    if (!Scene::init()) {
        return false;
    }

    // 播放背景音效
    audioPlayer("../Resources/event.ogg", true);


    Player* player = Player::getInstance();
    auto headbar = HeaderBar::create(player);
    headbar->setPosition(Vec2(0, 1150));          // 设置位置（在屏幕上部）
    this->addChild(headbar);
    headbar->setLocalZOrder(100);
    // 背景图像
    const auto background = Sprite::create("walkscene.jpg");
    background->setPosition(Vec2(1100, 300));
    this->addChild(background);

    // 欢迎标签
    auto continueLabel = Label::createWithSystemFont(u8"“欢迎来到商店！”\n一个戴面具的商人\n出现在你的面前", "Fonts/Kreon-Bold.ttf", 65);
    continueLabel->setPosition(Vec2(600, 800));
    continueLabel->setColor(Color3B::WHITE);
    this->addChild(continueLabel);

    // 商人背景图像
    const auto background0 = Sprite::create("merchant-removebg-preview.png");
    background0->setScale(1.0f);
    background0->setOpacity(200);
    background0->setPosition(Vec2(1500, 600));
    this->addChild(background0);

    // 使用 DelayTime 来创建延迟
    auto delay = DelayTime::create(3.0f); 

    // 创建一个回调函数，在延迟时间后执行
    auto showLayerCallback = CallFunc::create([this]() {
        // 创建新层，并设置背景图
        auto newLayer = Sprite::create("fin.png");
        newLayer->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2,
            Director::getInstance()->getVisibleSize().height / 2 + 190));
        newLayer->setScale(0.9f);
        this->addChild(newLayer, 100); // 100是层级，确保在其他元素之上

        // 创建返回按钮
        auto backButton = MenuItemImage::create(
            "cancelButton.png",    // 普通状态图片
            "cancelButtonOutline.png",   // 按下状态图片
            [this](Ref* sender) { // 点击按钮时执行
                // 返回到上一个场景
                Director::getInstance()->popScene();  // 弹出当前场景并返回前一个场景
            }
        );

        // 设置按钮位置
        backButton->setPosition(Vec2(Director::getInstance()->getVisibleSize().width - 100, 100)); 

        // 创建菜单并添加按钮
        auto menu = Menu::create(backButton, nullptr);
        menu->setPosition(Vec2::ZERO);  // 菜单的锚点设置为 (0, 0)
        newLayer->addChild(menu);  // 将按钮添加到 newLayer

        });


    // 创建一个顺序动作
    auto sequence = Sequence::create(delay, showLayerCallback, nullptr);

    // 执行动作
    this->runAction(sequence);

    return true;
}

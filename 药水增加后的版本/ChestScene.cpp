#include "ChestScene.h"
#include "AudioPlayer.h"
#include "NodeConnection.h"
#include "Player.h"
#include "HeaderBar.h"
#include "HoverButton.h"
#include "RewardLayer.h"


// 创建场景
cocos2d::Scene* ChestScene::createScene() {
    auto scene = Scene::create();          // 创建一个空白场景
    auto layer = ChestScene::create();
    scene->addChild(layer);                // 将层添加到场景
    return scene;
}


bool ChestScene::init() {
    if (!Scene::init()) {
        return false;
    }

    // 播放背景音效
    audioPlayer("chest.ogg", true);
    // 背景图像
    const auto background = Sprite::create("scene3.png");
    background->setPosition(Vec2(1100, 300));
    this->addChild(background);

    // 创建宝箱并设置位置
    auto chest = Sprite::create("largeChest.png");
    chest->setPosition(Vec2(400, 500));
    this->addChild(chest);
    auto Label1 = cocos2d::Label::createWithSystemFont(u8"点击获得宝藏", "Arial", 60);
    Label1->setTextColor(cocos2d::Color4B::WHITE);
    Label1->setPosition(cocos2d::Vec2(300, 200));  // 设置Label的位置
    this->addChild(Label1, 103);  // Label的层级高于卡片
    // 创建鼠标事件监听器
    auto mouseListener = EventListenerMouse::create();

    // 定义鼠标点击的回调函数
    mouseListener->onMouseDown = [chest](Event* event) {
        // 获取鼠标点击的位置
        EventMouse* mouseEvent = (EventMouse*)event;
        Vec2 clickPosition = mouseEvent->getLocationInView();

        // 获取宝箱的矩形区域
        Rect chestRect = chest->getBoundingBox();

        // 判断鼠标点击点是否在宝箱的区域内
        if (chestRect.containsPoint(clickPosition)) {
            // 宝箱被点击，播放开宝箱的音效
            audioPlayer("SOTE_SFX_ChestOpen_v2", false);

            // 更换宝箱的图像为已打开状态
            chest->setTexture("largeChestOpened.png");  // 替换为已打开的宝箱图像

            // 创建 RewardLayer 并显示奖励
            RewardLayer* rewardLayer = RewardLayer::create(true,true,false,false,true);
            Director::getInstance()->getRunningScene()->addChild(rewardLayer);  // 将 RewardLayer 添加到当前场景
        }
        event->stopPropagation();
        };

    // 注册鼠标事件监听器
    _eventDispatcher->addEventListenerWithSceneGraphPriority(mouseListener, this);

    // 创建 HoverButton
    auto startButton = HoverButton::create(
        "button1 (1).png",  // 默认图片
        "button1 (2).png",  // 按钮悬停时的图片
        "button1 (3).png"   // 按钮点击时的图片
    );

    // 设置按钮位置
    startButton->setPosition(Vec2(1800, 300));
    this->addChild(startButton);

    // 添加按钮点击事件监听器
    startButton->addClickEventListener([=](Ref* sender) {
        // 执行 popScene 操作，返回上一个场景
        Director::getInstance()->popScene();
        });

    return true;
}


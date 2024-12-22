#include <locale>                        // 引入本地化支持
#include <codecvt>                       // 引入编码转换
#include "MenuScene.h"                   // 引入菜单场景
#include "SettingsScene.h"               // 引入设置场景
#include "SelectionScene.h"              // 引入选择场景
#include "HoverButton.h"                 // 引入HoverButton类，用于按钮的悬停效果
#include "Enum.h"
#include "Player.h"
#include "AudioPlayer.h"

using namespace std;                    // 使用标准命名空间
using namespace cocos2d;                // 使用Cocos2d命名空间

/**
 * MenuScene类：游戏主菜单场景，包含开始游戏、游戏设置和退出游戏按钮。
 */
Scene* MenuScene::createScene() {
    // 创建一个新的场景对象，并添加当前场景作为子节点
    auto scene = Scene::create();
    auto layer = MenuScene::create();
    scene->addChild(layer);
    return scene;
}

/**
 * 初始化函数：初始化菜单场景并添加背景和按钮。
 */
bool MenuScene::init() {
    if (!Scene::init()) {
        return false;  // 如果初始化失败，则返回false
    }

    // 播放背景音乐
    audioPlayer("start.ogg", true);

    // 获取屏幕尺寸
    const auto screenSize = cocos2d::Director::getInstance()->getVisibleSize();

    // 添加背景图片
    const auto background0 = Sprite::create("title4.png");
    background0->setPosition(Vec2(screenSize.width / 2+150, screenSize.height / 2-200));
    this->addChild(background0);

    const auto background1 = Sprite::create("title5.png");
    background1->setPosition(Vec2(screenSize.width / 2+150, screenSize.height / 2-200));
    this->addChild(background1);

    const auto background2 = Sprite::create("title6.png");
    background2->setPosition(Vec2(screenSize.width / 2+150, screenSize.height / 2-200));
    this->addChild(background2);

    // 创建显示玩家名称的标签
    

    shared_ptr<Player> player = Player::getInstance();
    auto playerNameLabel = Label::createWithTTF("Player: "+player->name_,
        "Fonts/FangZhengZhaoGeYuan.ttf", 40);
    playerNameLabel->setPosition(Vec2(20, screenSize.height + 150)); // 设置在左上角位置
    playerNameLabel->setAnchorPoint(Vec2(0, 1));                      // 设置锚点为左上角
    playerNameLabel->setTextColor(cocos2d::Color4B(255, 255, 255, 255)); // 设置文本颜色为白色
    this->addChild(playerNameLabel);

    // 按钮间隔
    const float buttonSpacing = 100.0f;  // 按钮之间的间隔
    const float buttonStartY = screenSize.height / 2 + buttonSpacing;  // 第一个按钮的起始Y坐标

    // 创建并配置开始游戏按钮
    auto startGameButton = HoverButton::create("button2.png", "button2.png", "button2.png");
    startGameButton->setTitleText(u8"开始游戏");
    startGameButton->setScale(1.5f);
    startGameButton->setTitleFontSize(50);
    startGameButton->setPosition(Vec2(screenSize.width - 500, buttonStartY-200));
    startGameButton->addTouchEventListener([](Ref* sender, ui::Widget::TouchEventType type) {
        if (type == ui::Widget::TouchEventType::ENDED) {
            CCLOG("Start Game clicked!"); // 打印日志
            // 点击之后进行初始化EventSystem
            EventSystem::getInstance()->init();

            audioPlayer("ClickSoundEffect.mp3", false);  // 播放点击音效
            auto selectionScene = SelectionScene::createScene(); // 创建选择场景
            Director::getInstance()->replaceScene(TransitionFade::create(1.0f, selectionScene, Color3B::BLACK)); // 场景切换
        }
        });
    this->addChild(startGameButton);

    // 创建并配置游戏设置按钮
    auto settingsButton = HoverButton::create("button2.png", "button2.png", "button2.png");
    settingsButton->setTitleText(u8"游戏设置");
    settingsButton->setTitleFontSize(50);
    settingsButton->setScale(1.5f);
    settingsButton->setPosition(Vec2(screenSize.width - 500, buttonStartY - buttonSpacing-200));
    settingsButton->addTouchEventListener([](Ref* sender, ui::Widget::TouchEventType type) {
        if (type == ui::Widget::TouchEventType::ENDED) {
            CCLOG("Settings clicked!"); // 打印日志
            audioPlayer("ClickSoundEffect.mp3", false);  // 播放点击音效
            auto scene = SettingsScene::createScene();  // 创建设置场景
            Director::getInstance()->replaceScene(TransitionFade::create(1.0f, scene, Color3B::BLACK)); // 场景切换
        }
        });
    this->addChild(settingsButton);

    // 创建并配置退出游戏按钮
    auto exitGameButton = HoverButton::create("button2.png", "button2.png", "button2.png");
    exitGameButton->setTitleText(u8"退出游戏");
    exitGameButton->setTitleFontSize(50);
    exitGameButton->setScale(1.5f);
    exitGameButton->setPosition(Vec2(screenSize.width - 500, buttonStartY -200-buttonSpacing * 2));
    exitGameButton->addTouchEventListener([this](Ref* sender, ui::Widget::TouchEventType type) {
        if (type == ui::Widget::TouchEventType::ENDED) {
            audioPlayer("ClickSoundEffect.mp3", false);  // 播放点击音效

            // 创建黑屏效果
            auto blackScreen = LayerColor::create(Color4B(0, 0, 0, 255)); // 创建黑色覆盖层
            blackScreen->setOpacity(0);  // 设置为完全透明
            this->addChild(blackScreen, 100); // 添加黑屏层到当前场景

            // 执行渐变效果、停留和退出
            auto fadeIn = FadeIn::create(0.5f); // 渐变到黑屏
            auto delay = DelayTime::create(2.0f); // 停留2秒
            auto exitCallback = CallFunc::create([]() {
                Director::getInstance()->end(); // 退出游戏
                });

            // 顺序执行：渐变 -> 停留 -> 退出
            auto sequence = Sequence::create(fadeIn, delay, exitCallback, nullptr);
            blackScreen->runAction(sequence);  // 运行动作序列
        }
        });
    this->addChild(exitGameButton);

    return true; // 初始化成功，返回true
}

#include "ui/CocosGUI.h"
#include "const.h"
#include "AudioPlayer.h"
#include "StartScene.h"
#include "InitialScene.h"
#include "HelloWorldScene.h"

using namespace std;
using namespace cocos2d;

// 全局变量定义
int backgroundMusicId = -1;         ///< 背景音乐 ID
float backgroundVolumn = 0.5;      ///< 背景音乐音量
int soundEffectId =-1;             ///< 音效 ID
float soundEffectVolumn = 0.5;     ///< 音效音量

/**
 * @brief 创建 StartScene 场景。
 *
 * @return cocos2d::Scene* 返回 StartScene 的场景实例。
 */
Scene* StartScene::createScene() {
    auto scene = Scene::create();          // 创建一个空白场景
    auto layer = StartScene::create();     // 创建 StartScene 层
    scene->addChild(layer);                // 将层添加到场景
    return scene;
}

/**
 * @brief 初始化 StartScene 场景。
 *
 * 加载背景图片、播放背景音乐、设置加载进度条以及跳转到下一场景。
 *
 * @return bool 初始化是否成功。
 */
bool StartScene::init() {
    // 确保父类 Scene 初始化成功
    if (!Scene::init()) {
        return false;
    }

    // 播放背景音乐
    audioPlayer("../Resources/start.ogg", true);

    // 获取屏幕尺寸
    const auto screenSize = cocos2d::Director::getInstance()->getVisibleSize();

    // 加载背景图片
    const auto background0 = Sprite::create("title4.png");
    background0->setPosition(Vec2(screenSize.width / 2, screenSize.height / 2));
    this->addChild(background0);

    const auto background1 = Sprite::create("title5.png");
    background1->setPosition(Vec2(screenSize.width / 2, screenSize.height / 2));
    this->addChild(background1);

    const auto background2 = Sprite::create("scene0.jfif");

    // 计算缩放比例并调整背景大小
    float scaleX = screenSize.width / background2->getContentSize().width;
    float scaleY = screenSize.height / background2->getContentSize().height;
    float scale = std::min(scaleX, scaleY); // 确保背景适配屏幕
    background2->setScale(scale);
    background2->setPosition(Vec2(screenSize.width / 2, screenSize.height / 2 + 200));
    this->addChild(background2);

    // 创建加载进度条
    auto progressBar = cocos2d::ui::LoadingBar::create("StartupLoadingBar.png");
    progressBar->setPosition(Vec2(screenSize.width / 2, screenSize.height / 2 - 200));
    progressBar->setPercent(0); // 初始为 0%
    this->addChild(progressBar);

    // 创建加载进度文本
    auto progressLabel = Label::createWithTTF("0%", "../Resources/Fonts/arial.ttf", 50);
    progressLabel->setPosition(Vec2(progressBar->getPosition().x, progressBar->getPosition().y));
    progressLabel->setVisible(true);
    this->addChild(progressLabel);

    // 定义进度条更新间隔
    constexpr float interval = 2.0 / 100.0f; // 每更新 1% 所需时间

    // 更新进度条
    for (int i = 0; i <= 100; i++) {
        this->scheduleOnce([progressBar, progressLabel, i](float dt) {
            progressLabel->setVisible(i >= 10); // 10% 后显示进度文字
            progressBar->setPercent(i);         // 设置进度条百分比
            progressLabel->setString(std::to_string(i) + "%");

            // 动态调整进度文字的位置
            float percentage = progressBar->getPercent() / 100.0f;
            float xPosition = progressBar->getPosition().x - progressBar->getContentSize().width / 2
                + progressBar->getContentSize().width * percentage - 30;
            progressLabel->setPosition(Vec2(xPosition, progressBar->getPosition().y));

        
            }, interval * i, "UpdateStartupLoadingBar" + std::to_string(i));
    }

    // 跳转到下一场景
    this->scheduleOnce([](float dt) {
        auto nextScene = InitialScene::createScene(); // 创建目标场景
        Director::getInstance()->pushScene(TransitionFade::create(1.0f, nextScene)); // 使用 1 秒淡出效果
        }, interval * 200, "LoadNextScene");

    return true;
}

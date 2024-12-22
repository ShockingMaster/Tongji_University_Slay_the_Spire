#include "SettingsScene.h"
#include "HoverButton.h"
#include "MenuScene.h"
#include "AudioEngine.h"
#include "const.h"
#include "AudioPlayer.h"

// 命名空间
using namespace cocos2d;

int backgroundMusicId = 0;         ///< 背景音乐 ID
float backgroundVolumn = 0.3f;      ///< 背景音乐音量
int soundEffectId = 0;             ///< 音效 ID
float soundEffectVolumn = 0.3f;     ///< 音效音量


// 创建场景
Scene* SettingsScene::createScene()
{
    auto scene = Scene::create();
    auto layer = SettingsScene::create();
    scene->addChild(layer);
    return scene;
}


// 初始化场景
bool SettingsScene::init()
{
    // 创建场景
    if (!Scene::init()) {
        return false;
    }

    // 加载音乐
    audioPlayer("start.ogg", true);

    // 加载背景
    const auto screenSize = cocos2d::Director::getInstance()->getVisibleSize();
    const auto background = Sprite::create("scene2.jfif");
    background->setPosition(Vec2(screenSize.width / 2+50, screenSize.height / 2 + 10));
    this->addChild(background);

    // 创建说明文字
    auto bgMusicLabel = Label::createWithSystemFont(u8"背景音乐音量", "Arial", 50);
    bgMusicLabel->setPosition(Vec2(450, 540));  // 设置位置
    this->addChild(bgMusicLabel);

    auto soundEffectLabel = Label::createWithSystemFont(u8"音效音量", "Arial", 50);
    soundEffectLabel->setPosition(Vec2(450, 250));  // 设置位置
    this->addChild(soundEffectLabel);

    // 创建“0”和“100”的说明文字
    auto zeroLabel1 = Label::createWithSystemFont("0", "Arial", 50);
    zeroLabel1->setPosition(Vec2(200, 500));  // 设置位置
    this->addChild(zeroLabel1);

    auto hundredLabel1 = Label::createWithSystemFont("100", "Arial", 50);
    hundredLabel1->setPosition(Vec2(750, 500));  // 设置位置
    this->addChild(hundredLabel1);

    auto zeroLabel2 = Label::createWithSystemFont("0", "Arial", 50);
    zeroLabel2->setPosition(Vec2(200, 200));  // 设置位置
    this->addChild(zeroLabel2);

    auto hundredLabel2 = Label::createWithSystemFont("100", "Arial", 50);
    hundredLabel2->setPosition(Vec2(750, 200));  // 设置位置
    this->addChild(hundredLabel2);

    // 创建滑动条
    auto backgroundMusicVolumnSlider = cocos2d::ui::Slider::create();
    backgroundMusicVolumnSlider->loadBarTexture("SliderBackground.png");
    backgroundMusicVolumnSlider->loadSlidBallTextures("SliderNodeNormal.png", "SliderNodePress.png", "SliderNodeDisable.png");
    backgroundMusicVolumnSlider->loadProgressBarTexture("SliderBar.png");
    backgroundMusicVolumnSlider->setPosition(cocos2d::Vec2(450, 500));
    backgroundMusicVolumnSlider->setPercent(backgroundVolumn * 100);

    auto effectVolumnslider = cocos2d::ui::Slider::create();
    effectVolumnslider->loadBarTexture("SliderBackground.png");
    effectVolumnslider->loadSlidBallTextures("SliderNodeNormal.png", "SliderNodePress.png", "SliderNodeDisable.png");
    effectVolumnslider->loadProgressBarTexture("SliderBar.png");
    effectVolumnslider->setPosition(cocos2d::Vec2(450, 200));
    effectVolumnslider->setPercent(soundEffectVolumn * 100);

    // 为滑动条添加事件处理器
    backgroundMusicVolumnSlider->addEventListener([=](Ref* sender, cocos2d::ui::Slider::EventType type) {
        if (type == cocos2d::ui::Slider::EventType::ON_SLIDEBALL_DOWN) {
            // 加载点击音效
            audioPlayer("ClickSoundEffect.mp3", false);
        }
        if (type == cocos2d::ui::Slider::EventType::ON_PERCENTAGE_CHANGED) {
            const cocos2d::ui::Slider* slider = dynamic_cast<cocos2d::ui::Slider*>(sender);
            const float percent = slider->getPercent();
            backgroundVolumn = percent / 100.0f;
            // 设置背景音乐音量
            cocos2d::experimental::AudioEngine::setVolume(backgroundMusicId, backgroundVolumn);
            audioPlayer("start.ogg", true);
        }
        });

    effectVolumnslider->addEventListener([=](Ref* sender, cocos2d::ui::Slider::EventType type) {
        if (type == cocos2d::ui::Slider::EventType::ON_SLIDEBALL_DOWN) {
            // 加载点击音效
            audioPlayer("ClickSoundEffect.mp3", false);
        }
        if (type == cocos2d::ui::Slider::EventType::ON_PERCENTAGE_CHANGED) {
            const cocos2d::ui::Slider* slider = dynamic_cast<cocos2d::ui::Slider*>(sender);
            const float percent = slider->getPercent();
            soundEffectVolumn = percent / 100.0f;
            // 设置音效音量
            cocos2d::experimental::AudioEngine::setVolume(soundEffectId, soundEffectVolumn);
        }
        });

    // 将滑动条添加至场景
    this->addChild(backgroundMusicVolumnSlider);
    this->addChild(effectVolumnslider);

    // 创建返回按钮（HoverButton）
    auto backButton = HoverButton::create("button4(1).png", "button4(2).png", "button4(1).png");
    backButton->setPosition(Vec2(1250, 100));  // 设置按钮位置
    backButton->setTitleText(u8"返回");
    backButton->setTitleFontSize(50);
    backButton->addClickEventListener([=](Ref* sender) {
        // 按钮点击时返回 MenuScene
        Director::getInstance()->replaceScene(MenuScene::createScene());
        });

    this->addChild(backButton);

    return true;
}

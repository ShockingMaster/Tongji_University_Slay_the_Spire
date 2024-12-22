#include "SelectionScene.h"
#include "MenuScene.h"
#include "HoverButton.h"
#include "MapScene.h"
#include "enum.h"
#include "AudioPlayer.h"
#include <locale>
#include <codecvt>


using namespace std;
using namespace cocos2d;

Scene* SelectionScene::createScene()
{
    auto scene = Scene::create();
    auto layer = SelectionScene::create();
    scene->addChild(layer);
    return scene;
}
bool SelectionScene::init()
{
    if (!Scene::init()) {
        return false;
    }

    // 创建黑色背景
    auto background = LayerColor::create(Color4B(0, 0, 0, 255)); // 黑色背景
    this->addChild(background);

    // 创建四张图片的 Sprite
    vector<Sprite*> images;
    images.push_back(Sprite::create("circle1.png"));
    images.push_back(Sprite::create("circle2.png"));
    images.push_back(Sprite::create("circle3.png"));
    images.push_back(Sprite::create("circle4.png"));
    images.push_back(Sprite::create("circle5.png"));

    // 创建显示文本的Label
    auto label = Label::createWithSystemFont(u8"随机地图生成中...", "Arial", 60);
    label->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2,
        Director::getInstance()->getVisibleSize().height / 2 - 300));
    label->setTextColor(Color4B::WHITE); // 设置文字颜色为白色
    this->addChild(label);

    // 将每张图片设置为在屏幕中心位置
    for (auto& image : images) {
        image->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2,
            Director::getInstance()->getVisibleSize().height / 2-100));
        image->setVisible(false);  // 初始时不显示
        this->addChild(image);
    }

    // 显示第一张图片
    images[0]->setVisible(true);

    // 创建一个计时器，每隔一段时间切换图片
    int currentImageIndex = 0;
    schedule([=](float deltaTime) mutable {
        // 隐藏当前图片
        images[currentImageIndex]->setVisible(false);

        // 显示下一张图片
        currentImageIndex = (currentImageIndex + 1) % images.size();
        images[currentImageIndex]->setVisible(true);
        }, 0.5f, "image_switch_key");

    // 使用调度器延迟两秒后停止图片切换
    scheduleOnce([=](float deltaTime) {
        unschedule("image_switch_key");  // 停止计时器
        }, 2.0f, "stop_image_switch");

    // 延迟2秒后开始淡出
    auto fadeOut = FadeOut::create(2.0f);
    auto delay = DelayTime::create(2.0f);
    auto sequence = Sequence::create(delay, fadeOut, RemoveSelf::create(), nullptr); // 延迟+淡出+移除
    label->runAction(sequence);  // 为label应用动作
    background->runAction(Sequence::create(delay, fadeOut->clone(), nullptr));

    // 确保在淡出动作后再切换场景
    scheduleOnce([=](float dt) {
        auto nextScene = MapScene::createScene();
        Director::getInstance()->pushScene(TransitionFade::create(1.0f, nextScene));  // 使用1秒的淡入过渡效果
        }, 2.0f, "LoadNextScene");

    return true;
}

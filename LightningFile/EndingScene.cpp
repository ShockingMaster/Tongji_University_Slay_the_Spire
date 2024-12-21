#include "cocos2d.h"
#include "EndingScene.h"
#include "AudioPlayer.h"
#include "const.h"
using namespace std;
using namespace cocos2d;

// 创建静态函数，手动实现 create() 方法
EndingScene* EndingScene::create(bool isVictory)
{
    EndingScene* ret = new EndingScene();
    if (ret && ret->init(isVictory)) {
        ret->autorelease();  // 确保对象正确管理
        return ret;
    }
    else {
        delete ret;  // 如果初始化失败，则删除对象
        return nullptr;
    }
}

// 初始化函数，根据胜利或失败显示不同的文本
bool EndingScene::init(bool isVictory) {
    // 播放背景音乐
    audioPlayer("Victory.ogg", true);

    // 创建背景图片
    auto backgroundSprite = Sprite::create("ending.jpg");  // 确保背景图片路径正确
    backgroundSprite->setPosition(Director::getInstance()->getVisibleSize() / 2);  // 设置背景位置为屏幕中央
    this->addChild(backgroundSprite);  // 将背景添加到层中

    // 创建文本标签
    auto label = Label::createWithSystemFont("", "Arial", 48);  // 使用较大字体
    label->setPosition(Director::getInstance()->getVisibleSize() / 2);
    label->setColor(Color3B::WHITE);  // 设置字体颜色为白色

    // 根据胜利或失败显示不同的文本
    if (isVictory) {
        label->setString(u8"恭喜，游戏胜利");
    }
    else {
        label->setString(u8"游戏失败");
    }
    this->addChild(label);  // 将文本添加到层中

    // 延时3秒后切换场景
    this->runAction(Sequence::create(
        DelayTime::create(3.0f),        // 延时3秒
        CallFunc::create([=]() {
            Director::getInstance()->popToRootScene();  // 返回到根场景
            }),
        nullptr));

    return true;
}

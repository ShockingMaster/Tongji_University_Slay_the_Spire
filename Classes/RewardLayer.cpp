#include "RewardLayer.h"
#include "cocos2d.h"
#include "Card.h"
#include "Relic.h"
#include "Potion.h"
#include "HeaderBar.h"
#include "HoverButton.h"
USING_NS_CC;

RewardLayer::RewardLayer()
{
}

RewardLayer::~RewardLayer()
{
}

RewardLayer* RewardLayer::create()
{
    RewardLayer* ret = new RewardLayer();
    if (ret && ret->init())
    {
        ret->autorelease();
        return ret;
    }
    else
    {
        delete ret;
        return nullptr;
    }
}

bool RewardLayer::init()
{
    // 判断是否成功初始化
    if (!Layer::init())
    {
        return false;
    }

    // 设置背景
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // 背景图像
    auto background = Sprite::create("rewardScreenSheet.png");
    background->setScale(1.2f);
    background->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 + 300));
    this->addChild(background);
   
    auto background1 = Sprite::create("rewardListItemPanel.png");
    background1->setScale(1.2f);
    background1->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 + 500));
    this->addChild(background1);

    auto background4 = Sprite::create("gold.png");
    background4->setScale(2.0f);
    background4->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 + 500));
    this->addChild(background4, 2000);

    auto background2 = Sprite::create("rewardListItemPanel.png");
    background2->setScale(1.2f);
    background2->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 + 300));
    this->addChild(background2);

    auto background5 = Sprite::create("bossCardReward.png");
    background5->setScale(2.0f);
    background5->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 + 300));
    this->addChild(background5, 2000);

    auto background3 = Sprite::create("rewardListItemPanel.png");
    background3->setScale(1.2f);
    background3->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 + 100));
    this->addChild(background3);

    auto background6 = Sprite::create("relic_link.png");
    background6->setScale(2.0f);
    background6->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 + 100));
    this->addChild(background6, 2000);

    auto label = cocos2d::Label::createWithSystemFont(u8" 双击接收宝藏", "Arial", 60);
    label->setPosition(1000,300);  // 设置标签位置
    label->setTextColor(cocos2d::Color4B::WHITE);  // 设置字体颜色为白色
    this->addChild(label,1000);
    // 创建关闭按钮
    auto closeButton = MenuItemImage::create(
        "takeAll.png",         // 正常状态
        "takeAllUsed.png",     // 被点击状态
        CC_CALLBACK_1(RewardLayer::onCloseButtonClicked, this));  // 按钮点击回调

    // 确保按钮在屏幕可见区域内，并设置正确的位置
    closeButton->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 -200));

    // 创建菜单并添加按钮
    auto menu = Menu::create(closeButton, NULL);
    menu->setPosition(Vec2::ZERO);  // 设置菜单的位置为 (0, 0)

    // 将菜单添加到层
    this->addChild(menu); // 注意这里的1，确保它在其他UI元素上方
    // 添加触摸事件监听器
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(RewardLayer::onTouchBegan, this);
    listener->setSwallowTouches(true);  // 确保不将事件传递给其他层
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    return true;
}




void RewardLayer::onCloseButtonClicked(Ref* sender)
{
    this->removeFromParentAndCleanup(true);
}

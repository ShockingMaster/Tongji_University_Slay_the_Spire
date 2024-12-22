#include "ShowLayer.h" 
#include "ui/CocosGUI.h"  
#include "Card.h"
#include "Headerbar.h"
#include "RandomGenerator.h"
#include "SpriteGenerator.h"
#include "Player.h"
#include "AudioPlayer.h"
#include "cocos2d.h"
using namespace cocos2d;
ShowLayer::ShowLayer() : _background(nullptr) {
}

ShowLayer::~ShowLayer() {
}

ShowLayer* ShowLayer::create(int op) {
    ShowLayer* ret = new ShowLayer();
    if (ret && ret->init(op)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}


bool ShowLayer::init(int op) {
    if (!Layer::init()) {
        return false;
    }
    auto headbar = HeaderBar::create(EventSystem::getInstance());
    headbar->setPosition(Vec2(0, 750));          // 设置位置（在屏幕上部）
    this->addChild(headbar);
    headbar->setLocalZOrder(1000);
    operation = op;
    // 加载背景
    _background = cocos2d::Sprite::create("panel.png");
    if (_background) {
        _background->setScale(0.8f);
        _background->setColor(cocos2d::Color3B(240, 240, 240));
        _background->setPosition(cocos2d::Vec2(1000, 420));
        _background->setAnchorPoint(cocos2d::Vec2(0.5, 0.5));
        this->addChild(_background, 200);
    }

    // 创建关闭按钮
    auto closeButton = cocos2d::MenuItemImage::create(
        "cancelButton.png", "cancelButtonOutline.png", CC_CALLBACK_1(ShowLayer::closeLayerCallback, this)
    );
    closeButton->setPosition(cocos2d::Vec2(1940, 50));
    closeButton->setScale(1.3f);
    auto menu = cocos2d::Menu::create(closeButton, nullptr);
    menu->setPosition(cocos2d::Vec2::ZERO);
    this->addChild(menu, 102);  // 按钮的层级高于卡牌和背景
    // 创建返回Label
    auto returnLabel = cocos2d::Label::createWithSystemFont(u8"返回", "Arial", 60);
    returnLabel->setTextColor(cocos2d::Color4B::WHITE);
    returnLabel->setPosition(cocos2d::Vec2(1790, 115));  // 设置Label的位置，使其在按钮上方
    this->addChild(returnLabel, 6000);  // Label的层级高于按钮
    if (op == 1) {
        // 创建返回Label
        auto background4 = Sprite::create("gold.png");
        background4->setScale(6.0f);
        background4->setPosition(Vec2(1000, 400));
        this->addChild(background4, 2000);
        // 生成随机数
        int random_number = RandomGenerator::getInstance()->getRandomNumber(10, 20);
        auto Label1 = cocos2d::Label::createWithSystemFont(u8"恭喜获得金币" + to_string(random_number) + u8"枚", "Arial", 60);
        Label1->setTextColor(cocos2d::Color4B::WHITE);
        Label1->setPosition(cocos2d::Vec2(1000, 60));  // 设置Label的位置，使其在按钮上方
        this->addChild(Label1, 103);  // Label的层级高于按钮
        audioPlayer("gold.ogg", false);
        EventSystem::getInstance()->changeCoins(random_number);
        headbar->updateHeader(EventSystem::getInstance());
    }
    if (op == 2) {
        // 创建返回Label
        auto randomCard = RandomGenerator::getInstance()->getRandomCard();
        auto background4 = CardSpriteGenerator::createCardSprite(randomCard);
        background4->setScale(1.2f);
        background4->setPosition(Vec2(1000, 400));
        this->addChild(background4, 2000);
        auto Label1 = cocos2d::Label::createWithSystemFont(u8"恭喜获得新卡牌", "Arial", 60);
        Label1->setTextColor(cocos2d::Color4B::WHITE);
        Label1->setPosition(cocos2d::Vec2(1000, 60));  // 设置Label的位置，使其在按钮上方
        this->addChild(Label1, 103);  // Label的层级高于按钮
        audioPlayer("gold.ogg", false);
        EventSystem::getInstance()->cards_.push_back(NULL);//请调用Eventsystem进行卡牌增加
        headbar->updateHeader(EventSystem::getInstance());
    }
    if (op == 3) {
        
        // 生成随机数
        auto randomRelic = RandomGenerator::getInstance()->getRandomRelic();
        auto Label1 = cocos2d::Label::createWithSystemFont(u8"恭喜获得新遗物", "Arial", 60);
        // 创建返回Label
        auto background4 = RelicSpriteGenerator::createCardSprite(randomRelic);
        background4->setScale(3.0f);
        background4->setPosition(Vec2(1000, 400));
        this->addChild(background4, 2000);
        Label1->setTextColor(cocos2d::Color4B::WHITE);
        Label1->setPosition(cocos2d::Vec2(1000, 60));  // 设置Label的位置，使其在按钮上方
        this->addChild(Label1, 103);  // Label的层级高于按钮
        audioPlayer("gold.ogg", false);
        EventSystem::getInstance()->relics_.push_back(randomRelic);
        headbar->updateHeader(EventSystem::getInstance());
    }
    if (op == 4) {

        // 创建三张卡片
        auto randomCard1 = RandomGenerator::getInstance()->getRandomCard();
        auto card1 = CardSpriteGenerator::createCardSprite(randomCard1);
        card1->setScale(1.2f);
        card1->setPosition(Vec2(700, 400));
        this->addChild(card1, 2000);

        auto randomCard2 = RandomGenerator::getInstance()->getRandomCard();
        auto card2 = CardSpriteGenerator::createCardSprite(randomCard2);
        card2->setScale(1.2f);
        card2->setPosition(Vec2(1000, 400));
        this->addChild(card2, 2000);

        auto randomCard3 = RandomGenerator::getInstance()->getRandomCard();
        auto card3 = CardSpriteGenerator::createCardSprite(randomCard3);
        card3->setScale(1.2f);
        card3->setPosition(Vec2(1300, 400));
        this->addChild(card3, 2000);

        // 显示提示信息
        auto Label1 = cocos2d::Label::createWithSystemFont(u8"请选择一张想要的牌", "Arial", 60);
        Label1->setTextColor(cocos2d::Color4B::WHITE);
        Label1->setPosition(cocos2d::Vec2(1000, 60));  // 设置Label的位置
        this->addChild(Label1, 103);  // Label的层级高于卡片

        // 创建触摸事件监听器
        auto listener = EventListenerTouchOneByOne::create();

        // 设置触摸优先级
        listener->setSwallowTouches(true);  // 吞噬事件，防止其他层接收触摸事件

        // 触摸开始事件：判断卡片是否被点击并执行操作
        listener->onTouchBegan = [this, card1, card2, card3, randomCard1, randomCard2, randomCard3, headbar, Label1](Touch* touch, Event* event) {
            Vec2 touchPos = touch->getLocation();

            if (card1->getBoundingBox().containsPoint(touchPos)) {
                // 执行选中的卡片的操作
                audioPlayer("gold.ogg", false);
                Label1->setString(u8"选择成功，请点击返回");
                EventSystem::getInstance()->addCard(randomCard1);
                headbar->updateHeader(EventSystem::getInstance());
                return true;  // 返回true表示触摸事件已处理
            }
            else if (card2->getBoundingBox().containsPoint(touchPos)) {
                audioPlayer("gold.ogg", false);
                Label1->setString(u8"选择成功，请点击返回");
                EventSystem::getInstance()->addCard(randomCard2);
                headbar->updateHeader(EventSystem::getInstance());
                return true;
            }
            else if (card3->getBoundingBox().containsPoint(touchPos)) {
                audioPlayer("gold.ogg", false);
                Label1->setString(u8"选择成功，请点击返回");
                EventSystem::getInstance()->addCard(randomCard3);
                headbar->updateHeader(EventSystem::getInstance());
                return true;
            }

            return false;  // 返回false表示事件未处理，继续传递
            };

        // 添加触摸事件监听器
        _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    }


    if (op == 5) {

        // 生成随机数
        auto randomPotion = RandomGenerator::getInstance()->getRandomPotion();
        auto background4 = PotionSpriteGenerator::createCardSprite(randomPotion);
        background4->setScale(3.0f);
        background4->setPosition(Vec2(1000, 400));
        this->addChild(background4, 2000);
        auto Label1 = cocos2d::Label::createWithSystemFont(u8"恭喜获得新药水", "Arial", 60);
        Label1->setTextColor(cocos2d::Color4B::WHITE);
        Label1->setPosition(cocos2d::Vec2(1000, 60));  // 设置Label的位置，使其在按钮上方
        this->addChild(Label1, 103);  // Label的层级高于按钮
        audioPlayer("gold.ogg", false);
        EventSystem::getInstance()->addPotion(randomPotion);
        headbar->updateHeader(EventSystem::getInstance());
    }
    return true;
}




void ShowLayer::closeLayerCallback(cocos2d::Ref* sender) {
    this->removeFromParentAndCleanup(true);
}
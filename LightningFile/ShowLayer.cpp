#include "ShowLayer.h" 
#include "ui/CocosGUI.h"  
#include "Card.h"
#include "Headerbar.h"
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


bool ShowLayer::init( int op) {
    if (!Layer::init()) {
        return false;
    } 
    auto headbar = HeaderBar::create(Player::getInstance());
    headbar->setPosition(Vec2(0, 750));          // 设置位置（在屏幕上部）
    this->addChild(headbar);
    headbar->setLocalZOrder(100);
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
        "cancelButton.png", "cancelButtonOutline.png",CC_CALLBACK_1(ShowLayer::closeLayerCallback, this)
    );
    closeButton->setPosition(cocos2d::Vec2(1940, 50));
    closeButton->setScale(1.3f);
    auto menu = cocos2d::Menu::create(closeButton, nullptr);
    menu->setPosition(cocos2d::Vec2::ZERO);
    this->addChild(menu, 102);  // 按钮的层级高于卡牌和背景
    // 创建返回Label
    auto returnLabel = cocos2d::Label::createWithSystemFont(u8"返回", "Arial", 60);
    returnLabel->setTextColor(cocos2d::Color4B::WHITE);
    returnLabel->setPosition(cocos2d::Vec2(1790, -40));  // 设置Label的位置，使其在按钮上方
    this->addChild(returnLabel, 103);  // Label的层级高于按钮
    if (op == 1) {
        // 创建返回Label
        std::random_device rd;  // 随机数种子
        std::mt19937 gen(rd()); // Mersenne Twister 随机数生成器
        std::uniform_int_distribution<> distrib(100, 200); 
        auto background4 = Sprite::create("gold.png");
        background4->setScale(6.0f);
        background4->setPosition(Vec2(1000,400));
        this->addChild(background4, 2000);
        // 生成随机数
        int random_number = distrib(gen);
        auto Label1 = cocos2d::Label::createWithSystemFont(u8"恭喜获得金币"+to_string(random_number)+u8"枚", "Arial", 60);
        Label1->setTextColor(cocos2d::Color4B::WHITE);
        Label1->setPosition(cocos2d::Vec2(1000, 60));  // 设置Label的位置，使其在按钮上方
        this->addChild(Label1, 103);  // Label的层级高于按钮
        audioPlayer("gold.ogg", false);
        Player::getInstance()->coins_ += random_number;
        headbar->updateHeader(Player::getInstance());
    }
    if (op == 2) {
        // 创建返回Label
        std::random_device rd;  // 随机数种子
        std::mt19937 gen(rd()); // Mersenne Twister 随机数生成器
        std::uniform_int_distribution<> distrib(100, 200);
        auto background4 = Sprite::create("hitcard.png");
        background4->setScale(1.0f);
        background4->setPosition(Vec2(1000, 400));
        this->addChild(background4, 2000);
        // 生成随机数
        int random_number = distrib(gen);
        auto Label1 = cocos2d::Label::createWithSystemFont(u8"恭喜获得新卡牌", "Arial", 60);
        Label1->setTextColor(cocos2d::Color4B::WHITE);
        Label1->setPosition(cocos2d::Vec2(1000, 60));  // 设置Label的位置，使其在按钮上方
        this->addChild(Label1, 103);  // Label的层级高于按钮
        audioPlayer("gold.ogg", false);
        Player::getInstance()->cards_.push_back(NULL);
        headbar->updateHeader(Player::getInstance());
    }
    if (op == 3) {
        // 创建返回Label
        std::random_device rd;  // 随机数种子
        std::mt19937 gen(rd()); // Mersenne Twister 随机数生成器
        std::uniform_int_distribution<> distrib(100, 200);
        auto background4 = Sprite::create("bell.png");
        background4->setScale(4.0f);
        background4->setPosition(Vec2(1000, 400));
        this->addChild(background4, 2000);
        // 生成随机数
        int random_number = distrib(gen);
        auto Label1 = cocos2d::Label::createWithSystemFont(u8"恭喜获得新遗物", "Arial", 60);
        Label1->setTextColor(cocos2d::Color4B::WHITE);
        Label1->setPosition(cocos2d::Vec2(1000, 60));  // 设置Label的位置，使其在按钮上方
        this->addChild(Label1, 103);  // Label的层级高于按钮
        audioPlayer("gold.ogg", false);
        Player::getInstance()->relics_.push_back(NULL);
        headbar->updateHeader(Player::getInstance());
    }
    return true;
}




void ShowLayer::closeLayerCallback(cocos2d::Ref* sender) {
    this->removeFromParentAndCleanup(true);
}

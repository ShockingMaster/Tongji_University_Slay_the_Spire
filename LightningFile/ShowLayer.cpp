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
    headbar->setPosition(Vec2(0, 750));          // ����λ�ã�����Ļ�ϲ���
    this->addChild(headbar);
    headbar->setLocalZOrder(100);
    operation = op;
    // ���ر���
    _background = cocos2d::Sprite::create("panel.png");
    if (_background) {
        _background->setScale(0.8f);
        _background->setColor(cocos2d::Color3B(240, 240, 240));
        _background->setPosition(cocos2d::Vec2(1000, 420));
        _background->setAnchorPoint(cocos2d::Vec2(0.5, 0.5));
        this->addChild(_background, 200);
    }

    // �����رհ�ť
    auto closeButton = cocos2d::MenuItemImage::create(
        "cancelButton.png", "cancelButtonOutline.png",CC_CALLBACK_1(ShowLayer::closeLayerCallback, this)
    );
    closeButton->setPosition(cocos2d::Vec2(1940, 50));
    closeButton->setScale(1.3f);
    auto menu = cocos2d::Menu::create(closeButton, nullptr);
    menu->setPosition(cocos2d::Vec2::ZERO);
    this->addChild(menu, 102);  // ��ť�Ĳ㼶���ڿ��ƺͱ���
    // ��������Label
    auto returnLabel = cocos2d::Label::createWithSystemFont(u8"����", "Arial", 60);
    returnLabel->setTextColor(cocos2d::Color4B::WHITE);
    returnLabel->setPosition(cocos2d::Vec2(1790, -40));  // ����Label��λ�ã�ʹ���ڰ�ť�Ϸ�
    this->addChild(returnLabel, 103);  // Label�Ĳ㼶���ڰ�ť
    if (op == 1) {
        // ��������Label
        std::random_device rd;  // ���������
        std::mt19937 gen(rd()); // Mersenne Twister �����������
        std::uniform_int_distribution<> distrib(100, 200); 
        auto background4 = Sprite::create("gold.png");
        background4->setScale(6.0f);
        background4->setPosition(Vec2(1000,400));
        this->addChild(background4, 2000);
        // ���������
        int random_number = distrib(gen);
        auto Label1 = cocos2d::Label::createWithSystemFont(u8"��ϲ��ý��"+to_string(random_number)+u8"ö", "Arial", 60);
        Label1->setTextColor(cocos2d::Color4B::WHITE);
        Label1->setPosition(cocos2d::Vec2(1000, 60));  // ����Label��λ�ã�ʹ���ڰ�ť�Ϸ�
        this->addChild(Label1, 103);  // Label�Ĳ㼶���ڰ�ť
        audioPlayer("gold.ogg", false);
        Player::getInstance()->coins_ += random_number;
        headbar->updateHeader(Player::getInstance());
    }
    if (op == 2) {
        // ��������Label
        std::random_device rd;  // ���������
        std::mt19937 gen(rd()); // Mersenne Twister �����������
        std::uniform_int_distribution<> distrib(100, 200);
        auto background4 = Sprite::create("hitcard.png");
        background4->setScale(1.0f);
        background4->setPosition(Vec2(1000, 400));
        this->addChild(background4, 2000);
        // ���������
        int random_number = distrib(gen);
        auto Label1 = cocos2d::Label::createWithSystemFont(u8"��ϲ����¿���", "Arial", 60);
        Label1->setTextColor(cocos2d::Color4B::WHITE);
        Label1->setPosition(cocos2d::Vec2(1000, 60));  // ����Label��λ�ã�ʹ���ڰ�ť�Ϸ�
        this->addChild(Label1, 103);  // Label�Ĳ㼶���ڰ�ť
        audioPlayer("gold.ogg", false);
        Player::getInstance()->cards_.push_back(NULL);
        headbar->updateHeader(Player::getInstance());
    }
    if (op == 3) {
        // ��������Label
        std::random_device rd;  // ���������
        std::mt19937 gen(rd()); // Mersenne Twister �����������
        std::uniform_int_distribution<> distrib(100, 200);
        auto background4 = Sprite::create("bell.png");
        background4->setScale(4.0f);
        background4->setPosition(Vec2(1000, 400));
        this->addChild(background4, 2000);
        // ���������
        int random_number = distrib(gen);
        auto Label1 = cocos2d::Label::createWithSystemFont(u8"��ϲ���������", "Arial", 60);
        Label1->setTextColor(cocos2d::Color4B::WHITE);
        Label1->setPosition(cocos2d::Vec2(1000, 60));  // ����Label��λ�ã�ʹ���ڰ�ť�Ϸ�
        this->addChild(Label1, 103);  // Label�Ĳ㼶���ڰ�ť
        audioPlayer("gold.ogg", false);
        Player::getInstance()->relics_.push_back(NULL);
        headbar->updateHeader(Player::getInstance());
    }
    return true;
}




void ShowLayer::closeLayerCallback(cocos2d::Ref* sender) {
    this->removeFromParentAndCleanup(true);
}

#include "CardLayer.h"
#include "ui/CocosGUI.h"  
#include "Card.h"

CardLayer::CardLayer() : _cards({}), _scrollView(nullptr), _background(nullptr) {
}

CardLayer::~CardLayer() {
}

CardLayer* CardLayer::create(std::vector<Card*> cards) {
    CardLayer* ret = new CardLayer();
    if (ret && ret->init(cards)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool CardLayer::init(std::vector<Card*> cards) {
    if (!Layer::init()) {
        return false;
    }

    // ���ر���
    _background = cocos2d::Sprite::create("map2.png");
    if (_background) {
        _background->setPosition(cocos2d::Vec2(1000, 800));
        _background->setAnchorPoint(cocos2d::Vec2(0.5, 0.5));
        this->addChild(_background, 100);  // ����ͼ������ײ�
    }

    // ��ʼ������
    _cards = cards;

    // ���� ScrollView
    _scrollView = cocos2d::ui::ScrollView::create();
    _scrollView->setDirection(cocos2d::ui::ScrollView::Direction::VERTICAL);  // ����Ϊ�������
    _scrollView->setContentSize(cocos2d::Size(640, 480));  // ���ù��������С
    _scrollView->setAnchorPoint(cocos2d::Vec2(0.5, 0.5));
    _scrollView->setPosition(cocos2d::Vec2(320, 240));  // ������ͼ��λ��
    this->addChild(_scrollView, 101);  // ���ƹ�������Ĳ㼶���ڱ���

    displayCards();  // ��ʾ����

    // �����رհ�ť
    auto closeButton = cocos2d::MenuItemImage::create(
        "cancelButton.png", "cancelButtonOutline.png",
        CC_CALLBACK_1(CardLayer::closeLayerCallback, this)
    );
    closeButton->setPosition(cocos2d::Vec2(1600, 300));

    auto menu = cocos2d::Menu::create(closeButton, nullptr);
    menu->setPosition(cocos2d::Vec2::ZERO);
    this->addChild(menu, 102);  // ��ť�Ĳ㼶���ڿ��ƺͱ���

    return true;
}

void CardLayer::displayCards() {
    float startX = 0.0f;
    float startY = 100.0f;
    float offsetY = 120.0f;

    // ������������
    cocos2d::Node* cardContainer = cocos2d::Node::create();
    _scrollView->addChild(cardContainer);  // ������������ӵ�������ͼ��

    for (size_t i = 0; i < _cards.size(); ++i) {
        auto card = _cards[i];
        auto label = cocos2d::Label::createWithSystemFont(card->name_, "Arial", 24);
        label->setPosition(startX, startY - i * offsetY);
        cardContainer->addChild(label);
    }

    // ���ÿ��������ĸ߶ȣ�ȷ�����Թ���
    float containerHeight = _cards.size() * offsetY;
    cardContainer->setContentSize(cocos2d::Size(640, containerHeight));
}

void CardLayer::closeLayerCallback(cocos2d::Ref* sender) {
    this->removeFromParentAndCleanup(true);
}

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

    // 加载背景
    _background = cocos2d::Sprite::create("map2.png");
    if (_background) {
        _background->setPosition(cocos2d::Vec2(1000, 800));
        _background->setAnchorPoint(cocos2d::Vec2(0.5, 0.5));
        this->addChild(_background, 100);  // 背景图层在最底层
    }

    // 初始化卡牌
    _cards = cards;

    // 创建 ScrollView
    _scrollView = cocos2d::ui::ScrollView::create();
    _scrollView->setDirection(cocos2d::ui::ScrollView::Direction::VERTICAL);  // 设置为纵向滚动
    _scrollView->setContentSize(cocos2d::Size(640, 480));  // 设置滚动区域大小
    _scrollView->setAnchorPoint(cocos2d::Vec2(0.5, 0.5));
    _scrollView->setPosition(cocos2d::Vec2(320, 240));  // 滚动视图的位置
    this->addChild(_scrollView, 101);  // 卡牌滚动区域的层级高于背景

    displayCards();  // 显示卡牌

    // 创建关闭按钮
    auto closeButton = cocos2d::MenuItemImage::create(
        "cancelButton.png", "cancelButtonOutline.png",
        CC_CALLBACK_1(CardLayer::closeLayerCallback, this)
    );
    closeButton->setPosition(cocos2d::Vec2(1600, 300));

    auto menu = cocos2d::Menu::create(closeButton, nullptr);
    menu->setPosition(cocos2d::Vec2::ZERO);
    this->addChild(menu, 102);  // 按钮的层级高于卡牌和背景

    return true;
}

void CardLayer::displayCards() {
    float startX = 0.0f;
    float startY = 100.0f;
    float offsetY = 120.0f;

    // 创建卡牌容器
    cocos2d::Node* cardContainer = cocos2d::Node::create();
    _scrollView->addChild(cardContainer);  // 将卡牌容器添加到滚动视图中

    for (size_t i = 0; i < _cards.size(); ++i) {
        auto card = _cards[i];
        auto label = cocos2d::Label::createWithSystemFont(card->name_, "Arial", 24);
        label->setPosition(startX, startY - i * offsetY);
        cardContainer->addChild(label);
    }

    // 设置卡牌容器的高度，确保可以滚动
    float containerHeight = _cards.size() * offsetY;
    cardContainer->setContentSize(cocos2d::Size(640, containerHeight));
}

void CardLayer::closeLayerCallback(cocos2d::Ref* sender) {
    this->removeFromParentAndCleanup(true);
}

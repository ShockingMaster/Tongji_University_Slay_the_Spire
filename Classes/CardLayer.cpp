#include "CardLayer.h" 
#include "ui/CocosGUI.h"  
#include "Card.h"

CardLayer::CardLayer() : _cards({}), _scrollView(nullptr), _background(nullptr) {
}

CardLayer::~CardLayer() {
}

CardLayer* CardLayer::create(std::vector<Card*> cards,int op) {
    CardLayer* ret = new CardLayer();
    if (ret && ret->init(cards,op)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool CardLayer::init(std::vector<Card*> cards, int op) {
    if (!Layer::init()) {
        return false;
    }
    operation = op;
    // 加载背景
    _background = cocos2d::Sprite::create("cardlayer.png");
    if (_background) {
        _background->setScale(1.0f);
        _background->setColor(cocos2d::Color3B(240, 240, 240));
        _background->setPosition(cocos2d::Vec2(1000, 720));
        _background->setAnchorPoint(cocos2d::Vec2(0.5, 0.5));
        this->addChild(_background, 100);
    }

    // 初始化卡牌
    _cards = cards;

    // 创建 ScrollView
    _scrollView = cocos2d::ui::ScrollView::create();
    _scrollView->setDirection(cocos2d::ui::ScrollView::Direction::VERTICAL);  // 设置为纵向滚动
    _scrollView->setContentSize(cocos2d::Size(2000, 1000));  // 设置滚动区域大小
    _scrollView->setAnchorPoint(cocos2d::Vec2(0.5, 0.5));
    _scrollView->setPosition(cocos2d::Vec2(320, 240));  // 滚动视图的位置
    this->addChild(_scrollView, 101);  // 卡牌滚动区域的层级高于背景

    displayCards();  // 显示卡牌

    // 创建关闭按钮
    auto closeButton = cocos2d::MenuItemImage::create(
        "cancelButton.png", "cancelButtonOutline.png",
        CC_CALLBACK_1(CardLayer::closeLayerCallback, this)
    );
    closeButton->setPosition(cocos2d::Vec2(1940, 190));
    closeButton->setScale(1.3f);
    auto menu = cocos2d::Menu::create(closeButton, nullptr);
    menu->setPosition(cocos2d::Vec2::ZERO);
    this->addChild(menu, 102);  // 按钮的层级高于卡牌和背景
    // 创建返回Label
    auto returnLabel = cocos2d::Label::createWithSystemFont(u8"返回", "Arial", 60);
    returnLabel->setTextColor(cocos2d::Color4B::WHITE);
    returnLabel->setPosition(cocos2d::Vec2(1750, 260));  // 设置Label的位置，使其在按钮上方
    this->addChild(returnLabel, 103);  // Label的层级高于按钮

    if (operation == 1) {
        auto inform = cocos2d::Label::createWithSystemFont(u8"当前\n牌组", "Marker Felt.ttf", 80);
        inform->setTextColor(cocos2d::Color4B::WHITE);
        inform->setPosition(cocos2d::Vec2(1750, 700));  // 设置Label的位置，使其在按钮上方
        this->addChild(inform, 103);  // Label的层级高于按钮
    }
    if (operation == 2) {
        auto inform = cocos2d::Label::createWithSystemFont(u8"请选择\n一张牌\n被删除", "Marker Felt.ttf", 80);
        inform->setTextColor(cocos2d::Color4B::WHITE);
        inform->setPosition(cocos2d::Vec2(1700, 700));  // 设置Label的位置，使其在按钮上方
        this->addChild(inform, 103);  // Label的层级高于按钮
    }
    if (operation == 3) {
        auto inform = cocos2d::Label::createWithSystemFont(u8"请选择\n一张牌\n被升级", "Marker Felt.ttf", 80);
        inform->setTextColor(cocos2d::Color4B::WHITE);
        inform->setPosition(cocos2d::Vec2(1700, 700));  // 设置Label的位置，使其在按钮上方
        this->addChild(inform, 103);  // Label的层级高于按钮
    }
    return true;
}
void CardLayer::displayCards() {
    float startX = 400;
    float startY = 1000;
    size_t cardCount = _cards.size();

    // 创建卡牌容器
    cocos2d::Node* cardContainer = cocos2d::Node::create();
    _scrollView->addChild(cardContainer, 200);  // 将卡牌容器添加到滚动视图上

    // 遍历卡牌并添加到容器中
    for (size_t i = 0; i < cardCount; ++i) {
        auto card = _cards[i];
        auto cardSprite = cocos2d::Sprite::create("cardbackground.jpg");

        if (i % 4 == 0 && i != 0) {
            startY -= 400;
        }

        cardSprite->setPosition(startX + 350 * (i % 4), startY);
        cardSprite->setScale(0.4f);
        cardContainer->addChild(cardSprite, 200);  // 初始设置

        // 将每个卡牌的精灵添加到容器中，并存储卡牌的指针以便之后使用
        _cardSprites.push_back(cardSprite);

        // 创建鼠标事件监听器
        auto cardListener = cocos2d::EventListenerMouse::create();

        // 鼠标移动事件
        cardListener->onMouseMove = [cardSprite, cardContainer](cocos2d::Event* event) {
            cocos2d::EventMouse* mouseEvent = dynamic_cast<cocos2d::EventMouse*>(event);
            if (mouseEvent) {
                cocos2d::Vec2 mousePosition = mouseEvent->getLocation();

                // 计算鼠标相对卡牌容器的坐标
                cocos2d::Vec2 containerPos = cardContainer->getPosition();
                mousePosition -= containerPos;

                // 扩大检测范围：通过扩大bounding box来扩大卡牌触发区域
                cocos2d::Rect extendedBoundingBox = cardSprite->getBoundingBox();
                extendedBoundingBox.origin.x -= 20; // 向左扩展20像素
                extendedBoundingBox.origin.y -= 20; // 向下扩展20像素
                extendedBoundingBox.size.width += 40; // 扩展宽度40像素
                extendedBoundingBox.size.height += 40; // 扩展高度40像素

                // 判断鼠标是否在卡牌的扩展范围内
                if (extendedBoundingBox.containsPoint(mousePosition)) {
                    // 放大卡牌
                    cardSprite->setScale(0.5f);
                    cardSprite->setLocalZOrder(300);  // 提高 zOrder，确保它显示在上层
                }
                else {
                    // 恢复原尺寸
                    cardSprite->setScale(0.4f);  // 恢复到原尺寸
                    cardSprite->setLocalZOrder(200);  // 恢复初始 zOrder
                }
            }
            };

        // 为每个卡牌添加鼠标事件监听器
        _eventDispatcher->addEventListenerWithSceneGraphPriority(cardListener, cardSprite);
    }

    // 设置滚动视图的内容区域大小
    float contentHeight = 1200;  // 滚动视图内容的高度
    float contentWidth = 400 * 4;  // 内容的宽度

    // 添加鼠标滚轮事件监听器
    auto listener = cocos2d::EventListenerMouse::create();
    listener->onMouseScroll = CC_CALLBACK_1(CardLayer::onMouseScroll, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    // 设置滚动视图的尺寸和位置
    _scrollView->setContentSize(cocos2d::Size(1600, 1200));
    _scrollView->setPosition(cocos2d::Vec2(800, 600));

    // 设置滚动视图内容区域的位置
    _scrollView->setInnerContainerPosition(cocos2d::Vec2(0, 0));

    // 确保滚动视图的容器区域没有遮挡
    cardContainer->setLocalZOrder(100); // 低层级显示卡牌容器，防止被滚动视图遮挡
}

void CardLayer::onMouseScroll(cocos2d::EventMouse* event) {
    // 获取鼠标滚轮的偏移量
    float scrollDelta = event->getScrollY();

    // 获取当前ScrollView的滚动位置
    cocos2d::Vec2 currentPos = _scrollView->getInnerContainerPosition();

    // 获取ScrollView的显示区域和内容区域的尺寸
    cocos2d::Size innerSize = _scrollView->getInnerContainerSize();
    cocos2d::Size contentSize = _scrollView->getContentSize();

    // 计算滚动的上下限
    float maxScrollY = 200;
    float minScrollY = -700; // 最小滚动位置，内容区域超出显示区域时为负值

    // 计算新的滚动位置
    float newYPos = currentPos.y + scrollDelta * 2;
     
    // 限制新的滚动位置在上下界限之间
    newYPos = std::min(std::max(newYPos, minScrollY), maxScrollY);

    // 设置新的滚动位置
    _scrollView->setInnerContainerPosition(cocos2d::Vec2(0, newYPos));

    // 重新更新卡牌的BoundingBox（位置）
    updateCardBoundingBoxes();
}



// 更新卡牌BoundingBox的位置
void CardLayer::updateCardBoundingBoxes() {
    cocos2d::Vec2 containerPos = _scrollView->getInnerContainerPosition();

    // 遍历所有卡牌精灵并更新它们的boundingBox
    for (auto& cardSprite : _cardSprites) {
        // 获取卡牌的当前位置并更新BoundingBox
        cocos2d::Vec2 cardPos = cardSprite->getPosition();
        cardPos += containerPos;  // 更新卡牌的位置，使其反映滚动视图的偏移量

        // 更新卡牌的BoundingBox
        cardSprite->setPosition(cardPos);
    }
}


void CardLayer::closeLayerCallback(cocos2d::Ref* sender) {
    this->removeFromParentAndCleanup(true);
}

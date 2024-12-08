#include "IncludeAll.h"

HandPileLayer* HandPileLayer::instance_ = nullptr;

HandPileLayer* HandPileLayer::getInstance()
{
    if (instance_ == nullptr)
    {
        instance_ = new HandPileLayer(); // ����Ψһʵ��  
    }
    return instance_;
}

bool HandPileLayer::init()
{
    combatSystem = CombatSystem::getInstance();


    //�������ƶ�����
    hand = CombatSystem::getInstance()->getHandPile();
    discardPile = CombatSystem::getInstance()->discardPile;


    return true;
}

void HandPileLayer::updateHandPile()
{
    // ������еĿ��ƾ���
    for (auto sprite : cardSprites)
    {
        sprite->removeFromParent();
    }
    cardSprites.clear();

    // ��ȡ��ǰ����
    auto& hand = CombatSystem::getInstance()->getHandPile();

    // �������еĿ�����ʾ����
    float xPos = 100;  // ���Ƴ�ʼ��Xλ��
    for (const auto& card : hand)
    {
        auto cardSprite = CardSpriteGenerator::createCardSprite(card);
        cardSprite->setPosition(cocos2d::Vec2(xPos, 300));  // ���ÿ�����ʾ��λ��
        this->addChild(cardSprite);
        cardSprites.push_back(cardSprite);
        xPos += 100;  // ����֮��ļ��
    }
}

void HandPileLayer::enableCardDrag(Sprite* cardSprite, std::shared_ptr<Card> card)
{
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);

    listener->onTouchBegan = [=](Touch* touch, Event* event) -> bool {
        auto location = touch->getLocation();
        if (cardSprite->getBoundingBox().containsPoint(location)) {
            return true;
        }
        return false;
        };

    listener->onTouchMoved = [=](Touch* touch, Event* event) {
        auto location = touch->getLocation();
        cardSprite->setPosition(location);
        };

    auto playArea = Rect(500, 500, 400, 200); // ���ô������
    listener->onTouchEnded = [=](Touch* touch, Event* event) {
        auto location = touch->getLocation();
        if (playArea.containsPoint(location)) {
            CCLOG("Played card: %s", card->getName().c_str());
            CCLOG("Take Effect: %s", card->getDescription().c_str());
            discardPile.push(card);
            hand.erase(std::remove(hand.begin(), hand.end(), card), hand.end());
            cardSprite->removeFromParent();

        }
        adjustHandPile();
        };

    // ����϶�����
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, cardSprite);

    // ʹ�ÿ��Ƶ�ַ��ΪΨһ tag
    cardSprite->setTag(reinterpret_cast<intptr_t>(card.get()));
}

void HandPileLayer::drawCard(std::shared_ptr<Card> card)
{
    // ��������
    auto cardSprite = CardSpriteGenerator::createCardSprite(card);
    cardSprite->setPosition(Vec2(400, 300)); // ��ʼλ��
    this->addChild(cardSprite);

    // �����϶�
    enableCardDrag(cardSprite, card);

    adjustHandPile();
}

void HandPileLayer::adjustHandPile()
{
    // ��������λ��
    Vec2 handCenter = Vec2(500, 150);
    float cardSpacing = 100.0f; // ���Ƽ��
    float totalWidth = (hand.size() - 1) * cardSpacing;

    auto& newhand = CombatSystem::getInstance()->hand;
    CCLOG("now my hand has %d cards", newhand.size());
    for (size_t i = 0; i < newhand.size(); ++i)
    {
        // ����ÿ�����ƶ�Ӧ�ľ����� hand[i] �ĵ�ַ��ΪΨһ tag
        auto cardSprite = this->getChildByTag(reinterpret_cast<intptr_t>(newhand[i].get()));
        if (cardSprite) {
            
            // ����������������λ�ã�ȷ��������ʾ
            float xOffset = i * cardSpacing;

            Vec2 newPosition = handCenter + Vec2(xOffset, 0);
            
            //Vec2 newPosition = Vec2(400, 300);
            // ʹ�ö����ƶ�����λ�ã��Եø�ƽ��
            cardSprite->runAction(MoveTo::create(0.2f, newPosition));
        }
    }
}


bool CombatScene::init() 
{
    // ���������ñ���ͼ��
    auto background = cocos2d::Sprite::create("combatScene.png");
    if (background) {
        background->setContentSize(Size(1648, 1500)); // ���ÿ��ƴ�С
        background->setPosition(cocos2d::Vec2(750, 200));
        this->addChild(background);
    }
    else{
        CCLOG("Can't find: combatScene.png!");
    }

    // �������ƶ�ͼ��
    auto drawPileIcon = cocos2d::Sprite::create("drawPileIcon.png");
    if (drawPileIcon) {
        drawPileIcon->setPosition(cocos2d::Vec2(100, 100));
        this->addChild(drawPileIcon);
    }
    else {
        CCLOG("Can't find: drawPileIcon.png");
    }

    // �������ƶ�ͼ��
    auto discardPileIcon = cocos2d::Sprite::create("discardPileIcon.png");
    if (discardPileIcon) {
        discardPileIcon->setPosition(cocos2d::Vec2(1300, 100));
        this->addChild(discardPileIcon);
    }
    else {
        CCLOG("Can't find: discardPileIcon.png");
    }

    const auto screenSize = cocos2d::Director::getInstance()->getVisibleSize();


    isMyTurn = 1;
    //�����غϽ�����ť,������غϽ�����ť֮�󣬶������еĿ���
    auto endTurnButton = HoverButton::create("endTurnButton.png", "endTurnButtonGlow.png", "endTurnButton.png");
    endTurnButton->setTitleText(u8"�غϽ���");
    endTurnButton->setScale(1.5f);
    endTurnButton->setTitleFontSize(20);
    endTurnButton->setPosition(Vec2(screenSize.width - 200, screenSize.height - 650));

    //����ʹ�ã��غϿ�ʼ��ť��������غϿ�ʼ��ť֮��
    auto startTurnButton = HoverButton::create("endTurnButton.png", "endTurnButtonGlow.png", "endTurnButton.png");
    startTurnButton->setTitleText(u8"�غϿ�ʼ");
    startTurnButton->setScale(1.5f);
    startTurnButton->setTitleFontSize(20);
    startTurnButton->setPosition(Vec2(screenSize.width - 1200, screenSize.height - 650));
    
    // �����غϰ�ť
    endTurnButton->addTouchEventListener([&](cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type) {
        if (type == cocos2d::ui::Widget::TouchEventType::ENDED) {
            if(isMyTurn)
            {
                CCLOG("End Turn clicked!%d", isMyTurn);  // ��ӡ��־
            }
            isMyTurn = 0;
        }
        });

    // ��ʼ�غϰ�ť
    startTurnButton->addTouchEventListener([&](cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type) {
        if (type == cocos2d::ui::Widget::TouchEventType::ENDED ) {
            if (!isMyTurn)
            {
                CCLOG("Start Turn clicked!%d", isMyTurn);  // ��ӡ��־
                CombatSystem::getInstance()->drawCard(5);
            }
            isMyTurn = 1;
        }
        });
    this->addChild(endTurnButton);
    this->addChild(startTurnButton);


    //����ʹ�ã�����һ���ܴ���Ƶ����򣬵����Ʊ��϶����������ʱ�����
    playArea = Rect(500, 500, 400, 200); // ���ô������
    auto playAreaNode = DrawNode::create();
    playAreaNode->drawRect(playArea.origin, playArea.origin + playArea.size, Color4F::GRAY);
    this->addChild(playAreaNode);

    this->addChild(HandPileLayer::getInstance());
    return true;
}

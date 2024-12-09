#include "IncludeAll.h"
#include "HeaderBar.h"
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
    //��Ҫ���������ƶ����������ƶ�����

    return true;
}

//�����϶�
void HandPileLayer::enableCardDrag(Sprite* cardSprite, std::shared_ptr<Card> card)
{
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);

    //
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

    const cocos2d::Size screenSize = cocos2d::Director::getInstance()->getWinSize();
    //����ʹ��
    auto playArea = Rect(screenSize.width / 2, screenSize.height / 2, 0.3 * screenSize.width, 0.3 * screenSize.height); // ���ô������

    listener->onTouchEnded = [=](Touch* touch, Event* event) {
        auto location = touch->getLocation();
        auto& newhand = CombatSystem::getInstance()->hand; //����
        if (playArea.containsPoint(location)) {
            CCLOG("Played card: %s", card->getName().c_str());
            CCLOG("Take Effect: %s", card->getDescription().c_str());
            CombatSystem::getInstance()->discardPile.push(card); //�������ƶ�
            newhand.erase(std::remove(newhand.begin(), newhand.end(), card), newhand.end()); //�����Ƴ�����
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
    cardSprite->setPosition(Vec2(400, 300)); // ��ʼλ�ã����ֵû���κ�Ӱ��
    this->addChild(cardSprite);

    // �����϶�
    enableCardDrag(cardSprite, card);

    // �Գ��ƶѽ��и���
    auto currentScene = Director::getInstance()->getRunningScene();
    if (currentScene && dynamic_cast<CombatScene*>(currentScene)) {//����Ƿ�Ϊս������
        CombatScene* combatScene = static_cast<CombatScene*>(currentScene);
        combatScene->updateDrawPileDisplay();  
    }
}

void HandPileLayer::adjustHandPile()
{
    // ��������λ��
    const cocos2d::Size screenSize = cocos2d::Director::getInstance()->getWinSize();
    Vec2 handCenter = Vec2(screenSize.width / 2, 0.13 * screenSize.height);
    float cardSpacing = 0.1 * screenSize.height; // ���Ƽ��
    auto& newhand = CombatSystem::getInstance()->hand;
    float totalWidth = (newhand.size() - 1) * cardSpacing;
    CCLOG("now my hand has %d cards", newhand.size());
    for (size_t i = 0; i < newhand.size(); ++i)
    {
        // ����ÿ�����ƶ�Ӧ�ľ����� hand[i] �ĵ�ַ��ΪΨһ tag
        auto cardSprite = this->getChildByTag(reinterpret_cast<intptr_t>(newhand[i].get()));
        if (cardSprite) {
            
            // ����������������λ�ã�ȷ��������ʾ
            float xOffset = -totalWidth / 2 + i * cardSpacing;

            Vec2 newPosition = handCenter + Vec2(xOffset, 400);
            // ʹ�ö����ƶ�����λ�ã��Եø�ƽ��
            cardSprite->runAction(MoveTo::create(0.2f, newPosition));
        }
    }
}


bool CombatScene::init() 
{
    shared_ptr<Player> player = Player::getInstance();
    auto headbar = HeaderBar::create(player);
    headbar->setPosition(Vec2(0, 750));          // ����λ�ã�����Ļ�ϲ���
    this->addChild(headbar);

    headbar->setLocalZOrder(200);
    // ���������ñ���ͼ��
    auto background = cocos2d::Sprite::create("combatScene.png");
    cocos2d::Size screenSize = cocos2d::Director::getInstance()->getWinSize();
    if (background) 
    {
        background->setScale(1.1f);
        background->setContentSize(Size(1.1 * screenSize.width, 1.5 * screenSize.height)); // ���ñ�����С
        background->setPosition(cocos2d::Vec2(screenSize.width / 2 + 0.05 * screenSize.width, 
            screenSize.height / 2 - 0.25 * screenSize.height-130));
        this->addChild(background);
    }
    else{
        CCLOG("Can't find: combatScene.png!");
    }

    // �������ƶ�ͼ��
    auto drawPileIcon = cocos2d::Sprite::create("drawPileIcon.png");
    if (drawPileIcon) {
        drawPileIcon->setPosition(cocos2d::Vec2(0.1 * screenSize.width, 0.12 * screenSize.height));
        this->addChild(drawPileIcon);
    }
    else {
        CCLOG("Can't find: drawPileIcon.png");
    }

    // �������ƶ��������
    auto drawPileNumCount = cocos2d::Sprite::create("countCircle.png");
    if (drawPileNumCount) {
        drawPileNumCount->setPosition(cocos2d::Vec2(0.078125* screenSize.width+20, 0.078125 * screenSize.height));
        this->addChild(drawPileNumCount);
    }
    drawPileNumLabel = cocos2d::Label::createWithSystemFont(std::to_string(0), "Arial", 20);
    drawPileNumLabel->setPosition(cocos2d::Vec2(0.078125 * screenSize.width + 20, 0.078125 * screenSize.height));  // ����������ͼ�������
    drawPileNumLabel->setColor(cocos2d::Color3B::WHITE);  // ����������ɫ
    this->addChild(drawPileNumLabel);
    updateDrawPileDisplay();
    if(!drawPileNumLabel)
        CCLOG("areyouok");

    // �������ƶ�ͼ��
    auto discardPileIcon = cocos2d::Sprite::create("discardPileIcon.png");
    if (discardPileIcon) {
        discardPileIcon->setPosition(cocos2d::Vec2(0.88 * screenSize.width, 0.12 * screenSize.height));
        this->addChild(discardPileIcon);
    }
    else {
        CCLOG("Can't find: discardPileIcon.png");
    }


    // �������ƶѿ���������־
    auto discardPileNumCount = cocos2d::Sprite::create("countCircle.png");
    if (discardPileNumCount) {
        discardPileNumCount->setPosition(cocos2d::Vec2(0.9 * screenSize.width+20, 0.078125 * screenSize.height));
        this->addChild(discardPileNumCount);
    }
    discardPileNumLabel = cocos2d::Label::createWithSystemFont(std::to_string(0), "Arial", 20);
    discardPileNumLabel->setPosition(cocos2d::Vec2(0.9 * screenSize.width + 20, 0.078125 * screenSize.height));  // ����������ͼ�������
    discardPileNumLabel->setColor(cocos2d::Color3B::WHITE);  // ����������ɫ
    this->addChild(discardPileNumLabel);

    //����������־
    auto energyLayer1 = Sprite::create("energyLayer1.png");
    energyLayer1->setContentSize(Size(200, 200));
    energyLayer1->setPosition(cocos2d::Vec2(0.0976525 * screenSize.width, 0.390625 * screenSize.height));
    this->addChild(energyLayer1);
    auto energyLayer2 = Sprite::create("energyGreenVFX.png");
    energyLayer2->setContentSize(Size(120, 120));
    energyLayer2->setPosition(cocos2d::Vec2(0.0976525 * screenSize.width, 0.390625 * screenSize.height));
    this->addChild(energyLayer2);

    // ������ʾ����������
    energyLabel = cocos2d::Label::createWithSystemFont(std::to_string((int)3) + "/" + std::to_string((int)3), "Arial", 30);
    energyLabel->setPosition(cocos2d::Vec2(200, 365));  // ����������ͼ�������
    energyLabel->setColor(cocos2d::Color3B::BLACK);  // ����������ɫ
    this->addChild(energyLabel);

    // ���������и���
    updateEnergyDisplay();

   

    //ս����ʼʱ�����趨��Ϊ�ҷ��غ�
    isMyTurn = 0;

    //�����غϽ�����ť,������غϽ�����ť֮�󣬶������еĿ���
    auto endTurnButton = HoverButton::create("endTurnButton.png", "endTurnButtonGlow.png", "endTurnButton.png");
    endTurnButton->setTitleText(u8"�غϽ���");
    endTurnButton->setScale(1.5f);
    endTurnButton->setTitleFontSize(20);
    endTurnButton->setPosition(Vec2(0.906525 * screenSize.width, 0.25625 * screenSize.height));

    //����ʹ�ã��غϿ�ʼ��ť��������غϿ�ʼ��ť֮�󣬿��Գ�5����
    auto startTurnButton = HoverButton::create("endTurnButton.png", "endTurnButtonGlow.png", "endTurnButton.png");
    startTurnButton->setTitleText(u8"�غϿ�ʼ");
    startTurnButton->setScale(1.5f);
    startTurnButton->setTitleFontSize(20);
    startTurnButton->setPosition(Vec2(0.0976525 * screenSize.width, 0.25625 * screenSize.height));
    
    // �����غϰ�ť
    endTurnButton->addTouchEventListener([&](cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type) {
        if (type == cocos2d::ui::Widget::TouchEventType::ENDED) {
            if(isMyTurn)
            {
                CCLOG("End Turn clicked!%d");  // ��ӡ��־
                CombatSystem::getInstance()->endTurn(Player::getInstance());//��һغϽ���
            }
            isMyTurn = 0;
        }
        });

    // ��ʼ�غϰ�ť
    startTurnButton->addTouchEventListener([&](cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type) {
        if (type == cocos2d::ui::Widget::TouchEventType::ENDED ) {
            if (!isMyTurn)
            {
                CCLOG("Start Turn clicked!%d");  // ��ӡ��־
                CombatSystem::getInstance()->startTurn(Player::getInstance());//��һغϿ�ʼ�����и���
            }
            isMyTurn = 1;
        }
        });
    this->addChild(endTurnButton);
    this->addChild(startTurnButton);


    //����ʹ�ã�����һ���ܴ���Ƶ����򣬵����Ʊ��϶����������ʱ�����
    playArea = Rect(screenSize.width / 2 - 0.15 * screenSize.width, screenSize.height / 2, 
        0.3 * screenSize.width, 0.3 * screenSize.height); // ���ô������
    auto playAreaNode = DrawNode::create();
    playAreaNode->drawRect(playArea.origin, playArea.origin + playArea.size, Color4F::GRAY);
    this->addChild(playAreaNode);

    this->addChild(HandPileLayer::getInstance());
    return true;
}

void CombatScene::updateEnergyDisplay()
{
    int currentEnergy = 4;
    int maxEnergy = 4;
    energyLabel->setString(std::to_string((int)currentEnergy) + "/" + std::to_string((int)maxEnergy));  // ���±�ǩ�ı�Ϊ��ǰ����ֵ
}

void CombatScene::updateDrawPileDisplay()
{
    const int drawPileNum = CombatSystem::getInstance()->getDrawPileNumber();
    drawPileNumLabel->setString(std::to_string(drawPileNum));
}

void CombatScene::updateDiscardPileDisplay()
{
    const int discardPileNum = CombatSystem::getInstance()->getDiscardPileNumber();
    discardPileNumLabel->setString(std::to_string(discardPileNum));
}
#include "IncludeAll.h"
#include "HeaderBar.h"
HandPileLayer* HandPileLayer::instance_ = nullptr;

HandPileLayer* HandPileLayer::getInstance()
{
    if (instance_ == nullptr)
    {
        instance_ = new HandPileLayer(); // 创建唯一实例  
    }
    return instance_;
}

bool HandPileLayer::init()
{
    //需要创建：抽牌堆数量，弃牌堆数量

    return true;
}

//卡牌拖动
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
    //测试使用
    auto playArea = Rect(screenSize.width / 2, screenSize.height / 2, 0.3 * screenSize.width, 0.3 * screenSize.height); // 设置打出区域

    listener->onTouchEnded = [=](Touch* touch, Event* event) {
        auto location = touch->getLocation();
        auto& newhand = CombatSystem::getInstance()->hand; //手牌
        if (playArea.containsPoint(location)) {
            CCLOG("Played card: %s", card->getName().c_str());
            CCLOG("Take Effect: %s", card->getDescription().c_str());
            CombatSystem::getInstance()->discardPile.push(card); //置入弃牌堆
            newhand.erase(std::remove(newhand.begin(), newhand.end(), card), newhand.end()); //手牌移除卡牌
            cardSprite->removeFromParent();
        }
        adjustHandPile();
        };

    // 添加拖动监听
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, cardSprite);

    // 使用卡牌地址作为唯一 tag
    cardSprite->setTag(reinterpret_cast<intptr_t>(card.get()));
}

void HandPileLayer::drawCard(std::shared_ptr<Card> card)
{
    // 创建精灵
    auto cardSprite = CardSpriteGenerator::createCardSprite(card);
    cardSprite->setPosition(Vec2(400, 300)); // 初始位置，这个值没有任何影响
    this->addChild(cardSprite);

    // 启用拖动
    enableCardDrag(cardSprite, card);

    // 对抽牌堆进行更新
    auto currentScene = Director::getInstance()->getRunningScene();
    if (currentScene && dynamic_cast<CombatScene*>(currentScene)) {//检查是否为战斗场景
        CombatScene* combatScene = static_cast<CombatScene*>(currentScene);
        combatScene->updateDrawPileDisplay();  
    }
}

void HandPileLayer::adjustHandPile()
{
    // 手牌中心位置
    const cocos2d::Size screenSize = cocos2d::Director::getInstance()->getWinSize();
    Vec2 handCenter = Vec2(screenSize.width / 2, 0.13 * screenSize.height);
    float cardSpacing = 0.1 * screenSize.height; // 卡牌间距
    auto& newhand = CombatSystem::getInstance()->hand;
    float totalWidth = (newhand.size() - 1) * cardSpacing;
    CCLOG("now my hand has %d cards", newhand.size());
    for (size_t i = 0; i < newhand.size(); ++i)
    {
        // 假设每个手牌对应的精灵用 hand[i] 的地址作为唯一 tag
        auto cardSprite = this->getChildByTag(reinterpret_cast<intptr_t>(newhand[i].get()));
        if (cardSprite) {
            
            // 根据手牌数量调整位置，确保居中显示
            float xOffset = -totalWidth / 2 + i * cardSpacing;

            Vec2 newPosition = handCenter + Vec2(xOffset, 400);
            // 使用动作移动到新位置，显得更平滑
            cardSprite->runAction(MoveTo::create(0.2f, newPosition));
        }
    }
}


bool CombatScene::init() 
{
    shared_ptr<Player> player = Player::getInstance();
    auto headbar = HeaderBar::create(player);
    headbar->setPosition(Vec2(0, 750));          // 设置位置（在屏幕上部）
    this->addChild(headbar);

    headbar->setLocalZOrder(200);
    // 创建并设置背景图像
    auto background = cocos2d::Sprite::create("combatScene.png");
    cocos2d::Size screenSize = cocos2d::Director::getInstance()->getWinSize();
    if (background) 
    {
        background->setScale(1.1f);
        background->setContentSize(Size(1.1 * screenSize.width, 1.5 * screenSize.height)); // 设置背景大小
        background->setPosition(cocos2d::Vec2(screenSize.width / 2 + 0.05 * screenSize.width, 
            screenSize.height / 2 - 0.25 * screenSize.height-130));
        this->addChild(background);
    }
    else{
        CCLOG("Can't find: combatScene.png!");
    }

    // 创建抽牌堆图标
    auto drawPileIcon = cocos2d::Sprite::create("drawPileIcon.png");
    if (drawPileIcon) {
        drawPileIcon->setPosition(cocos2d::Vec2(0.1 * screenSize.width, 0.12 * screenSize.height));
        this->addChild(drawPileIcon);
    }
    else {
        CCLOG("Can't find: drawPileIcon.png");
    }

    // 创建抽牌堆数量标记
    auto drawPileNumCount = cocos2d::Sprite::create("countCircle.png");
    if (drawPileNumCount) {
        drawPileNumCount->setPosition(cocos2d::Vec2(0.078125* screenSize.width+20, 0.078125 * screenSize.height));
        this->addChild(drawPileNumCount);
    }
    drawPileNumLabel = cocos2d::Label::createWithSystemFont(std::to_string(0), "Arial", 20);
    drawPileNumLabel->setPosition(cocos2d::Vec2(0.078125 * screenSize.width + 20, 0.078125 * screenSize.height));  // 设置在能量图标的中心
    drawPileNumLabel->setColor(cocos2d::Color3B::WHITE);  // 设置文字颜色
    this->addChild(drawPileNumLabel);
    updateDrawPileDisplay();
    if(!drawPileNumLabel)
        CCLOG("areyouok");

    // 创建弃牌堆图标
    auto discardPileIcon = cocos2d::Sprite::create("discardPileIcon.png");
    if (discardPileIcon) {
        discardPileIcon->setPosition(cocos2d::Vec2(0.88 * screenSize.width, 0.12 * screenSize.height));
        this->addChild(discardPileIcon);
    }
    else {
        CCLOG("Can't find: discardPileIcon.png");
    }


    // 创建弃牌堆卡牌数量标志
    auto discardPileNumCount = cocos2d::Sprite::create("countCircle.png");
    if (discardPileNumCount) {
        discardPileNumCount->setPosition(cocos2d::Vec2(0.9 * screenSize.width+20, 0.078125 * screenSize.height));
        this->addChild(discardPileNumCount);
    }
    discardPileNumLabel = cocos2d::Label::createWithSystemFont(std::to_string(0), "Arial", 20);
    discardPileNumLabel->setPosition(cocos2d::Vec2(0.9 * screenSize.width + 20, 0.078125 * screenSize.height));  // 设置在能量图标的中心
    discardPileNumLabel->setColor(cocos2d::Color3B::WHITE);  // 设置文字颜色
    this->addChild(discardPileNumLabel);

    //创建能量标志
    auto energyLayer1 = Sprite::create("energyLayer1.png");
    energyLayer1->setContentSize(Size(200, 200));
    energyLayer1->setPosition(cocos2d::Vec2(0.0976525 * screenSize.width, 0.390625 * screenSize.height));
    this->addChild(energyLayer1);
    auto energyLayer2 = Sprite::create("energyGreenVFX.png");
    energyLayer2->setContentSize(Size(120, 120));
    energyLayer2->setPosition(cocos2d::Vec2(0.0976525 * screenSize.width, 0.390625 * screenSize.height));
    this->addChild(energyLayer2);

    // 创建显示能量的数字
    energyLabel = cocos2d::Label::createWithSystemFont(std::to_string((int)3) + "/" + std::to_string((int)3), "Arial", 30);
    energyLabel->setPosition(cocos2d::Vec2(200, 365));  // 设置在能量图标的中心
    energyLabel->setColor(cocos2d::Color3B::BLACK);  // 设置文字颜色
    this->addChild(energyLabel);

    // 对能量进行更新
    updateEnergyDisplay();

   

    //战斗开始时，先设定不为我方回合
    isMyTurn = 0;

    //创建回合结束按钮,当点击回合结束按钮之后，丢弃所有的卡牌
    auto endTurnButton = HoverButton::create("endTurnButton.png", "endTurnButtonGlow.png", "endTurnButton.png");
    endTurnButton->setTitleText(u8"回合结束");
    endTurnButton->setScale(1.5f);
    endTurnButton->setTitleFontSize(20);
    endTurnButton->setPosition(Vec2(0.906525 * screenSize.width, 0.25625 * screenSize.height));

    //测试使用，回合开始按钮，当点击回合开始按钮之后，可以抽5张牌
    auto startTurnButton = HoverButton::create("endTurnButton.png", "endTurnButtonGlow.png", "endTurnButton.png");
    startTurnButton->setTitleText(u8"回合开始");
    startTurnButton->setScale(1.5f);
    startTurnButton->setTitleFontSize(20);
    startTurnButton->setPosition(Vec2(0.0976525 * screenSize.width, 0.25625 * screenSize.height));
    
    // 结束回合按钮
    endTurnButton->addTouchEventListener([&](cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type) {
        if (type == cocos2d::ui::Widget::TouchEventType::ENDED) {
            if(isMyTurn)
            {
                CCLOG("End Turn clicked!%d");  // 打印日志
                CombatSystem::getInstance()->endTurn(Player::getInstance());//玩家回合结束
            }
            isMyTurn = 0;
        }
        });

    // 开始回合按钮
    startTurnButton->addTouchEventListener([&](cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type) {
        if (type == cocos2d::ui::Widget::TouchEventType::ENDED ) {
            if (!isMyTurn)
            {
                CCLOG("Start Turn clicked!%d");  // 打印日志
                CombatSystem::getInstance()->startTurn(Player::getInstance());//玩家回合开始，进行更新
            }
            isMyTurn = 1;
        }
        });
    this->addChild(endTurnButton);
    this->addChild(startTurnButton);


    //测试使用：创建一个能打出牌的区域，当卡牌被拖动到这个区域时被打出
    playArea = Rect(screenSize.width / 2 - 0.15 * screenSize.width, screenSize.height / 2, 
        0.3 * screenSize.width, 0.3 * screenSize.height); // 设置打出区域
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
    energyLabel->setString(std::to_string((int)currentEnergy) + "/" + std::to_string((int)maxEnergy));  // 更新标签文本为当前能量值
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
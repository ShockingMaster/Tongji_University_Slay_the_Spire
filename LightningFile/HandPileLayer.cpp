#include "IncludeAll.h"

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
    const cocos2d::Size screenSize = cocos2d::Director::getInstance()->getWinSize();
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
        drawPileNumCount->setPosition(cocos2d::Vec2(0.078125 * screenSize.width, 0.078125 * screenSize.height));
        this->addChild(drawPileNumCount);
    }
    HandPileLayer::getInstance()->drawPileNumLabel = cocos2d::Label::createWithSystemFont(std::to_string(0), "Arial", 20);
    HandPileLayer::getInstance()->drawPileNumLabel->setPosition(cocos2d::Vec2(0.078125 * screenSize.width, 0.078125 * screenSize.height));  // 设置在能量图标的中心
    HandPileLayer::getInstance()->drawPileNumLabel->setColor(cocos2d::Color3B::WHITE);  // 设置文字颜色
    this->addChild(HandPileLayer::getInstance()->drawPileNumLabel);
    updateDrawPileDisplay();

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
        discardPileNumCount->setPosition(cocos2d::Vec2(0.9 * screenSize.width, 0.078125 * screenSize.height));
        this->addChild(discardPileNumCount);
    }
    HandPileLayer::getInstance()->discardPileNumLabel = cocos2d::Label::createWithSystemFont(std::to_string(0), "Arial", 20);
    HandPileLayer::getInstance()->discardPileNumLabel->setPosition(cocos2d::Vec2(0.9 * screenSize.width, 0.078125 * screenSize.height));  // 设置在能量图标的中心
    HandPileLayer::getInstance()->discardPileNumLabel->setColor(cocos2d::Color3B::WHITE);  // 设置文字颜色
    this->addChild(HandPileLayer::getInstance()->discardPileNumLabel);
    return true;
}

/*
* 函数名称:enableCardDrag
* 参数：卡牌精灵及相应的卡牌智能指针
* 功能：设定卡牌可以被拖动
*/
void HandPileLayer::enableCardDrag(Sprite* cardSprite, std::shared_ptr<Card> card)
{
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);

    // 如果触碰区域有卡牌，那么返回true
    listener->onTouchBegan = [=](Touch* touch, Event* event) -> bool {
        auto location = touch->getLocation();
        if (cardSprite->getBoundingBox().containsPoint(location)) {
            return true;
        }
        return false;
        };

    // 将点击到的卡牌跟随鼠标移动
    listener->onTouchMoved = [=](Touch* touch, Event* event) {
        auto location = touch->getLocation();
        cardSprite->setPosition(location);
        };

    const cocos2d::Size screenSize = cocos2d::Director::getInstance()->getWinSize();


    // 测试使用，对于设定卡牌打出区域，如果在这一区域释放卡牌，那么打出卡牌
    auto playArea = Rect(screenSize.width / 2 - 0.15 * screenSize.width, screenSize.height / 2,
        0.3 * screenSize.width, 0.3 * screenSize.height);
    listener->onTouchEnded = [=](Touch* touch, Event* event) {
        auto location = touch->getLocation();
        auto& newhand = CombatSystem::getInstance()->hand;                                      //获取手牌引用

        //这里的card->needTarget()是错误的，仅仅为了进行测试
        if (playArea.containsPoint(location) && card->getCanBePlayed() && 1   //对于不需要选中敌人的卡牌
            && card->getEnergyCost() <= Player::getInstance()->getCurrentEnergy()) 
        {
            //auto& tempcard = card;
            newhand.erase(std::remove(newhand.begin(), newhand.end(), card), newhand.end());    //在手牌中移除这张卡牌
            CombatSystem::getInstance()->cardPlayed(card);                                      //告诉战斗系统打出这张牌
            cardSprite->removeFromParent();                                                     //将卡牌移除出屏幕

            if (!card->isExhaust()) {                                                           //如果卡牌不是消耗类型
                CombatSystem::getInstance()->discardPile.push(card);                            //向弃牌堆中加入这张卡牌
                HandPileLayer::getInstance()->updateDiscardPileDisplay();                       //对弃牌堆进行更新
            }
        }
        //需要更新！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！
        //
        //
        else if (playArea.containsPoint(location) 
            && card->getEnergyCost() <= Player::getInstance()->getCurrentEnergy())//还需要更新对于需要选中敌人的卡牌的逻辑
        {

        }
        adjustHandPile();                                                                   //每次进行点击都调整卡牌位置
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
    cardSprite->setPosition(Vec2(400, 300));                                // 初始位置，这个值没有任何影响,后续会进行更新
    this->addChild(cardSprite);

    // 启用拖动
    enableCardDrag(cardSprite, card);

    // 对抽牌堆进行更新
    updateDrawPileDisplay();

    //adjustHandPile();
}


/*
* 函数名称：removeCard
* 功能：给定某一张卡牌的指针，将该卡牌对应的精灵移除
*/
void HandPileLayer::removeCard(std::shared_ptr<Card> card)
{
    auto& newhand = CombatSystem::getInstance()->hand;
    for (size_t i = 0; i < newhand.size(); ++i)
    {
        if (card == newhand[i])
        {
            auto cardSprite = HandPileLayer::getInstance()->getChildByTag(reinterpret_cast<intptr_t>(newhand[i].get()));
            HandPileLayer::getInstance()->removeChild(cardSprite);
        }
    }
}
/*
* 函数名称：adjustHandPile()
*
*
*/
void HandPileLayer::adjustHandPile()
{
    // 手牌中心位置
    const cocos2d::Size screenSize = cocos2d::Director::getInstance()->getWinSize();
    Vec2 handCenter = Vec2(screenSize.width / 2, 0.13 * screenSize.height);
    const float cardSpacing = 0.1 * screenSize.height;                              // 卡牌间距
    auto& newhand = CombatSystem::getInstance()->hand;
    const float totalWidth = (newhand.size() - 1) * cardSpacing;
    CCLOG("now my hand has %d cards", newhand.size());
    for (size_t i = 0; i < newhand.size(); ++i)
    {
        // 每个手牌对应的精灵用 hand[i] 的地址作为唯一 tag,确定唯一卡牌
        auto cardSprite = this->getChildByTag(reinterpret_cast<intptr_t>(newhand[i].get()));
        if (cardSprite) {

            // 根据手牌数量调整位置，确保居中显示
            const float xOffset = -totalWidth / 2 + i * cardSpacing;
            Vec2 newPosition = handCenter + Vec2(xOffset, 0);

            // 使用动作移动到新位置，显得更平滑
            auto moveAction = MoveTo::create(0.2f, newPosition);

            // 确保只有一个移动动作在执行
            cardSprite->stopAllActions();
            cardSprite->runAction(moveAction);
    
            CCLOG("Card %zu position: (%f, %f)", i, newPosition.x, newPosition.y);
        }
    }
}
/*
* 函数名称：updateDrawPileDisplay
* 功能：根据战斗系统的抽牌堆数量进行更新
*/
void HandPileLayer::updateDrawPileDisplay()
{
    const int drawPileNum = CombatSystem::getInstance()->getDrawPileNumber();
    HandPileLayer::getInstance()->drawPileNumLabel->setString(std::to_string(drawPileNum));
}

/*
* 函数名称:updateDiscardPileDisplay
* 功能：根据战斗系统的弃牌堆数量进行更新
*/
void HandPileLayer::updateDiscardPileDisplay()
{
    const int discardPileNum = CombatSystem::getInstance()->getDiscardPileNumber();
    HandPileLayer::getInstance()->discardPileNumLabel->setString(std::to_string(discardPileNum));
}
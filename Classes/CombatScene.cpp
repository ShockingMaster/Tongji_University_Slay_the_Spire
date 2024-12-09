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
    combatSystem = CombatSystem::getInstance();


    //创建手牌堆引用
    hand = CombatSystem::getInstance()->getHandPile();
    discardPile = CombatSystem::getInstance()->discardPile;


    return true;
}

void HandPileLayer::updateHandPile()
{
    // 清除现有的卡牌精灵
    for (auto sprite : cardSprites)
    {
        sprite->removeFromParent();
    }
    cardSprites.clear();

    // 获取当前手牌
    auto& hand = CombatSystem::getInstance()->getHandPile();

    // 将手牌中的卡牌显示出来
    float xPos = 100;  // 卡牌初始的X位置
    for (const auto& card : hand)
    {
        auto cardSprite = CardSpriteGenerator::createCardSprite(card);
        cardSprite->setPosition(cocos2d::Vec2(xPos, 300));  // 设置卡牌显示的位置
        this->addChild(cardSprite);
        cardSprites.push_back(cardSprite);
        xPos += 100;  // 卡牌之间的间距
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

    auto playArea = Rect(500, 500, 400, 200); // 设置打出区域
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

    // 添加拖动监听
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, cardSprite);

    // 使用卡牌地址作为唯一 tag
    cardSprite->setTag(reinterpret_cast<intptr_t>(card.get()));
}

void HandPileLayer::drawCard(std::shared_ptr<Card> card)
{
    // 创建精灵
    auto cardSprite = CardSpriteGenerator::createCardSprite(card);
    cardSprite->setPosition(Vec2(400, 300)); // 初始位置
    this->addChild(cardSprite);

    // 启用拖动
    enableCardDrag(cardSprite, card);

    adjustHandPile();
}

void HandPileLayer::adjustHandPile()
{
    // 手牌中心位置
    Vec2 handCenter = Vec2(500, 150);
    float cardSpacing = 100.0f; // 卡牌间距
    float totalWidth = (hand.size() - 1) * cardSpacing;

    auto& newhand = CombatSystem::getInstance()->hand;
    CCLOG("now my hand has %d cards", newhand.size());
    for (size_t i = 0; i < newhand.size(); ++i)
    {
        // 假设每个手牌对应的精灵用 hand[i] 的地址作为唯一 tag
        auto cardSprite = this->getChildByTag(reinterpret_cast<intptr_t>(newhand[i].get()));
        if (cardSprite) {

            // 根据手牌数量调整位置，确保居中显示
            float xOffset = i * cardSpacing;

            Vec2 newPosition = handCenter + Vec2(xOffset, 0);

            //Vec2 newPosition = Vec2(400, 300);
            // 使用动作移动到新位置，显得更平滑
            cardSprite->runAction(MoveTo::create(0.2f, newPosition));
        }
    }
}


bool CombatScene::init()
{
    // 创建并设置背景图像
    auto background = cocos2d::Sprite::create("combatScene.png");
    if (background) {
        background->setContentSize(Size(1648, 1500)); // 设置卡牌大小
        background->setPosition(cocos2d::Vec2(750, 200));
        this->addChild(background);
    }
    else {
        CCLOG("Can't find: combatScene.png!");
    }

    // 创建抽牌堆图标
    auto drawPileIcon = cocos2d::Sprite::create("drawPileIcon.png");
    if (drawPileIcon) {
        drawPileIcon->setPosition(cocos2d::Vec2(100, 100));
        this->addChild(drawPileIcon);
    }
    else {
        CCLOG("Can't find: drawPileIcon.png");
    }

    // 创建弃牌堆图标
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
    //创建回合结束按钮,当点击回合结束按钮之后，丢弃所有的卡牌
    auto endTurnButton = HoverButton::create("endTurnButton.png", "endTurnButtonGlow.png", "endTurnButton.png");
    endTurnButton->setTitleText(u8"回合结束");
    endTurnButton->setScale(1.5f);
    endTurnButton->setTitleFontSize(20);
    endTurnButton->setPosition(Vec2(screenSize.width - 200, screenSize.height - 650));

    //测试使用，回合开始按钮，当点击回合开始按钮之后，
    auto startTurnButton = HoverButton::create("endTurnButton.png", "endTurnButtonGlow.png", "endTurnButton.png");
    startTurnButton->setTitleText(u8"回合开始");
    startTurnButton->setScale(1.5f);
    startTurnButton->setTitleFontSize(20);
    startTurnButton->setPosition(Vec2(screenSize.width - 1200, screenSize.height - 650));

    // 结束回合按钮
    endTurnButton->addTouchEventListener([&](cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type) {
        if (type == cocos2d::ui::Widget::TouchEventType::ENDED) {
            if (isMyTurn)
            {
                CCLOG("End Turn clicked!%d", isMyTurn);  // 打印日志
            }
            isMyTurn = 0;
        }
        });

    // 开始回合按钮
    startTurnButton->addTouchEventListener([&](cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type) {
        if (type == cocos2d::ui::Widget::TouchEventType::ENDED) {
            if (!isMyTurn)
            {
                CCLOG("Start Turn clicked!%d", isMyTurn);  // 打印日志
                CombatSystem::getInstance()->drawCard(5);
            }
            isMyTurn = 1;
        }
        });
    this->addChild(endTurnButton);
    this->addChild(startTurnButton);


    //测试使用：创建一个能打出牌的区域，当卡牌被拖动到这个区域时被打出
    playArea = Rect(500, 500, 400, 200); // 设置打出区域
    auto playAreaNode = DrawNode::create();
    playAreaNode->drawRect(playArea.origin, playArea.origin + playArea.size, Color4F::GRAY);
    this->addChild(playAreaNode);

    this->addChild(HandPileLayer::getInstance());
    return true;
}

cocos2d::Scene* CombatScene::createScene() {
    auto scene = Scene::create();          // 创建一个空白场景
    auto layer = CombatScene::create();
    scene->addChild(layer);                // 将层添加到场景
    return scene;
}

/*#include "CombatScene.h"
#include "AudioPlayer.h"
#include "NodeConnection.h"
#include "Player.h"
#include "HeaderBar.h"
#include "HoverButton.h"
#include "CardLayer.h"
#include "EnergyBall.h"

using namespace cocos2d;

// 创建场景


bool CombatScene::init() {
    if (!Scene::init()) {
        return false;
    }

    //能量图像
    auto energyBall = EnergyBall::create();
    energyBall->setPosition(Vec2(200, 200)); // 设置位置
    energyBall->setEnergy(3, 3);             // 设置当前能量和总能量
    this->addChild(energyBall, 100);       // 添加到场景

    auto Button1 = HoverButton::create(
        "base.png",  // 默认图片
        "base.png",  // 按钮悬停时的图片
        "base.png"   // 按钮点击时的图片
    );
    // 设置按钮位置
    Button1->setScale(2.0f);
    Button1->setPosition(Vec2(100, 280));
    this->addChild(Button1,10000);
    // 添加按钮点击事件监听器
    Button1->addClickEventListener([=](Ref* sender) {
                    

        });

    auto Button2 = HoverButton::create(
        "base2.png",  // 默认图片
        "base2.png",  // 按钮悬停时的图片
        "base2.png"   // 按钮点击时的图片
    );

    // 设置按钮位置
    Button2->setScale(2.0f);
    Button2->setPosition(Vec2(1800, 280));
    this->addChild(Button2, 10000);

    // 添加按钮点击事件监听器
    Button2->addClickEventListener([=](Ref* sender) {
        // 执行 popScene 操作，返回上一个场景

        });

    
    Player* player = Player::getInstance();
    auto headbar = HeaderBar::create(player);
    headbar->setPosition(Vec2(0, 1150));          // 设置位置（在屏幕上部）
    this->addChild(headbar);
    headbar->setLocalZOrder(100);

    // 背景图像
    const auto background = Sprite::create("scene3.png");
    background->setPosition(Vec2(1100, 500));
    background->setScale(1.05f);
    this->addChild(background);

  
    

    return true;
   

}

*/
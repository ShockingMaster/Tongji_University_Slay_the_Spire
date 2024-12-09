#include "IncludeAll.h"

bool CombatScene::init() 
{
    // 创建并设置背景图像
    auto background = cocos2d::Sprite::create("combatScene.png");
    const cocos2d::Size screenSize = cocos2d::Director::getInstance()->getWinSize();
    if (background) 
    {
        background->setContentSize(Size(1.1 * screenSize.width, 1.5 * screenSize.height)); // 设置背景大小
        background->setPosition(cocos2d::Vec2(screenSize.width / 2 + 0.05 * screenSize.width, 
            screenSize.height / 2 - 0.25 * screenSize.height));
        this->addChild(background);
    }
    else{
        CCLOG("Can't find: combatScene.png!");
    }
    //创建能量标志
    auto energyLayer1 = Sprite::create("energyLayer1.png");
    if (energyLayer1) {
        energyLayer1->setContentSize(Size(200, 200));
        energyLayer1->setPosition(cocos2d::Vec2(0.0976525 * screenSize.width, 0.390625 * screenSize.height));
        this->addChild(energyLayer1);
    }
    else {
        CCLOG("Can't find: energyLayer1.png!");
    }
    auto energyLayer2 = Sprite::create("energyGreenVFX.png");
    if (energyLayer2) {
        energyLayer2->setContentSize(Size(120, 120));
        energyLayer2->setPosition(cocos2d::Vec2(0.0976525 * screenSize.width, 0.390625 * screenSize.height));
        this->addChild(energyLayer2);
    }
    else {
        CCLOG("Can't find: energyGreenVFX.png!");
    }

    // 创建显示能量的数字
    energyLabel = cocos2d::Label::createWithSystemFont(std::to_string(0) + "/" + std::to_string(0), "Arial", 24);
    energyLabel->setPosition(cocos2d::Vec2(0.0976525 * screenSize.width, 0.390625 * screenSize.height));  // 设置在能量图标的中心
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
                CCLOG("End Turn clicked!");  // 打印日志
                CombatSystem::getInstance()->endTurn(Player::getInstance());//执行玩家回合结束效果
            }
            isMyTurn = 0;
        }
        });

    // 开始回合按钮
    startTurnButton->addTouchEventListener([&](cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type) {
        if (type == cocos2d::ui::Widget::TouchEventType::ENDED ) {
            if (!isMyTurn)
            {
                CCLOG("Start Turn clicked!");  // 打印日志
                CombatSystem::getInstance()->startTurn(Player::getInstance());// 执行玩家回合开始效果
            }
            isMyTurn = 1;
        }
        });
    this->addChild(endTurnButton);
    this->addChild(startTurnButton);


    HandPileLayer::getInstance()->init();
    //测试使用：创建一个能打出牌的区域，当卡牌被拖动到这个区域时被打出
    playArea = Rect(screenSize.width / 2 - 0.15 * screenSize.width, screenSize.height / 2, 
        0.3 * screenSize.width, 0.3 * screenSize.height); // 设置打出区域
    auto playAreaNode = DrawNode::create();
    playAreaNode->drawRect(playArea.origin, playArea.origin + playArea.size, Color4F::GRAY);
    this->addChild(playAreaNode);

    this->addChild(HandPileLayer::getInstance());

    return true;
}

/*
* 函数名称：updateEnergyDisplay
* 功能：根据Player唯一实例的最大能量和当前能量进行更新
*/
void CombatScene::updateEnergyDisplay()
{
    const int currentEnergy = Player::getInstance()->getCurrentEnergy();
    const int maxEnergy = Player::getInstance()->getMaxEnergy();
    energyLabel->setString(std::to_string(currentEnergy) + "/" + std::to_string(maxEnergy));  // 更新标签文本为当前能量值
}


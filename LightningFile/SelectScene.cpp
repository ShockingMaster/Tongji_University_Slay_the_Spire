#include "IncludeAll.h"

SelectScene* SelectScene::instance_ = nullptr;

SelectScene* SelectScene::getInstance()
{
    if (instance_ == nullptr)
    {
        instance_ = new SelectScene();
    }
    return instance_;
}

bool SelectScene::init()
{

    // 创建并设置背景图像
    auto background = cocos2d::Sprite::create("combatScene.png");
    const cocos2d::Size screenSize = cocos2d::Director::getInstance()->getWinSize();
    if (background)
    {
        background->setContentSize(Size(1.1 * screenSize.width, 1.5 * screenSize.height)); // 设置背景大小
        background->setPosition(cocos2d::Vec2(screenSize.width / 2 + 0.05 * screenSize.width,
            screenSize.height / 2 - 0.25 * screenSize.height));
        background->setColor(Color3B(150, 150, 150)); // 调整颜色变暗
        this->addChild(background);
    }
    else {
        CCLOG("Can't find: combatScene.png!");
    }

    // 创建能量标志
    auto energyLayer1 = Sprite::create("energyLayer1.png");
    if (energyLayer1) {
        energyLayer1->setContentSize(Size(200, 200));
        energyLayer1->setPosition(cocos2d::Vec2(0.0976525 * screenSize.width, 0.390625 * screenSize.height));
        energyLayer1->setColor(Color3B(100, 100, 100)); // 调整颜色变暗
        this->addChild(energyLayer1);
    }
    else {
        CCLOG("Can't find: energyLayer1.png!");
    }

    auto energyLayer2 = Sprite::create("energyGreenVFX.png");
    if (energyLayer2) {
        energyLayer2->setContentSize(Size(120, 120));
        energyLayer2->setPosition(cocos2d::Vec2(0.0976525 * screenSize.width, 0.390625 * screenSize.height));
        energyLayer2->setOpacity(128); // 调整透明度变暗
        this->addChild(energyLayer2);
    }
    else {
        CCLOG("Can't find: energyGreenVFX.png!");
    }

    // 创建显示能量的数字
    energyLabel = cocos2d::Label::createWithSystemFont(std::to_string(0) + "/" + std::to_string(0), "Arial", 24);
    energyLabel->setPosition(cocos2d::Vec2(0.0976525 * screenSize.width, 0.390625 * screenSize.height));
    energyLabel->setColor(cocos2d::Color3B(100, 100, 100)); // 设置文字颜色变暗
    this->addChild(energyLabel);

    // 对能量进行更新
    updateEnergyDisplay();

    // 创建切换场景按钮
    auto switchSceneButton = HoverButton::create("endTurnButton.png", "endTurnButtonGlow.png", "endTurnButton.png");
    switchSceneButton->setTitleText(u8"切换场景");
    switchSceneButton->setScale(1.5f);
    switchSceneButton->setTitleFontSize(20);
    switchSceneButton->setPosition(Vec2(0.906525 * screenSize.width, 0.15625 * screenSize.height)); // 设置按钮位置

    SelectScene::getInstance()->switchSceneButton = switchSceneButton;
    switchSceneButton->setEnabled(HandPileLayer::getInstance()->canSwitchScene);
    switchSceneButton->setTouchEnabled(HandPileLayer::getInstance()->canSwitchScene);

    // 切换场景按钮事件
    switchSceneButton->addTouchEventListener([&](cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type) {
        if (type == cocos2d::ui::Widget::TouchEventType::ENDED) {
            CCLOG("Switch Scene clicked!");  // 打印日志

            HandPileLayer::getInstance()->card_num_select = 0;
            HandPileLayer::getInstance()->canSwitchScene = false;

            // 在此处执行切换场景的操作
            //切回原来卡牌的监听
            auto combatsystem = CombatSystem::getInstance();
            for (auto& card : combatsystem->hand) {
                HandPileLayer::getInstance()->switchToenableCardDrag(card);
            }
            HandPileLayer::getInstance() ->setSceneType(HandPileLayer::SceneType::SCENE_TYPE_1);
            CombatSystem::getInstance()->exhaustCard();
            HandPileLayer::getInstance()->exhaustCard();
            HandPileLayer::getInstance()->select_card_list.clear();
            CombatSystem::getInstance()->use_tem_card();
            auto delay = DelayTime::create(0.3f); 
            auto popSceneAction = CallFunc::create([=]() {
                Director::getInstance()->popScene();  
                });
            
            this->runAction(Sequence::create(delay, popSceneAction, nullptr));
            
        }
        });

    // 添加按钮到当前场景
    this->addChild(switchSceneButton);



    
    
    //改变原来的卡牌监听
    auto handPileLayer = HandPileLayer::getInstance();
    auto combatsystem = CombatSystem::getInstance();
    
    handPileLayer->setSceneType(HandPileLayer::SceneType::SCENE_TYPE_2);
    for (auto& card : combatsystem->hand) {
        HandPileLayer::getInstance()->switchToCardHighlight(card);
    }
    HandPileLayer::getInstance()->adjustHandPile();
    this->addChild(handPileLayer);
    
    return true;
    
}

void SelectScene::update_button() {

    switchSceneButton->setEnabled(HandPileLayer::getInstance()->canSwitchScene);
    switchSceneButton->setTouchEnabled(HandPileLayer::getInstance()->canSwitchScene);
}


/*
* 函数名称：updateEnergyDisplay
* 功能：根据Player唯一实例的最大能量和当前能量进行更新
*/
void SelectScene::updateEnergyDisplay()
{
    const int currentEnergy = Player::getInstance()->getCurrentEnergy();
    const int maxEnergy = Player::getInstance()->getMaxEnergy();
    energyLabel->setString(std::to_string(currentEnergy) + "/" + std::to_string(maxEnergy));  // 更新标签文本为当前能量值
}
#include "IncludeAll.h"
#include "string"


cocos2d::Scene* CombatScene::createScene()
{
    auto scene = Scene::create();          // 创建一个空白场景
    auto layer = CombatScene::create();     
    scene->addChild(layer);                // 将层添加到场景
    return scene;
}

void CombatScene::onEnter() {
    Scene::onEnter();

    // 初始检查
    this->scheduleOnce([this](float dt) {
        checkScene(); // 调用检查函数
        }, 0.5f, "CheckSceneAfterDelay");
}

void CombatScene::checkScene() {
    auto currentScene = Director::getInstance()->getRunningScene();

    if (currentScene) {
        CCLOG("Current running scene: %s", typeid(*currentScene).name());
    }
    else {
        CCLOG("No running scene.");
    }

    // 尝试将当前场景转换为 CombatScene
    CombatScene* scene = dynamic_cast<CombatScene*>(currentScene);
    if (scene) {
        CCLOG("Successfully cast to CombatScene.");
        // 调用 updateDisplay
        if (creatureLayer) {
            creatureLayer->updateDisplay();
        }
    }
    else {
        CCLOG("Failed to cast to CombatScene. Retrying...");
        // 如果失败了，隔一段时间再尝试
        this->scheduleOnce([this](float dt) {
            checkScene(); // 递归调用自己继续检查
            }, 0.05f, "RetryCheckScene");
    }
}

// 在类的定义中添加一个成员变量来存储当前显示的怪物图标
std::vector<cocos2d::Sprite*> combatSprites;
std::vector<cocos2d::Label*> combatSprites_num;

void CombatScene::intent_change(const cocos2d::Size screenSize) {
    // 1. 删除之前显示的所有图标
    for (auto sprite : combatSprites) {
        sprite->removeFromParent();  // 从场景中移除
    }
    combatSprites.clear();  // 清空容器
    for (auto sprite : combatSprites_num) {
        sprite->removeFromParent();  // 从场景中移除
    }
    combatSprites_num.clear();  // 清空容器

    // 2. 添加新的怪物图标
    auto combat = CombatSystem::getInstance();
    for (int i = 0; i < combat->Monsters_.size(); i++) {
        auto monster = static_pointer_cast<Monster>(combat->Monsters_[i]);
        std::string png_path = monster->Intention_display();

        // 计算图标位置
        float rectX = 0.765 * screenSize.width;
        float rectY = 0.72 * screenSize.height;

        // 计算图标位置：怪物区域的上方
        float spriteX = rectX+(i- (combat->Monsters_.size()-1)/2.0)* 0.12 * screenSize.width;  // 中心位置
        float spriteY = rectY; // 上方位置，偏移一个矩形高度
        



        auto sprite = cocos2d::Sprite::create(png_path);
        if (png_path=="attack.png") {
            string num = monster->num;
            auto label = cocos2d::Label::createWithSystemFont(num, "Arial", 24);
            if (label) {
                // 设置Label的位置，放置在sprite右边
                label->setPosition(cocos2d::Vec2(spriteX + 0.02 * screenSize.width, spriteY));
                // 将Label添加到场景中
                this->addChild(label, 102);  // 设置层级为102，确保在sprite上方
                combatSprites_num.push_back(label);
            }
        }
        if (sprite) {
            // 设置Sprite位置，将其放置在怪物区域的上方
            sprite->setPosition(cocos2d::Vec2(spriteX, spriteY));
            this->addChild(sprite, 101);  // 将Sprite添加到场景中，确保在DrawNode之上
            // 将新添加的Sprite保存到容器中
            combatSprites.push_back(sprite);
        }
    }
}



bool CombatScene::init() 
{
    // 先进行战斗系统初始化
    CombatSystem::getInstance()->init();
    const cocos2d::Size screenSize = cocos2d::Director::getInstance()->getWinSize();
    auto combat = CombatSystem::getInstance();
    for (int i = 0;i < combat->Monsters_.size();i++)
    {
        auto monster = static_pointer_cast<Monster>(combat->Monsters_[i]);

        // 设定怪物区域
        static_pointer_cast<Monster>(combat->Monsters_[i])->
            setRect(cocos2d::Vec2(0.83 * screenSize.width + combat->Monsters_.size() * 0.12207 * screenSize.width / 2 - (i + 1) * 0.12207 * screenSize.width, 0.526315 * screenSize.height), 0.09765 * screenSize.width, 0.315789 * screenSize.height);
        cocos2d::Rect monsterRect = monster->getRect();

        // 仅测试使用，画出可打出卡牌区域 
        auto drawNode = cocos2d::DrawNode::create();
        // 用红色绘制矩形
        drawNode->drawRect(monsterRect.origin, cocos2d::Vec2(monsterRect.origin.x + monsterRect.size.width, monsterRect.origin.y + monsterRect.size.height), cocos2d::Color4F(1, 0, 0, 1));
        this->addChild(drawNode,100);  // 将 DrawNode 添加到场景中
    }
    CombatScene::intent_change(screenSize);

    auto player = EventSystem::getInstance();
    headbar = HeaderBar::create(player);
    headbar->setPosition(Vec2(0, 750));          // 设置位置（在屏幕上部）
    this->addChild(headbar,100);

    // 创建并设置背景图像
    auto background = cocos2d::Sprite::create("combatScene.png");
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
                for (auto& card : CombatSystem::getInstance()->hand) {
                    card->takeeffectonturnend(card);
                }
                CombatSystem::getInstance()->endTurn(Player::getInstance());//执行玩家回合结束效果
                //测试
                
                for (int i = 0; i < CombatSystem::getInstance()->Monsters_.size(); i++)
                {
                    
                    auto monster = static_pointer_cast<Monster>(CombatSystem::getInstance()->Monsters_[i]);

                    monster->takeEffect();
                }
                const cocos2d::Size screenSize = cocos2d::Director::getInstance()->getWinSize();
                CombatScene::intent_change(screenSize);
                
                
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


    // 在你的场景或 Layer 中创建战斗结束按钮
    auto endBattleButton = cocos2d::ui::Button::create("endTurnButton.png", "endTurnButton.png");  // 设置按钮的常态和按下态图片
    endBattleButton->setPosition(cocos2d::Vec2(400, 800));  // 设置按钮位置
    endBattleButton->setTitleText("End Battle");  // 设置按钮文本
    // 添加按钮到当前层
    this->addChild(endBattleButton);
    // 设置触摸事件监听器
    endBattleButton->addTouchEventListener([&](cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type) {
        if (type == cocos2d::ui::Widget::TouchEventType::ENDED) {
            CCLOG("End Battle button clicked!");  // 打印日志
            CombatSystem::getInstance()->endTurn(Player::getInstance());
            // 结束战斗，弹出当前场景并返回上一个场景
            Director::getInstance()->popScene();
        }
        });



    HandPileLayer::getInstance()->init();
    //测试使用：创建一个能打出牌的区域，当卡牌被拖动到这个区域时被打出
    playArea = Rect(screenSize.width / 2 - 0.15 * screenSize.width, screenSize.height / 2,
        0.3 * screenSize.width, 0.3 * screenSize.height);
    auto playAreaNode = DrawNode::create();
    playAreaNode->drawRect(playArea.origin, playArea.origin + playArea.size, Color4F::GRAY);
    this->addChild(playAreaNode);

    this->addChild(HandPileLayer::getInstance());

    // 创建
    creatureLayer = CreatureLayer::create(CombatSystem::getInstance()->Monsters_);
    this->addChild(creatureLayer);
    
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


#include "CombatFacade.h"
#include "IncludeAll.h"
#include "string"

cocos2d::Scene* CombatScene::createScene()
{
    auto scene = Scene::create();
    auto layer = CombatScene::create();
    scene->addChild(layer);
    return scene;
}

void CombatScene::onEnter()
{
    Scene::onEnter();

    this->scheduleOnce([this](float dt) {
        checkScene();

        if (isMyTurn == 0) {
            isMyTurn = 1;
            // Use Facade instead of direct CombatSystem calls
            combatFacade_->startCombat();
            creatureLayer->updateDisplay();
            this->updateEnergyDisplay();
        }
    },
        0.5f, "CheckSceneAfterDelay");
}

void CombatScene::checkScene()
{
    auto currentScene = Director::getInstance()->getRunningScene();

    if (currentScene) {
        CCLOG("Current running scene: %s", typeid(*currentScene).name());
    } else {
        CCLOG("No running scene.");
    }

    CombatScene* scene = dynamic_cast<CombatScene*>(currentScene);
    if (scene) {
        CCLOG("Successfully cast to CombatScene.");
        if (creatureLayer) {
            creatureLayer->updateDisplay();
        }
    } else {
        CCLOG("Failed to cast to CombatScene. Retrying...");
        this->scheduleOnce([this](float dt) {
            checkScene();
        },
            0.05f, "RetryCheckScene");
    }
}

bool CombatScene::init()
{
    // Create Facade instance
    combatFacade_ = CombatFacade::create();
    if (!combatFacade_) {
        CCLOG("Failed to create CombatFacade!");
        return false;
    }

    const cocos2d::Size screenSize = cocos2d::Director::getInstance()->getWinSize();

    // Use Facade to get monsters for positioning
    auto monsters = combatFacade_->getMonsters();
    for (int i = 0; i < monsters.size(); i++) {
        auto monster = std::dynamic_pointer_cast<Monster>(monsters[i]);
        if (monster) {
            monster->setRect(
                cocos2d::Vec2(0.83 * screenSize.width + monsters.size() * 0.12207 * screenSize.width / 2 - (i + 1) * 0.12207 * screenSize.width, 0.526315 * screenSize.height),
                0.09765 * screenSize.width,
                0.315789 * screenSize.height);
        }
    }

    auto player = EventSystem::getInstance();
    headbar = HeaderBar::create(player);
    headbar->setPosition(Vec2(0, 750));
    this->addChild(headbar, 1);

    // Background setup
    auto background = cocos2d::Sprite::create("combatScene.png");
    if (background) {
        background->setContentSize(Size(1.1 * screenSize.width, 1.5 * screenSize.height));
        background->setPosition(cocos2d::Vec2(screenSize.width / 2 + 0.05 * screenSize.width,
            screenSize.height / 2 - 0.25 * screenSize.height));
        this->addChild(background);
    } else {
        CCLOG("Can't find: combatScene.png!");
    }

    // Energy display sprites
    auto energyLayer1 = Sprite::create("energyLayer1.png");
    if (energyLayer1) {
        energyLayer1->setContentSize(Size(200, 200));
        energyLayer1->setPosition(cocos2d::Vec2(0.0976525 * screenSize.width, 0.390625 * screenSize.height));
        this->addChild(energyLayer1);
    }

    auto energyLayer2 = Sprite::create("energyGreenVFX.png");
    if (energyLayer2) {
        energyLayer2->setContentSize(Size(120, 120));
        energyLayer2->setPosition(cocos2d::Vec2(0.0976525 * screenSize.width, 0.390625 * screenSize.height));
        this->addChild(energyLayer2);
    }

    // Energy label
    energyLabel = cocos2d::Label::createWithSystemFont("0/0", "Arial", 24);
    energyLabel->setPosition(cocos2d::Vec2(0.0976525 * screenSize.width, 0.390625 * screenSize.height));
    energyLabel->setColor(cocos2d::Color3B::BLACK);
    this->addChild(energyLabel);

    updateEnergyDisplay();

    isMyTurn = 0;

    // End Turn Button - now uses Facade
    auto endTurnButton = HoverButton::create("endTurnButton.png", "endTurnButtonGlow.png", "endTurnButton.png");
    endTurnButton->setTitleText(u8"回合结束");
    endTurnButton->setScale(1.5f);
    endTurnButton->setTitleFontSize(20);
    endTurnButton->setPosition(Vec2(0.906525 * screenSize.width, 0.25625 * screenSize.height));

    // Simplified: Use Facade for end turn logic
    endTurnButton->addTouchEventListener([this](cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type) {
        if (type == cocos2d::ui::Widget::TouchEventType::ENDED) {
            if (isMyTurn) {
                CCLOG("End Turn clicked!");
                isMyTurn = 0;

                // Single Facade call replaces multiple CombatSystem calls
                combatFacade_->endPlayerTurn();

                // Update UI
                creatureLayer->updateDisplay();
                this->updateEnergyDisplay();
            }
            isMyTurn = 1;
        }
    });
    this->addChild(endTurnButton);

    // End Battle Button
    auto endBattleButton = cocos2d::ui::Button::create("endTurnButton.png", "endTurnButton.png");
    endBattleButton->setPosition(cocos2d::Vec2(400, 800));
    endBattleButton->setTitleText("End Battle");
    this->addChild(endBattleButton);

    endBattleButton->addTouchEventListener([this](cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type) {
        if (type == cocos2d::ui::Widget::TouchEventType::ENDED) {
            CCLOG("End Battle button clicked!");
            combatFacade_->endCombat();
            isMyTurn = 0;
            Director::getInstance()->popScene();
        }
    });

    HandPileLayer::getInstance()->init();

    // Play area
    playArea = Rect(screenSize.width / 2 - 0.15 * screenSize.width, screenSize.height / 2,
        0.3 * screenSize.width, 0.3 * screenSize.height);
    auto playAreaNode = DrawNode::create();
    playAreaNode->drawRect(playArea.origin, playArea.origin + playArea.size, Color4F::GRAY);
    this->addChild(playAreaNode);

    this->addChild(HandPileLayer::getInstance());

    // Creature layer - use Facade to get monsters
    creatureLayer = CreatureLayer::create(combatFacade_->getMonstersAsCreatures());
    this->addChild(creatureLayer);

    return true;
}

void CombatScene::updateEnergyDisplay()
{
    // Use Facade to get player info
    auto player = combatFacade_->getPlayer();
    if (player) {
        const int currentEnergy = player->getCurrentEnergy();
        const int maxEnergy = player->getMaxEnergy();
        energyLabel->setString(std::to_string(currentEnergy) + "/" + std::to_string(maxEnergy));
    }
}

#pragma once
#include "cocos2d.h"
#include <memory>
#include <queue>
#include <vector>
using namespace cocos2d;

class Card;
class CombatFacade;
class Creature;
class CreatureLayer;
class HeaderBar;

class CombatScene : public cocos2d::Scene {
private:
    cocos2d::Rect playArea;
    cocos2d::Label* energyLabel;

    // Facade instance - single entry point for combat operations
    std::unique_ptr<CombatFacade> combatFacade_;

public:
    static cocos2d::Scene* createScene();
    void onEnter();
    void checkScene();
    bool init();

    CreatureLayer* creatureLayer;
    HeaderBar* headbar;

    int isMyTurn;

    static void onEndTurnClicked(cocos2d::Ref* sender);

    void updateEnergyDisplay();

    cocos2d::Rect getplayArea()
    {
        return playArea;
    }
    CREATE_FUNC(CombatScene);
};
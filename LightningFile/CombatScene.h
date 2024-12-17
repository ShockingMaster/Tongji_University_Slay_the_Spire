#pragma once
#include "cocos2d.h"
#include <memory>
#include <queue>
#include <vector>
using namespace cocos2d;
class Card;
class CombatSystem;
class CombatDeck;
class Creature;
class CreatureLayer;
class HeaderBar;
class CombatScene : public cocos2d::Scene
{
private:
    //测试使用：
    cocos2d::Rect playArea;
    cocos2d::Label* energyLabel;
public:
    static cocos2d::Scene* createScene();
    void onEnter();
    void checkScene();

    void intent_change(const cocos2d::Size screenSize);

    bool init();

    CreatureLayer* creatureLayer;
    HeaderBar* headbar;

    //测试使用
    int isMyTurn;

    static void onEndTurnClicked(cocos2d::Ref* sender);//当回合结束按钮被点击时

    void updateEnergyDisplay();//更新能量显示

    cocos2d::Rect getplayArea()
    {
        return playArea;
    }
    CREATE_FUNC(CombatScene);
};
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

class SelectScene : public cocos2d::Scene
{
private:
    //����ʹ�ã�
    cocos2d::Rect playArea;
    cocos2d::Label* energyLabel;
public:
    bool init();

    //����ʹ��
    int isMyTurn;

    static void onEndTurnClicked(cocos2d::Ref* sender);//���غϽ�����ť�����ʱ

    void updateEnergyDisplay();//����������ʾ

    cocos2d::Rect getplayArea()
    {
        return playArea;
    }
    CREATE_FUNC(SelectScene);
};
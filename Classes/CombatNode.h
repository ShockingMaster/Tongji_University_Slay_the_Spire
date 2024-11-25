#pragma once
#include "CombatDeck.h"
class Player;
class CombatNode//战斗节点类
{
public:
    CombatDeck combat_deck_;
    int init();//完成场景的初始化,同时进行角色和敌人的创建
    int turn;//进行回合数的记录，通过奇偶分辨是敌方回合还是己方回合 
};
#pragma once
#include<vector>
#include "Creature.h"
using namespace std;
class Potion;
class Player :public Creature
{
public:
    static std::shared_ptr<Player> getInstance();

    virtual void start_turn();     //得牌，得能量，失去所有格挡，进行一次buff结算

    virtual void end_turn();       //失去所有卡牌，失去所有能量，进行一次buff结算

    vector<Potion*> potions_;

    void addPotion(Potion* potion);//需要调用本体的addPotion()

    void init();                   //每场战斗开始时对角色进行初始化
private:
    static std::shared_ptr<Player> instance_;
};


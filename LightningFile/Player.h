#pragma once
#include<vector>
#include "Creature.h"
using namespace std;
class Potion;
class Player :public Creature
{
public:
    static std::shared_ptr<Player> getInstance();

    void startTurn();          //得牌，得能量，失去所有格挡，进行一次buff结算

    void endTurn();            //失去所有卡牌，失去所有能量，进行一次buff结算

    int getMaxEnergy() const {         //返回角色最大能量
        return maxEnergy_;
    }          

    int getCurrentEnergy() const {     //返回角色当前有的能量
        return currentEnergy_;
    }
    void energyChange(int changeValue);//对角色的能量进行修改

    vector<Potion*> potions_;

    void addPotion(Potion* potion);    //需要调用本体的addPotion()

    void init();                       //每场战斗开始时对角色进行初始化
private:
    static std::shared_ptr<Player> instance_;
    int maxEnergy_;
    int currentEnergy_;
};


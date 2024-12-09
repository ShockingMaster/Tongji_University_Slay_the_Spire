#pragma once
#include<vector>
#include "Creature.h"
using namespace std;
class Potion;
class Player :public Creature
{
public:
    static std::shared_ptr<Player> getInstance();

    int getMaxEnergy() const {         //返回角色最大能量
        return maxEnergy_;
    }          

    int getCurrentEnergy() const {     //返回角色当前有的能量
        return currentEnergy_;
    }
    void energyChange(int changeValue);//对角色的能量进行修改

    void init();                       //每场战斗开始时对角色进行初始化

    virtual ~Player() {};
private:
    static std::shared_ptr<Player> instance_;
    int maxEnergy_;
    int currentEnergy_;
};


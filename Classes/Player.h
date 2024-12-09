#pragma once
#include<vector>
#include "Creature.h"
using namespace std;
class Potion;

class Player : public Creature
{
public:
    
    string character_;
    int coins_;  //金币数量
    static Player* getInstance();//返回唯一实例
    virtual void start_turn();//得牌，得能量，失去所有格挡，进行一次buff结算
    virtual void end_turn();//失去所有卡牌，失去所有能量，进行一次buff结算
    vector<Potion*> potions_;
    void addPotion(Potion* potion);//需要调用本体的addPotion()
    void init();//每场战斗开始时对角色进行初始化
    void addEnergy(int numeric_value_);
    int getEnergyValue();
    int getMaxEnergyValue();
private:
    static Player* instance_;
    int energy_;
    int maxEnergy_;
};
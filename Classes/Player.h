#pragma once
#include<vector>
#include "Creature.h"
#include "Card.h"
#include "Relic.h"
using namespace std;
class Potion;

class Player : public Creature
{
public:
    vector<Card*> cards_= { NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL};       // 当前拥有的卡组
    string character_;          //当前角色
    int coins_;                 //金币数量
    static Player* getInstance();//返回唯一实例
    virtual void start_turn();//得牌，得能量，失去所有格挡，进行一次buff结算
    virtual void end_turn();//失去所有卡牌，失去所有能量，进行一次buff结算
    vector<Potion*> potions_ = { NULL,NULL,NULL };
    vector<Relic*> relics_={ NULL,NULL,NULL,NULL,NULL,NULL};
    void addPotion(Potion* potion);//需要调用本体的addPotion()
    void init();//每场战斗开始时对角色进行初始化
private:
    static Player* instance_;
    
};
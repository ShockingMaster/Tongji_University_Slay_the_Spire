#pragma once
#include<vector>
#include<string>
using namespace std;
class Creature;
class Card
{
    string name_;
    string description_;
    int energy_cost_;//卡牌消耗能量
    int money_cost_;//商店购买价格
    int rarity_;//稀有度
    bool is_enabled_;//这个时候卡牌可以被打出
    int type_;//卡牌类型
    int trigger_times_;//对于有的卡牌，效果会触发多次
    void take_effect(Creature* target);
    void take_effect(vector<Creature*> target);
};
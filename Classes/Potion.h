#pragma once
#include<vector>
#include<string>
using namespace std;
class Creature;
class Potion
{
public:
    string name_;
    string description_;
    int numeric_value_;//药水的数值
    int money_cost_;//商店的花费
    int rarity_;//稀有度
    bool is_enabled_;//有的药水只有在自己的回合内能使用，但是有的药水全局都可以使用
    virtual void take_effect(Creature* target);//需要给定一个使用目标
    virtual void take_effect(vector<Creature*> target);//如果有多个目标的话，给定一个vector
    void show_info();//当鼠标置于药水上时，会展示药水的基本信息
};
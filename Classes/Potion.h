#pragma once
#include <vector>
#include <string>
#include <memory> // 引入智能指针支持
using namespace std;

class Creature;

class Potion {
public:
    Potion(string name, string description, int numeric_value, int money_cost, int rarity, bool is_enabled) :
        name_(name),
        description_(description),
        numeric_value_(numeric_value),
        money_cost_(money_cost),
        rarity_(rarity),
        is_enabled_(is_enabled)
    {};


    // 使用智能指针：目标是Creature类的shared_ptr
    virtual void take_effect();  // 处理单个目标
    string name_;
    string description_;
    int numeric_value_;  // 药水的数值
    int money_cost_;     // 商店的花费
    int rarity_;         // 稀有度
    bool is_enabled_;    // 有的药水只有在自己的回合内能使用，有的药水全局都可以使用
};
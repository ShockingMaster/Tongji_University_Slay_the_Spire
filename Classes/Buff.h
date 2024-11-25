#pragma once
#include<iostream>
using namespace std;
class Creature;
class Buff
{
public:
    string name_;
    string description_;
    int duration_;//buff持续时间
    int trigger_type_;//buff触发方式
    int priotity_;//buff触发优先级 
    bool is_stackable_;//有的buff可以叠加
    int numeric_value_;//buff的数值，比如力量的数值，精准的数值，无实体的数值
    Buff(string name, string description, int trigger_type, int duration, int priotity);
    virtual void take_effect(Creature* target);
    virtual void take_effect(int& numeric_value);
    virtual ~Buff();
    bool operator<(const Buff& other) const;//比较两个buff的优先级
};
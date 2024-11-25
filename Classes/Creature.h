#pragma once
#include<vector>
#include<iostream>
using namespace std;
class Buff;
class Creature
{
public:
    string name_;
    string description_;
    int health_;
    int block_;
    int can_end_turn_;
    vector<Buff*> buffs_;
    void updateHealth();//需要调用本体的updateHealth()
    void updateBuff();//需要调用本体的updateBuff()
    void updateblock();
    void addBuff(Buff* buff);
    void takeDamage(int numeric_value);//这个是即将受到伤害时进行的效果判定
    void healthLoss(int numeric_value);//这个之前还要进行一次modifyValue判定
    //void buffSettle(Creature* target, int TriggerType, int& numeric_value);//如果说需要对数值进行修改
    void modifyValue(Creature* target, int TriggerType, int& numeric_value);
    void buffSettle(Creature* target, int TriggerType);//如果不需要对数值进行修改
    void show_info();
    void take_damage(int damage);
    virtual void start_turn() = 0;
    virtual void end_turn() = 0;
};
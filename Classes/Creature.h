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
    void updateHealth();//��Ҫ���ñ����updateHealth()
    void updateBuff();//��Ҫ���ñ����updateBuff()
    void updateblock();
    void addBuff(Buff* buff);
    void takeDamage(int numeric_value);//����Ǽ����ܵ��˺�ʱ���е�Ч���ж�
    void healthLoss(int numeric_value);//���֮ǰ��Ҫ����һ��modifyValue�ж�
    //void buffSettle(Creature* target, int TriggerType, int& numeric_value);//���˵��Ҫ����ֵ�����޸�
    void modifyValue(Creature* target, int TriggerType, int& numeric_value);
    void buffSettle(Creature* target, int TriggerType);//�������Ҫ����ֵ�����޸�
    void show_info();
    void take_damage(int damage);
    virtual void start_turn() = 0;
    virtual void end_turn() = 0;
};
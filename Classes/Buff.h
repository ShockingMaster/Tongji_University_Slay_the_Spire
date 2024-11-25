#pragma once
#include<iostream>
using namespace std;
class Creature;
class Buff
{
public:
    string name_;
    string description_;
    int duration_;//buff����ʱ��
    int trigger_type_;//buff������ʽ
    int priotity_;//buff�������ȼ� 
    bool is_stackable_;//�е�buff���Ե���
    int numeric_value_;//buff����ֵ��������������ֵ����׼����ֵ����ʵ�����ֵ
    Buff(string name, string description, int trigger_type, int duration, int priotity);
    virtual void take_effect(Creature* target);
    virtual void take_effect(int& numeric_value);
    virtual ~Buff();
    bool operator<(const Buff& other) const;//�Ƚ�����buff�����ȼ�
};
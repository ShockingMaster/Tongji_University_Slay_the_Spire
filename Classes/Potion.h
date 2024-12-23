#pragma once
#include <vector>
#include <string>
#include <memory> // ��������ָ��֧��
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


    // ʹ������ָ�룺Ŀ����Creature���shared_ptr
    virtual void take_effect();  // ������Ŀ��
    string name_;
    string description_;
    int numeric_value_;  // ҩˮ����ֵ
    int money_cost_;     // �̵�Ļ���
    int rarity_;         // ϡ�ж�
    bool is_enabled_;    // �е�ҩˮֻ�����Լ��Ļغ�����ʹ�ã��е�ҩˮȫ�ֶ�����ʹ��
};

#pragma once
#include <iostream>
#include "Enum.h"
using namespace std;
class CombatSystem;
class Creature;
class Card;
class Buff
{
public:
    string name_;
    string description_;
    int stack_type_;                                                   //���ӷ�ʽ������ʱ�䣨DURATION������Ч��������EFFECT_LAYERS��
    int duration_=0;                                                     //buff����ʱ��
    int trigger_type_;                                                 //buff������ʽ
    int priority_;                                                     //buff�������ȼ�
    bool is_stackable_;                                                //buff�Ƿ��ܱ�����
    bool is_positive_;                                                 //����������buff
    int effect_layers=0;                                                //buff��Ч������
    Buff(string name, string description, int duration, int priority, bool is_positive, int effect_layers,
        bool is_stackable = YES, int stack_type = DURATION)
        :name_(name),
        description_(description),
        priority_(priority),
        is_positive_(is_positive),
        effect_layers(effect_layers),
        is_stackable_(is_stackable),
        stack_type_(stack_type) {};
    virtual void onCardPlayed(std::shared_ptr<Card> card) {};          //�ڴ��һ����ʱ����

    virtual void onCombatStart(std::shared_ptr<Creature> creature) {};

    virtual void onCombatEnd(std::shared_ptr<Creature> creaure) {};

    virtual void onTurnStart() {};                                     //�ڻغϿ�ʼʱ����

    virtual void onTurnEnd() {};                                       //�ڻغϽ���ʱ����

    virtual void onAttack(int& numeric_value_, std::string cardName = "",
        std::shared_ptr<Creature> user = nullptr, std::shared_ptr<Creature> target = nullptr) {};      //�ڽ��й���ʱ����,��ҪӦ�����޸ģ����ˡ�����������

    virtual void onGetBlock(int& numeric_value_) {};                   //�ڵõ���ʱ��������ҪӦ�����޸ģ�����

    virtual void onAttacked(int& numeric_value_, std::shared_ptr<Creature> user, std::shared_ptr<Creature> target) {};

    virtual void onTakeDamage(int& numeric_value_) {};

    virtual void onGetEnergy(int& numeric_value_) {};

    virtual void onLoseBlock(int& numeric_value_) {};

    virtual void onLoseHealth(int& numeric_value_) {};

    virtual void onAddHealth(int& numeric_value_) {};

    virtual void addBuff(std::shared_ptr<Buff> buff, int& numeric_value) {};

    virtual void getBuff() {};

    virtual void onShuffleDeck() {};

    virtual void onDrawCard(int& num) {};

    virtual void onExhaustCard() {};

    virtual ~Buff();

    bool operator<(const Buff& other) const;                           //�Ƚ�����buff�����ȼ�
private:

};

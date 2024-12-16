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
    Buff(string name, string description, int duration, int priority, bool is_positive, 
        bool is_stackable = YES, int stack_type = DURATION)
        :name_(name),
        description_(description),
        priority_(priority) ,
        is_positive_(is_positive),
        is_stackable_(is_stackable),
        stack_type_(stack_type){};

    string name_;
    string description_;
    int stack_type_;                                                   //���ӷ�ʽ������ʱ�����Ч������
    int duration_;                                                     //buff����ʱ��
    int trigger_type_;                                                 //buff������ʽ
    int priority_;                                                     //buff�������ȼ�
    bool is_stackable_;                                                //buff�Ƿ��ܱ�����
    bool is_positive_;                                                 //����������buff
    int numeric_value_;                                                //buff��Ч������
    
    virtual void onCardPlayed(std::shared_ptr<Card> card) {};          //�ڴ��һ����ʱ����
    
    virtual void onTurnStart() {};                                     //�ڻغϿ�ʼʱ����
    
    virtual void onTurnEnd() {};                                       //�ڻغϽ���ʱ����
   
    virtual void onAttack(int& numeric_value_, std::string cardName = "", 
        std::shared_ptr<Creature> user = nullptr, std::shared_ptr<Creature> target = nullptr) {};      //�ڽ��й���ʱ����,��ҪӦ�����޸ģ����ˡ�����������
    
    virtual void onGetBlock(int& numeric_value_) {};                   //�ڵõ���ʱ��������ҪӦ�����޸ģ�����
    
    virtual void onAttacked(int& numeric_value_, std::shared_ptr<Creature>, std::shared_ptr<Creature>) {};
    
    virtual void onTakeDamage(int& numeric_value_) {};

    virtual void onGetEnergy(int& numeric_value_) {};
    
    virtual void onLoseBlock(int& numeric_value_) {};
    
    virtual void onLoseHealth(int& numeric_value_) {};

    virtual void onAddHealth(int& numeric_value_) {};
    
    virtual void getBuff() {};

    virtual void onShuffleDeck() {};

    virtual void onDrawCard(int& num) {};
    
    virtual void onExhaustCard() {};
    
    virtual ~Buff();
    
    bool operator<(const Buff& other) const;                           //�Ƚ�����buff�����ȼ�
private:

};



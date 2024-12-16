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
    int stack_type_;                                                   //叠加方式：持续时间或是效果层数
    int duration_;                                                     //buff持续时间
    int trigger_type_;                                                 //buff触发方式
    int priority_;                                                     //buff触发优先级
    bool is_stackable_;                                                //buff是否能被叠加
    bool is_positive_;                                                 //区分正负面buff
    int numeric_value_;                                                //buff的效果层数
    
    virtual void onCardPlayed(std::shared_ptr<Card> card) {};          //在打出一张牌时触发
    
    virtual void onTurnStart() {};                                     //在回合开始时触发
    
    virtual void onTurnEnd() {};                                       //在回合结束时触发
   
    virtual void onAttack(int& numeric_value_, std::string cardName = "", 
        std::shared_ptr<Creature> user = nullptr, std::shared_ptr<Creature> target = nullptr) {};      //在进行攻击时触发,主要应用于修改：易伤、虚弱、力量
    
    virtual void onGetBlock(int& numeric_value_) {};                   //在得到格挡时触发，主要应用于修改：敏捷
    
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
    
    bool operator<(const Buff& other) const;                           //比较两个buff的优先级
private:

};



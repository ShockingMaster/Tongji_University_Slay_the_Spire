
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
    int stack_type_;                                                   //叠加方式：持续时间（DURATION）或是效果层数（EFFECT_LAYERS）
    int duration_=0;                                                     //buff持续时间
    int trigger_type_;                                                 //buff触发方式
    int priority_;                                                     //buff触发优先级
    bool is_stackable_;                                                //buff是否能被叠加
    bool is_positive_;                                                 //区分正负面buff
    int effect_layers=0;                                                //buff的效果层数
    Buff(string name, string description, int duration, int priority, bool is_positive, int effect_layers,
        bool is_stackable = YES, int stack_type = DURATION)
        :name_(name),
        description_(description),
        priority_(priority),
        is_positive_(is_positive),
        effect_layers(effect_layers),
        is_stackable_(is_stackable),
        stack_type_(stack_type) {};
    virtual void onCardPlayed(std::shared_ptr<Card> card) {};          //在打出一张牌时触发

    virtual void onCombatStart(std::shared_ptr<Creature> creature) {};

    virtual void onCombatEnd(std::shared_ptr<Creature> creaure) {};

    virtual void onTurnStart() {};                                     //在回合开始时触发

    virtual void onTurnEnd() {};                                       //在回合结束时触发

    virtual void onAttack(int& numeric_value_, std::string cardName = "",
        std::shared_ptr<Creature> user = nullptr, std::shared_ptr<Creature> target = nullptr) {};      //在进行攻击时触发,主要应用于修改：易伤、虚弱、力量

    virtual void onGetBlock(int& numeric_value_) {};                   //在得到格挡时触发，主要应用于修改：敏捷

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

    bool operator<(const Buff& other) const;                           //比较两个buff的优先级
private:

};

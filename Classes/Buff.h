#pragma once
#include<iostream>
using namespace std;
class CombatSystem;
class Creature;
class Card;
class Buff
{
public:
    Buff(string name, string description, int trigger_type, int duration, int priotity);

    string name_;

    string description_;

    int duration_;                                                     //buff持续时间

    int trigger_type_;                                                 //buff触发方式

    int priotity_;                                                     //buff触发优先级 

    bool is_stackable_;                                                //有的buff可以叠加

    bool is_positive_;                                                 //区分正负面buff

    int numeric_value_;                                                //buff的数值，比如力量的数值，精准的数值，无实体的数值

    virtual void onCardPlayed(std::shared_ptr<Card> card) {};                          //在打出一张牌时触发

    virtual void onTurnStart() {};                                     //在回合开始时触发

    virtual void onTurnEnd() {};                                       //在回合结束时触发

    virtual void onAttack(int& numeric_value_, std::string cardName = "",
        std::shared_ptr<Creature> user = nullptr, std::shared_ptr<Creature> target = nullptr) {};      //在进行攻击时触发,主要应用于修改：易伤、虚弱、力量

    virtual void onGetBlock(int& numeric_value_) {};                   //在得到格挡时触发，主要应用于修改：敏捷

    virtual void onAttacked(int& numeric_value_, std::shared_ptr<Creature>, std::shared_ptr<Creature>) {};

    virtual void onTakeDamage(int& numeric_value_) {};

    virtual void onLoseBlock(int& numeric_value_) {};

    virtual void onLoseHealth(int& numeric_value_) {};

    virtual void getBuff() {};

    virtual void onDrawCard(int& num) {};

    virtual void onExhaustCard() {};

    virtual ~Buff();

    bool operator<(const Buff& other) const;                           //比较两个buff的优先级
private:

};
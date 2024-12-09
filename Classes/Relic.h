#pragma once
#include <iostream>
#include <memory>
#include <string>
using namespace std;

class Card;
class Creature;

class Relic {
public:
    Relic(string name, string description, int trigger_type, int priority);

    virtual ~Relic();

    // 事件触发方法
    virtual void onTurnStart() {};
    virtual void onTurnEnd() {};
    virtual void onCardPlayed(shared_ptr<Card> card) {};
    virtual void onTakeDamage(int& damage, shared_ptr<Creature> user, shared_ptr<Creature> target) {};
    virtual void onHeal(int& healAmount) {};
    virtual void onKill(shared_ptr<Creature> target) {};

    // 比较优先级
    bool operator<(const Relic& other) const;

    string name_;               // 遗物名称
    string description_;        // 遗物描述
    int trigger_type_;          // 触发类型
    int priority_;              // 优先级
    int price_;                 // 在商店购买的价格
};

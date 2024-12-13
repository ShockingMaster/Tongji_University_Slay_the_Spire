#pragma once
#include <vector>
#include <string>
using namespace std;
class Creature;

class CombatSystem;
class Card;
class Potion;
class Relic
{
public:
    Relic(string name, string description, int rarity) :
        name_(name),
        description_(description),
        rarity_(rarity) {};

    std::string name_;

    string description_;

    int rarity_;

    virtual void onCardPlayed(std::shared_ptr<Card> card) {};          //�ڴ��һ����ʱ����

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

    virtual void getBuff() {};

    virtual void onDrawCard(int& num) {};

    virtual void onExhaustCard() {};

    virtual void onShuffleDeck() {};

    virtual void onAddCard(std::shared_ptr<Card> card) {};

    virtual void onAddPotion(std::shared_ptr<Potion> potion) {};

    virtual void onDeath() {};

    virtual void onAddRelic(std::shared_ptr<Relic> relics) {};

    virtual void onCoinChange(int& coinChange) {};

    virtual void onHealthChange(int& healthChange) {};

    virtual ~Relic();

private:

};
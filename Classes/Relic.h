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

    // �¼���������
    virtual void onTurnStart() {};
    virtual void onTurnEnd() {};
    virtual void onCardPlayed(shared_ptr<Card> card) {};
    virtual void onTakeDamage(int& damage, shared_ptr<Creature> user, shared_ptr<Creature> target) {};
    virtual void onHeal(int& healAmount) {};
    virtual void onKill(shared_ptr<Creature> target) {};

    // �Ƚ����ȼ�
    bool operator<(const Relic& other) const;

    string name_;               // ��������
    string description_;        // ��������
    int trigger_type_;          // ��������
    int priority_;              // ���ȼ�
    int price_;                 // ���̵깺��ļ۸�
};

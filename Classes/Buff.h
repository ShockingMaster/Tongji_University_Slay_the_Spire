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

    int duration_;                                                     //buff����ʱ��

    int trigger_type_;                                                 //buff������ʽ

    int priotity_;                                                     //buff�������ȼ� 

    bool is_stackable_;                                                //�е�buff���Ե���

    bool is_positive_;                                                 //����������buff

    int numeric_value_;                                                //buff����ֵ��������������ֵ����׼����ֵ����ʵ�����ֵ

    virtual void onCardPlayed(std::shared_ptr<Card> card) {};                          //�ڴ��һ����ʱ����

    virtual void onTurnStart() {};                                     //�ڻغϿ�ʼʱ����

    virtual void onTurnEnd() {};                                       //�ڻغϽ���ʱ����

    virtual void onAttack(int& numeric_value_, std::string cardName = "",
        std::shared_ptr<Creature> user = nullptr, std::shared_ptr<Creature> target = nullptr) {};      //�ڽ��й���ʱ����,��ҪӦ�����޸ģ����ˡ�����������

    virtual void onGetBlock(int& numeric_value_) {};                   //�ڵõ���ʱ��������ҪӦ�����޸ģ�����

    virtual void onAttacked(int& numeric_value_, std::shared_ptr<Creature>, std::shared_ptr<Creature>) {};

    virtual void onTakeDamage(int& numeric_value_) {};

    virtual void onLoseBlock(int& numeric_value_) {};

    virtual void onLoseHealth(int& numeric_value_) {};

    virtual void getBuff() {};

    virtual void onDrawCard(int& num) {};

    virtual void onExhaustCard() {};

    virtual ~Buff();

    bool operator<(const Buff& other) const;                           //�Ƚ�����buff�����ȼ�
private:

};
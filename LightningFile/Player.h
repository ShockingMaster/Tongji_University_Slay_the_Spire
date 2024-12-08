#pragma once
#include<vector>
#include "Creature.h"
using namespace std;
class Potion;
class Player :public Creature
{
public:
    static std::shared_ptr<Player> getInstance();

    virtual void start_turn();     //���ƣ���������ʧȥ���и񵲣�����һ��buff����

    virtual void end_turn();       //ʧȥ���п��ƣ�ʧȥ��������������һ��buff����

    vector<Potion*> potions_;

    void addPotion(Potion* potion);//��Ҫ���ñ����addPotion()

    void init();                   //ÿ��ս����ʼʱ�Խ�ɫ���г�ʼ��
private:
    static std::shared_ptr<Player> instance_;
};


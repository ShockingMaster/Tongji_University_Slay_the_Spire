#pragma once
#include<vector>
#include "Creature.h"
using namespace std;
class Potion;
class Player :public Creature
{
public:
    static std::shared_ptr<Player> getInstance();

    void startTurn();          //���ƣ���������ʧȥ���и񵲣�����һ��buff����

    void endTurn();            //ʧȥ���п��ƣ�ʧȥ��������������һ��buff����

    int getMaxEnergy() const {         //���ؽ�ɫ�������
        return maxEnergy_;
    }          

    int getCurrentEnergy() const {     //���ؽ�ɫ��ǰ�е�����
        return currentEnergy_;
    }
    void energyChange(int changeValue);//�Խ�ɫ�����������޸�

    vector<Potion*> potions_;

    void addPotion(Potion* potion);    //��Ҫ���ñ����addPotion()

    void init();                       //ÿ��ս����ʼʱ�Խ�ɫ���г�ʼ��
private:
    static std::shared_ptr<Player> instance_;
    int maxEnergy_;
    int currentEnergy_;
};


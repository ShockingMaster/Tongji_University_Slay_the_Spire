#pragma once
#include<vector>
#include "Creature.h"
using namespace std;
class Potion;
class Player :public Creature
{
public:
    static std::shared_ptr<Player> getInstance();

    int getMaxEnergy() const {         //���ؽ�ɫ�������
        return maxEnergy_;
    }          

    int getCurrentEnergy() const {     //���ؽ�ɫ��ǰ�е�����
        return currentEnergy_;
    }
    void energyChange(int changeValue);//�Խ�ɫ�����������޸�

    void init();                       //ÿ��ս����ʼʱ�Խ�ɫ���г�ʼ��

    virtual ~Player() {};
private:
    static std::shared_ptr<Player> instance_;
    int maxEnergy_;
    int currentEnergy_;
};


#pragma once
#include<vector>
#include "Creature.h"
using namespace std;
class Potion;

class Player : public Creature
{
public:
    string character_;
    int coins_;  //�������
    static Player* getInstance();//����Ψһʵ��
    virtual void start_turn();//���ƣ���������ʧȥ���и񵲣�����һ��buff����
    virtual void end_turn();//ʧȥ���п��ƣ�ʧȥ��������������һ��buff����
    vector<Potion*> potions_;
    void addPotion(Potion* potion);//��Ҫ���ñ����addPotion()
    void init();//ÿ��ս����ʼʱ�Խ�ɫ���г�ʼ��
    //static Player* instance_;
private:
    static Player* instance_;
    
};
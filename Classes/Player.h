#pragma once
#include<vector>
#include "Creature.h"
#include "Card.h"
#include "Relic.h"
using namespace std;
class Potion;

class Player : public Creature
{
public:
    vector<Card*> cards_= { NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL};       // ��ǰӵ�еĿ���
    string character_;          //��ǰ��ɫ
    int coins_;                 //�������
    static Player* getInstance();//����Ψһʵ��
    virtual void start_turn();//���ƣ���������ʧȥ���и񵲣�����һ��buff����
    virtual void end_turn();//ʧȥ���п��ƣ�ʧȥ��������������һ��buff����
    vector<Potion*> potions_ = { NULL,NULL,NULL };
    vector<Relic*> relics_={ NULL,NULL,NULL,NULL,NULL,NULL};
    void addPotion(Potion* potion);//��Ҫ���ñ����addPotion()
    void init();//ÿ��ս����ʼʱ�Խ�ɫ���г�ʼ��
private:
    static Player* instance_;
    
};
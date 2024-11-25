#pragma once
#include<vector>
#include<string>
using namespace std;
class Creature;
class Card
{
    string name_;
    string description_;
    int energy_cost_;//������������
    int money_cost_;//�̵깺��۸�
    int rarity_;//ϡ�ж�
    bool is_enabled_;//���ʱ���ƿ��Ա����
    int type_;//��������
    int trigger_times_;//�����еĿ��ƣ�Ч���ᴥ�����
    void take_effect(Creature* target);
    void take_effect(vector<Creature*> target);
};
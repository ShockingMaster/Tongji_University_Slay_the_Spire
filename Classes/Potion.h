#pragma once
#include<vector>
#include<string>
using namespace std;
class Creature;
class Potion
{
public:
    string name_;
    string description_;
    int numeric_value_;//ҩˮ����ֵ
    int money_cost_;//�̵�Ļ���
    int rarity_;//ϡ�ж�
    bool is_enabled_;//�е�ҩˮֻ�����Լ��Ļغ�����ʹ�ã������е�ҩˮȫ�ֶ�����ʹ��
    virtual void take_effect(Creature* target);//��Ҫ����һ��ʹ��Ŀ��
    virtual void take_effect(vector<Creature*> target);//����ж��Ŀ��Ļ�������һ��vector
    void show_info();//���������ҩˮ��ʱ����չʾҩˮ�Ļ�����Ϣ
};
#pragma once
#include<vector>
#include<string>
using namespace std;
class Creature;
class Card
{
public:
    Card() {};
    Card(std::string temp_name, std::string temp_description, int temp_energy_cost,
        int temp_money_cost, int temp_rarity, bool temp_can_be_played, int temp_type, int temp_need_target_)
        :name_(temp_name),
        description_(temp_description),
        energy_cost_(temp_energy_cost),
        money_cost_(temp_money_cost),
        rarity_(temp_rarity),
        can_be_played_(temp_can_be_played),
        type_(temp_type),
        need_target_(temp_need_target_) {};

    virtual void takeEffect() {};                    /*�������ʱ����Ч��������Ҫѡ�е���(��ȫ��������Ч����
                                                       �����������Ч�����������Ŀ�����Ч��)*/
    virtual void takeEffect(std::shared_ptr<Creature> target) {};    //�������ʱ����Ч������Ҫѡ�е���
    
    virtual void takeEffectOnDiscard() {};                           //������ʱ����Ч��
    
    virtual void takeEffectOnExhaust() {};                           //������ʱ����Ч��
    
    std::string getName() const                                      //���ؿ�������
    {
        return name_;
    }
    int getEnergyCost() const                                        //���ؿ�����������ֵ
    {
        return energy_cost_;
    }
    int getType() const                                              //���ؿ������ͣ�����,���ܣ�������
    {
        return type_;
    }
    std::string getDescription() const                               //���ؿ�������
    {
        return description_;
    }
    int getMoneyCost() const {                                       //���ع�������Ҫ�Ľ��
        return  money_cost_;
    }
    int getRarity() const {                                          //���ؿ���ϡ�ж�
        return rarity_;
    }
    bool getCanBePlayed() const{                                     //���ؿ����Ƿ��ܱ����
        return can_be_played_;
    }

    bool needTarget() const {
        return need_target_;
    }
    virtual ~Card() {                                                //��������

    }
private:
    std::string name_;                                               //��������
    std::string description_;                                        //��������
    int energy_cost_;                                                //������������
    int money_cost_;                                                 //�̵깺��۸�
    int rarity_;                                                     //ϡ�ж�
    bool can_be_played_;                                             //�����Ƿ��ܱ����
    int type_;                                                       //��������
    bool need_target_;                                                //�Ƿ���Ҫѡ��Ŀ����ܴ��
};
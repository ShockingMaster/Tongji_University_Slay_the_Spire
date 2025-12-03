#pragma once
#include<vector>
#include<string>
#include<memory>
using namespace std;
class Creature;
class IEffect;
class Card : public std::enable_shared_from_this<Card>
{
public:
    Card() {};
    Card(std::string temp_name, std::string temp_description, int temp_energy_cost, int temp_money_cost,
        int temp_rarity, bool temp_can_be_played, int temp_type, bool temp_need_target_, bool temp_is_exhaust_ = 0,
        bool temp_is_upgraded_ = 0)
        :name_(temp_name),
        description_(temp_description),
        energy_cost_(temp_energy_cost),
        money_cost_(temp_money_cost),
        rarity_(temp_rarity),
        can_be_played_(temp_can_be_played),
        type_(temp_type),
        need_target_(temp_need_target_),
        is_exhaust_(temp_is_exhaust_) ,
        is_upgraded_(temp_is_upgraded_){};

    virtual void takeEffect() {};                    /*�������ʱ����Ч��������Ҫѡ�е���(��ȫ��������Ч����
                                                       �����������Ч�����������Ŀ�����Ч��)*/
    virtual void takeEffect(std::shared_ptr<Creature> target) {};    //�������ʱ����Ч������Ҫѡ�е���

    virtual void takeeffectonturnend(std::shared_ptr<Card> card) {};                           //�غϽ��������Ч��

    virtual void takeEffectOnDiscard() {};                           //������ʱ����Ч��

    virtual void takeEffectOnExhaust() {};                           //������ʱ����Ч��


    virtual void upgrade() {};                                       //�Կ��ƽ�������
    
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
    bool isExhaust() const {
        return is_exhaust_;
    }

    bool isUpgraded() const {
        return is_upgraded_;
    }

    virtual ~Card() {}                                              //��������
    int tag = 0;
    
    // 添加效果相关方法
    void addEffect(std::shared_ptr<IEffect> effect) {
        effects_.push_back(effect);
    }
    
    // 执行所有效果
    void executeAllEffects(std::shared_ptr<Creature> target = nullptr) {
        int numeric_value = 0;
        for (auto& effect : effects_) {
            effect->execute(Player::getInstance(), target, shared_from_this(), numeric_value);
        }
    }
    
protected:
    std::string name_;                                                //��������
    std::string description_;                                         //��������
    int energy_cost_;                                                 //������������
    int money_cost_;                                                  //�̵깺��۸�
    int rarity_;                                                      //ϡ�ж�
    bool can_be_played_;                                              //�����Ƿ��ܱ����
    int type_;                                                        //��������
    bool need_target_;                                                //�Ƿ���Ҫѡ��Ŀ����ܴ��
    bool is_exhaust_;                                                 //�Ƿ�Ϊ������
    bool is_upgraded_;                                                //�Ƿ��������Ŀ���
    std::vector<std::shared_ptr<IEffect>> effects_;                   // 效果列表
};

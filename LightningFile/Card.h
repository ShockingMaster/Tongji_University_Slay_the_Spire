#pragma once
#include<vector>
#include<string>
using namespace std;
class Creature;
class Card
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

    virtual void takeEffect() {};                    /*打出卡牌时触发效果，不需要选中敌人(对全体敌人造成效果、
                                                       对于自身造成效果、对于随机目标造成效果)*/
    virtual void takeEffect(std::shared_ptr<Creature> target) {};    //打出卡牌时触发效果，需要选中敌人

    virtual void takeEffectOnDiscard() {};                           //被弃置时产生效果

    virtual void takeEffectOnExhaust() {};                           //被消耗时产生效果

    virtual void takeEffectOnTurnEnd() {};                           //在回合结束时产生效果 

    virtual void upgrade() {};                                       //对卡牌进行升级
    
    std::string getName() const                                      //返回卡牌名称
    {
        return name_;
    }
    int getEnergyCost() const                                        //返回卡牌消耗能量值
    {
        return energy_cost_;
    }
    int getType() const                                              //返回卡牌类型（攻击,技能，能力）
    {
        return type_;
    }
    std::string getDescription() const                               //返回卡牌描述
    {
        return description_;
    }
    int getMoneyCost() const {                                       //返回购买卡牌需要的金币
        return  money_cost_;
    }
    int getRarity() const {                                          //返回卡牌稀有度
        return rarity_;
    }
    bool getCanBePlayed() const{                                     //返回卡牌是否能被打出
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

    virtual ~Card() {}                                              //析构函数
    int tag = 0;
    
protected:
    std::string name_;                                                //卡牌名称
    std::string description_;                                         //卡牌描述
    int energy_cost_;                                                 //卡牌消耗能量
    int money_cost_;                                                  //商店购买价格
    int rarity_;                                                      //稀有度
    bool can_be_played_;                                              //卡牌是否能被打出
    int type_;                                                        //卡牌类型
    bool need_target_;                                                //是否需要选中目标才能打出
    bool is_exhaust_;                                                 //是否为消耗牌
    bool is_upgraded_;                                                //是否是升级的卡牌
};

#include "IncludeAll.h"
#include "Enum.h"


class Attack : public Card
{
    int damage_;
    //进行buff结算
    void take_effect(Creature* target)
    {
        int temp_damage = damage_;
        Player::getInstance()->modifyValue(target, ON_ATTACK, temp_damage);//完成damage的结算
        target->take_damage(temp_damage);
    }
};



Card::Card(string name, string description, int energyCost, int moneyCost, int rarity, bool isEnabled, int type, int triggerTimes)
    : name_(name), description_(description), energyCost_(energyCost), moneyCost_(moneyCost), rarity_(rarity),
    isEnabled_(isEnabled), type_(type), triggerTimes_(triggerTimes), targetNum(false) {
    // 如果卡牌有多个目标，可以根据需要设置 targetNum
}

/**
 * @brief 施加卡牌效果到单个目标
 * @param target 施加效果的目标
 */
void Card::takeEffect(Creature* target) {

    // 根据卡牌类型施加不同的效果
    

   
}

/**
 * @brief 施加卡牌效果到多个目标
 * @param targets 施加效果的多个目标
 */
void Card::takeEffect(vector<Creature*> targets) {

    for (auto& target : targets) {
        if (target != nullptr) {
            takeEffect(target);  // 针对每个目标施加效果
        }
    }
}

void Card::update() {

}

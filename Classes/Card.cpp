#include "IncludeAll.h"
#include "Enum.h"


class Attack : public Card
{
    int damage_;
    //����buff����
    void take_effect(Creature* target)
    {
        int temp_damage = damage_;
        Player::getInstance()->modifyValue(target, ON_ATTACK, temp_damage);//���damage�Ľ���
        target->take_damage(temp_damage);
    }
};



Card::Card(string name, string description, int energyCost, int moneyCost, int rarity, bool isEnabled, int type, int triggerTimes)
    : name_(name), description_(description), energyCost_(energyCost), moneyCost_(moneyCost), rarity_(rarity),
    isEnabled_(isEnabled), type_(type), triggerTimes_(triggerTimes), targetNum(false) {
    // ��������ж��Ŀ�꣬���Ը�����Ҫ���� targetNum
}

/**
 * @brief ʩ�ӿ���Ч��������Ŀ��
 * @param target ʩ��Ч����Ŀ��
 */
void Card::takeEffect(Creature* target) {

    // ���ݿ�������ʩ�Ӳ�ͬ��Ч��
    

   
}

/**
 * @brief ʩ�ӿ���Ч�������Ŀ��
 * @param targets ʩ��Ч���Ķ��Ŀ��
 */
void Card::takeEffect(vector<Creature*> targets) {

    for (auto& target : targets) {
        if (target != nullptr) {
            takeEffect(target);  // ���ÿ��Ŀ��ʩ��Ч��
        }
    }
}

void Card::update() {

}

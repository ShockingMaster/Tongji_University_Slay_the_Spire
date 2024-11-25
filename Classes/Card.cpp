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

void Card::take_effect(Creature* target)
{
}

void Card::take_effect(vector<Creature*> target)
{
}

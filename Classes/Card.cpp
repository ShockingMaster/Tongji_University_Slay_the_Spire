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

void Card::take_effect(Creature* target)
{
}

void Card::take_effect(vector<Creature*> target)
{
}

#include "IncludeAll.h"
#include "Enum.h"
Buff::Buff(string name, string description, int trigger_type, int duration, int priotity)
{
}
void Buff::take_effect(Creature* target)
{
}

void Buff::take_effect(int& numeric_value)
{
}

Buff::~Buff()
{
}

bool Buff::operator<(const Buff& other) const
{
    return false;
}

class HealingBuff : public Buff
{
public:
    HealingBuff(int healing_value)
        : Buff("Healing", "Heals the player", ON_TURN_END, 1, 1) {}
    void take_effect(Creature* target)
    {
        target->health_ += numeric_value_;
        numeric_value_--;
        target->updateHealth();//Õâ¸ö
        target->updateBuff();
    }
};
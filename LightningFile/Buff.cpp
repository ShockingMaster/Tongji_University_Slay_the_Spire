#include "IncludeAll.h"
#include "Enum.h"
Buff::Buff(string name, string description, int trigger_type, int duration, int priotity)
{
}


Buff::~Buff()
{
}

bool Buff::operator<(const Buff& other) const
{
    return false;
}

//力量Buff, 效果为增加伤害数值
class StrengthBuff : public Buff
{
    void onAttack(int& numeric_value_, std::string cardName,
        std::shared_ptr<Creature> user, std::shared_ptr<Creature> target)
    {
        numeric_value_ += 2;
    }
};



//精准Buff，效果为当卡牌名称为小刀时，造成更多伤害
class Accuracy : public Buff
{
    void onAttack(int& numeric_value_, std::string cardName,
        std::shared_ptr<Creature> user, std::shared_ptr<Creature> target)
    {
        if (cardName == "Blade" || cardName =="Blade+")
        {
            numeric_value_ += 4;
        }
    }
};

//荆棘，效果为当受到攻击（由攻击牌触发）时，对伤害来源造成一定伤害
class Thorns :public Buff
{
    void onAttacked(int& numeric_value_, std::shared_ptr<Creature> user, std::shared_ptr<Creature> target)
    {
        CombatSystem::getInstance()->takeDamage(user, 5, target);
    }
};
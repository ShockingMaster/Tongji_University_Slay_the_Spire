#include "IncludeAll.h"
#include "Enum.h"


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

//人工制品，效果为当被叠加debuff的时候，消耗一层并抵消
class artificialProducts :public Buff
{
    void addBuff(std::shared_ptr<Buff> buff, int &numeric_value, std::shared_ptr<Creature> target)
    {
        //负面buff才会执行
        if (buff->is_positive_ == false) {
            if (numeric_value_ <= numeric_value) {
                numeric_value_ = 0;                        //人工制品层数归零
                numeric_value -= numeric_value_;           //debuff层数减少
            }
            else {
                numeric_value_ -= numeric_value;           //人工制品层数减少
            }
        }
    }
};
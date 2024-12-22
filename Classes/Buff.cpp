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
public:
    StrengthBuff() : Buff("Strength", "", 0, 0, true, 0, YES, EFFECT_LAYERS) {}
    void onAttack(int& numeric_value_, std::string cardName,
        std::shared_ptr<Creature> user, std::shared_ptr<Creature> target)
    {
        numeric_value_ += 2;
    }
};
//进行卡牌注册
AUTO_REGISTER_BUFF(StrengthBuff);

/*
//精准Buff，效果为当卡牌名称为小刀时，造成更多伤害
class Accuracy : public Buff
{
public:
    Accuracy() : Buff("Accuracy", "", 0, 0, true, 0, YES, DURATION) {}
    void onAttack(int& numeric_value_, std::string cardName,
        std::shared_ptr<Creature> user, std::shared_ptr<Creature> target)
    {
        if (cardName == "Blade" || cardName == "Blade+")
        {
            numeric_value_ += 4;
        }
    }
};
AUTO_REGISTER_BUFF(Accuracy);


//荆棘，效果为当受到攻击（由攻击牌触发）时，对伤害来源造成一定伤害
class Thorns :public Buff
{
public:
    Thorns() : Buff("Thorns", "", 0, 0, true, 0, YES, DURATION) {}
    void onAttacked(int& numeric_value_, std::shared_ptr<Creature> user, std::shared_ptr<Creature> target)
    {
        CombatSystem::getInstance()->takeDamage(user, 5, target);
    }
};
AUTO_REGISTER_BUFF(Thorns);
*/

//人工制品，效果为当被叠加debuff的时候，消耗一层并抵消
class ArtificialProducts :public Buff
{
public:
    ArtificialProducts() : Buff("ArtificialProducts", "", 0, 0, true, 0, YES, EFFECT_LAYERS) {}
    void addBuff(std::shared_ptr<Buff> buff, int& numeric_value)
    {
        //负面buff才会执行
        if (buff->is_positive_ == false) {
            if (effect_layers <= numeric_value) {
                
                numeric_value -= effect_layers;           //debuff层数减少
                effect_layers = 0;                        //人工制品层数归零
            }
            else {
                effect_layers -= numeric_value;           //人工制品层数减少
                numeric_value = 0;                         //debuff层数归零
            }
        }
        //更新
        auto scene = (CombatScene*)(Director::getInstance()->getRunningScene());
        scene->creatureLayer->updateDisplay();
    }
};
AUTO_REGISTER_BUFF(ArtificialProducts);



//易伤，效果为从 攻击 受到的伤害增加 50% .持续 X 回合。
class vulnerability :public Buff
{
public:
    vulnerability() : Buff("vulnerability", "", 0, 0, false, 0, YES, DURATION) {}
    void onAttacked(int& numeric_value_,std::shared_ptr<Creature> user, std::shared_ptr<Creature> target)
    {
        numeric_value_ *= 1.5;
    }
};
AUTO_REGISTER_BUFF(vulnerability);
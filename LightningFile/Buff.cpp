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

//����Buff, Ч��Ϊ�����˺���ֵ
class StrengthBuff : public Buff
{
    void onAttack(int& numeric_value_, std::string cardName,
        std::shared_ptr<Creature> user, std::shared_ptr<Creature> target)
    {
        numeric_value_ += 2;
    }
};



//��׼Buff��Ч��Ϊ����������ΪС��ʱ����ɸ����˺�
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

//������Ч��Ϊ���ܵ��������ɹ����ƴ�����ʱ�����˺���Դ���һ���˺�
class Thorns :public Buff
{
    void onAttacked(int& numeric_value_, std::shared_ptr<Creature> user, std::shared_ptr<Creature> target)
    {
        CombatSystem::getInstance()->takeDamage(user, 5, target);
    }
};
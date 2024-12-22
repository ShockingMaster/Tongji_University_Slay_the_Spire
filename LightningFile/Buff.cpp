#include "IncludeAll.h"
#include "Enum.h"


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
public:
    StrengthBuff() : Buff("Strength", "", 0, 0, true, 0, YES, EFFECT_LAYERS) {}
    void onAttack(int& numeric_value_, std::string cardName,
        std::shared_ptr<Creature> user, std::shared_ptr<Creature> target)
    {
        numeric_value_ += 2;
    }
};
//���п���ע��
AUTO_REGISTER_BUFF(StrengthBuff);

/*
//��׼Buff��Ч��Ϊ����������ΪС��ʱ����ɸ����˺�
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


//������Ч��Ϊ���ܵ��������ɹ����ƴ�����ʱ�����˺���Դ���һ���˺�
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

//�˹���Ʒ��Ч��Ϊ��������debuff��ʱ������һ�㲢����
class ArtificialProducts :public Buff
{
public:
    ArtificialProducts() : Buff("ArtificialProducts", "", 0, 0, true, 0, YES, EFFECT_LAYERS) {}
    void addBuff(std::shared_ptr<Buff> buff, int& numeric_value)
    {
        //����buff�Ż�ִ��
        if (buff->is_positive_ == false) {
            if (effect_layers <= numeric_value) {
                
                numeric_value -= effect_layers;           //debuff��������
                effect_layers = 0;                        //�˹���Ʒ��������
            }
            else {
                effect_layers -= numeric_value;           //�˹���Ʒ��������
                numeric_value = 0;                         //debuff��������
            }
        }
        //����
        auto scene = (CombatScene*)(Director::getInstance()->getRunningScene());
        scene->creatureLayer->updateDisplay();
    }
};
AUTO_REGISTER_BUFF(ArtificialProducts);



//���ˣ�Ч��Ϊ�� ���� �ܵ����˺����� 50% .���� X �غϡ�
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
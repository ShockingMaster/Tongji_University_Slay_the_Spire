#include "IncludeAll.h"
#include "Enum.h"
/* 
 * ��ʼ�����������ƣ��������������Ʒ��ã������̵�۸񣬿���ϡ�жȣ������Ƿ��ܱ��������������
 * 
 */

/*
* �������ƣ�Attack
* Ч��������ѡ�еĵ������6���˺�
 */
class Attack : public Card
{
public:
    Attack() : Card("Attack", "Deal 6 damage", 1, 20, COMMON, PLAYABLE, ATTACK, YES, NO) {}
    void takeEffect(std::shared_ptr<Creature> target)
    {
        int temp_attack = 6;
        CombatSystem::getInstance()->onAttack(Player::getInstance(), target, 
            temp_attack, "Attack");
    }
};
//���п���ע��
AUTO_REGISTER_CARD(Attack)


/*
* �������ƣ�second_wind
* Ч�����������зǹ����ƣ�ÿ�Ż��5���
 */
    class Second_wind : public Card
{
public:
    Second_wind() : Card("Second_wind", "Consume all non attack cards and receive 5 grid blocks per card", 1, 20, UNCOMMON, PLAYABLE, ABILITY, NO, YES) {}
    void takeEffect()
    {
        int temp_block = 5;
        int num = 0;
        for (size_t i = 0; i < CombatSystem::getInstance()->hand.size(); ++i) {
            const auto& card = CombatSystem::getInstance()->hand[i];  // ��ȡ��ǰ����
            if (card->getType() != ATTACK) {
                ++num;
                CombatSystem::getInstance()->deleteCard(i, "Second_wind"); //���ķǹ�����
                i--;
                CCLOG("Card at index %zu is an attack card", i);  // �����ǰ���Ƶ�����
            }
        }
        CombatSystem::getInstance()->Addblock(Player::getInstance(),temp_block*num, "Second_wind"); //���ӻ���
    }
};
//���п���ע��
AUTO_REGISTER_CARD(Second_wind)


/*
* �������ƣ�Revival
* Ч����
 */
//class Revival : public Card
//{
//public:
//    Revival() : Card("Revival", "Exhaust all cards that are not attack cards in hand", 1,
//        50, UNCOMMON, PLAYABLE, SKILL, NO) {}
//    void takeEffect()
//    {
//        /* �������п���
//         * ����getType()��ΪAttack����
//         * ���ĸÿ��ƣ����5���(���������������Ϊ�������Ϊ����ν���ʵ�ֵ�)
//         */
//    }
//};
//AUTO_REGISTER_CARD(Revival);

/*
 *
 * 
 * 
 */

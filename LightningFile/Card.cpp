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

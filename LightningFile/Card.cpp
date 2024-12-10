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
        std::vector<std::shared_ptr<Card>> card_hand = CombatSystem::getInstance()->hand;
        for (size_t i = 0; i < card_hand.size(); ++i) {
            const auto& card = card_hand[i];  // ��ȡ��ǰ����
            if (card->getType() != ATTACK) {
                ++num;
                CombatSystem::getInstance()->hand.erase(card_hand.begin() + i); //���ķǹ�����
                CCLOG("Card at index %zu is an attack card", i);  // �����ǰ���Ƶ�����
            }
        }
        Player::getInstance()->block_ += num * temp_block;
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

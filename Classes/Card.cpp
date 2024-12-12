#include "IncludeAll.h"
#include "Enum.h"
/* 
 * ��ʼ�����������ƣ��������������Ʒ��ã������̵�۸񣬿���ϡ�жȣ������Ƿ��ܱ�������������ͣ������Ƿ���ҪĿ�����������Ƿ�Ϊ������
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

class Second_wind : public Card
{
public:
    Second_wind() : Card("Second_wind", "Consume all non attack cards and receive 5 grid blocks per card", 1, 20, UNCOMMON, PLAYABLE, SKILL, NO, NO) {}
    void takeEffect()
    {
        int temp_block = 5;
        int num = 0;
        for (size_t i = 0; i < CombatSystem::getInstance()->hand.size(); ++i) 
        {
            const auto& card = CombatSystem::getInstance()->hand[i];  // ��ȡ��ǰ����
            if (card->getType() != ATTACK) {
                ++num;
                CombatSystem::getInstance()->exhaustCard(i, "Second_wind"); //���ķǹ�����
                i--;
                CCLOG("Card at index %zu isn't an attack card", i);  // �����ǰ���Ƶ�����
                CombatSystem::getInstance()->Addblock(Player::getInstance(), temp_block, "Second_wind"); //���ӻ���
            }
        }
    }
};
//���п���ע��
AUTO_REGISTER_CARD(Second_wind)

/*
* �������ƣ�BurningContract
* Ч��������һ�����ƣ���������
 */
    class BurningContract : public Card
{
public:
    BurningContract() : Card("BurningContract", "Consume one card and draw two cards", 1, 20, UNCOMMON, PLAYABLE, SKILL, NO, NO) {}
    void takeEffect()
    {
        int draw_num = 2;
        //ʵ��ǰ��ѡ���Ʋ���ȡλ��
       // ���� selectScene ��ʹ�� pushScene �����л�
        Scene* selectScene = SelectScene::create();
        cocos2d::Director::getInstance()->pushScene(selectScene);  // �л��� SelectScene
        // ʵ��ǰ��ѡ���Ʋ���ȡλ��

        // ĿǰΪ���ĵ�0����
        // CombatSystem::getInstance()->exhaustCard(num, "BurningContract"); //����ѡ��Ŀ���
        CombatSystem::getInstance()->drawCard(draw_num); //��ȡ����
    }
};
//���п���ע��
AUTO_REGISTER_CARD(BurningContract)

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

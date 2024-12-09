#include "IncludeAll.h"
#include "Enum.h"
/* 
 * 初始化：卡牌名称，卡牌描述，卡牌费用，卡牌商店价格，卡牌稀有度，卡牌是否能被打出，卡牌类型
 * 
 */

/*
* 卡牌名称：Attack
* 效果：对于选中的敌人造成6点伤害
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
//进行卡牌注册
AUTO_REGISTER_CARD(Attack)


/*
* 卡牌名称：Revival
* 效果：
 */
//class Revival : public Card
//{
//public:
//    Revival() : Card("Revival", "Exhaust all cards that are not attack cards in hand", 1,
//        50, UNCOMMON, PLAYABLE, SKILL, NO) {}
//    void takeEffect()
//    {
//        /* 遍历手中卡牌
//         * 卡牌getType()不为Attack，则
//         * 消耗该卡牌，获得5点格挡(如何完成消耗这个行为，这个行为是如何进行实现的)
//         */
//    }
//};
//AUTO_REGISTER_CARD(Revival);

/*
 *
 * 
 * 
 */

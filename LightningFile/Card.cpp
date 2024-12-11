#include "IncludeAll.h"
#include "Enum.h"
/* 
 * 初始化：卡牌名称，卡牌描述，卡牌费用，卡牌商店价格，卡牌稀有度，卡牌是否能被打出，卡牌类型，卡牌是否需要目标打出，卡牌是否为消耗牌
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
            const auto& card = CombatSystem::getInstance()->hand[i];  // 获取当前卡牌
            if (card->getType() != ATTACK) {
                ++num;
                CombatSystem::getInstance()->exhaustCard(i, "Second_wind"); //消耗非攻击牌
                i--;
                CCLOG("Card at index %zu isn't an attack card", i);  // 输出当前卡牌的索引
                CombatSystem::getInstance()->Addblock(Player::getInstance(), temp_block, "Second_wind"); //增加护盾
            }
        }
    }
};
//进行卡牌注册
AUTO_REGISTER_CARD(Second_wind)

/*
* 卡牌名称：BurningContract
* 效果：消耗一张手牌，抽两张牌
 */
    class BurningContract : public Card
{
public:
    BurningContract() : Card("BurningContract", "Consume one card and draw two cards", 1, 20, UNCOMMON, PLAYABLE, SKILL, NO, NO) {}
    void takeEffect()
    {
        int draw_num = 2;
        //实现前端选择卡牌并读取位数
       // 创建 selectScene 并使用 pushScene 进行切换
        Scene* selectScene = SelectScene::create();
        cocos2d::Director::getInstance()->pushScene(selectScene);  // 切换到 SelectScene
        // 实现前端选择卡牌并读取位数

        // 目前为消耗第0张牌
        // CombatSystem::getInstance()->exhaustCard(num, "BurningContract"); //消耗选择的卡牌
        CombatSystem::getInstance()->drawCard(draw_num); //抽取卡牌
    }
};
//进行卡牌注册
AUTO_REGISTER_CARD(BurningContract)

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

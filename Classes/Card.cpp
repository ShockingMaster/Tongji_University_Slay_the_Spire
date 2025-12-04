#include "IncludeAll.h"
#include "Enum.h"
#include "AttackEffectAdapter.h"
#include "BlockEffectAdapter.h"
#include "SecondWindEffectAdapter.h"
#include "BurningContractEffectAdapter.h"
#include "ApotheosisEffectAdapter.h"
#include "TurnEndExhaustEffectAdapter.h"
#include "TurnEndDamageEffectAdapter.h"
#include "TrounceEffectAdapter.h"
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
    Attack() : Card("Attack", "Deal 6 damage", 1, 20, COMMON, PLAYABLE, ATTACK, YES, NO, NO) {
        addEffect(std::make_shared<AttackEffectAdapter>(6));
    }
    void upgrade() {
        is_upgraded_ = 1;
        name_ += '+';
        description_ = "Deal 9 damage";
        // 移除旧效果，添加新效果
        clearPlayEffects();
        addEffect(std::make_shared<AttackEffectAdapter>(9));
    }
    void takeEffect(std::shared_ptr<Creature> target)
    {
        executePlayEffects(target);
    }
};
//进行卡牌注册
AUTO_REGISTER_CARD(Attack)

/*
* 卡牌名称：Defense
* 效果：增加5点防御
 */
class Defense : public Card
{
public:
    Defense() : Card("Defense", "Gain 5 block", 1, 20, COMMON, PLAYABLE, SKILL, NO, NO, NO) {
        addEffect(std::make_shared<BlockEffectAdapter>(5));
    }
    void upgrade() {
        is_upgraded_ = 1;
        name_ += '+';
        description_ = "Gain 8 block";
        // 移除旧效果，添加新效果
        clearPlayEffects();
        addEffect(std::make_shared<BlockEffectAdapter>(8));
    }
    void takeEffect()
    {
        CCLOG("Defense played!");
        executePlayEffects();
    }
};
//进行卡牌注册
AUTO_REGISTER_CARD(Defense)


/*
* 卡牌名称：重振精神
* 效果：消耗所有非攻击牌，每张获得5点格挡
*/
class Second_wind : public Card
{
public:
    Second_wind() : Card("Second_wind", "Consume all non attack cards and receive 5 grid blocks per card", 
        1, 20, UNCOMMON, PLAYABLE, SKILL, NO, NO, NO) {
        addEffect(std::make_shared<SecondWindEffectAdapter>(5));
    }
    void upgrade() {
        is_upgraded_ = 1;
        name_ += '+';
        description_ = "Consume all non attack cards and receive 7 grid blocks per card";
        // 移除旧效果，添加新效果
        clearPlayEffects();
        addEffect(std::make_shared<SecondWindEffectAdapter>(7));
    }
    void takeEffect()
    {
        executePlayEffects();
    }
};
// 进行卡牌注册
AUTO_REGISTER_CARD(Second_wind)

/*
* 卡牌名称：BurningContract
* 效果：消耗一张手牌，抽两张牌
 */
class BurningContract : public Card
{
public:
    BurningContract() : Card("BurningContract", "Consume one card and draw two cards", 
        1, 20, UNCOMMON, PLAYABLE, SKILL, NO, NO, NO) {
        addEffect(std::make_shared<BurningContractEffectAdapter>(2, 1));
    }
    void upgrade() {
        is_upgraded_ = 1;
        name_ += '+';
        description_ = "Consume one card and draw three cards";
        clearPlayEffects();
        addEffect(std::make_shared<BurningContractEffectAdapter>(2, 1));
    }
    void takeEffect()
    {
        executePlayEffects();
    }
};
// 进行卡牌注册
AUTO_REGISTER_CARD(BurningContract)


/*
* 卡牌名称：神话
* 效果：临时升级当前战斗中的所有牌
*/
class Apotheosis :public Card
{
public:
    Apotheosis() : Card("Apotheosis", "Upgrade ALL of your cards for the rest of combat.Exhaust.",
        2, 180, RARE, PLAYABLE, SKILL, NO, YES, NO) {
        addEffect(std::make_shared<ApotheosisEffectAdapter>());
    };
    void upgrade() {
        is_upgraded_ = 1;
        name_ += '+';
        energy_cost_ -= 1;
    }
    void takeEffect()
    {
        executePlayEffects();
    }
};
AUTO_REGISTER_CARD(Apotheosis)


/*
* 卡牌名称：眩晕
* 效果：不能被打出，虚无
*/
class dazed :public Card
{
public:
    dazed() : Card("dazed", "Cannot be hit",
        0, 0, NORMAL, FALSE, STATUS, NO, NO, NO) {
        addTurnEndEffect(std::make_shared<TurnEndExhaustEffectAdapter>());
    };
    void takeeffectonturnend(std::shared_ptr<Card> card) {
        CCLOG("discard dazed");
        executeTurnEndEffects(card);
    }
};
AUTO_REGISTER_CARD(dazed)


/*
* 卡牌名称：灼烧
* 效果：不能被打出，回合结束造成两点伤害
*/
class burn :public Card
{
public:
    burn() : Card("burn", "Cannot be hit, causing two points of damage at the end of the turn",
        0, 0, NORMAL, FALSE, STATUS, NO, NO, NO) {
        addTurnEndEffect(std::make_shared<TurnEndDamageEffectAdapter>(2));
    };
    void takeeffectonturnend(std::shared_ptr<Card> card) {
        executeTurnEndEffects(card);
    }
};
AUTO_REGISTER_CARD(burn)


/*
* 卡牌名称：痛击
* 效果：造成8点伤害。给予2层易伤。
*/
class trounce :public Card
{
public:
    trounce() : Card("trounce", "Causing 8 points of damage. Give 2 layers of vulnerability.",
        2, 40, RARE, PLAYABLE, ATTACK, YES, NO, NO) {
        addEffect(std::make_shared<TrounceEffectAdapter>(8, 2, 2, 1));
    };
    void upgrade() {
        is_upgraded_ = 1;
        name_ += '+';
        description_ = "Causing 10 points of damage. Give 3 layers of vulnerability.";
        clearPlayEffects();
        addEffect(std::make_shared<TrounceEffectAdapter>(8, 2, 2, 1));
    }
    void takeEffect(std::shared_ptr<Creature> target)
    {
        executePlayEffects(target);
    }
};
AUTO_REGISTER_CARD(trounce)

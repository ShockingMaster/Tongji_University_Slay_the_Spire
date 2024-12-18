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
    Attack() : Card("Attack", "Deal 6 damage", 1, 20, COMMON, PLAYABLE, ATTACK, YES, NO, NO) {}
    void upgrade() {
        is_upgraded_ = 1;
        name_ += '+';
        description_ = "Deal 9 damage";
    }
    void takeEffect(std::shared_ptr<Creature> target)
    {
        int temp_attack = 6;
        if (is_upgraded_)
        {
            temp_attack += 3;
        }
        CombatSystem::getInstance()->onAttack(Player::getInstance(), target, 
            temp_attack, "Attack");
    }
};
//进行卡牌注册
AUTO_REGISTER_CARD(Attack)

class Second_wind : public Card
{
public:
    Second_wind() : Card("Second_wind", "Consume all non attack cards and receive 5 grid blocks per card", 
        1, 20, UNCOMMON, PLAYABLE, SKILL, NO, NO, NO) {}
    void upgrade() {
        is_upgraded_ = 1;
        name_ += '+';
        description_ = "Consume all non attack cards and receive 7 grid blocks per card";
    }
    void takeEffect()
    {
        int temp_block = 5;
        if (is_upgraded_)
        {
            temp_block += 2;
        }
        int num = 0;
        for (size_t i = 0; i < CombatSystem::getInstance()->hand.size(); ++i) 
        {
            const auto& card = CombatSystem::getInstance()->hand[i];  // 获取当前卡牌
            if (card->getType() != ATTACK) {
                ++num;
                CombatSystem::getInstance()->exhaustCard(i, "Second_wind"); //消耗非攻击牌
                i--;
                CCLOG("Card at index %zu isn't an attack card", i);  // 输出当前卡牌的索引
                CombatSystem::getInstance()->Addblock(Player::getInstance(), temp_block); //增加护盾
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
    BurningContract() : Card("BurningContract", "Consume one card and draw two cards", 
        1, 20, UNCOMMON, PLAYABLE, SKILL, NO, NO, NO) {}
    void upgrade() {
        is_upgraded_ = 1;
        name_ += '+';
        description_ = "Consume one card and draw three cards";
    }
    void takeEffect()
    {
        int draw_num = 2;
        if (is_upgraded_)
        {
            draw_num += 1;
        }
        if (tag == 0) {
            
            Scene* selectScene = SelectScene::create();
            HandPileLayer::getInstance()->card_num_select_target = 1;

            cocos2d::Director::getInstance()->pushScene(selectScene);  

        }
        if (tag == 1) {
            
            CombatSystem::getInstance()->drawCard(draw_num); 
            HandPileLayer::getInstance()->adjustHandPile();
        }
    }
};
//进行卡牌注册
AUTO_REGISTER_CARD(BurningContract)


/*
* 卡牌名称：神话
* 效果：临时升级当前战斗中的所有牌
*/
class Apotheosis :public Card
{
public:
    Apotheosis() : Card("Apotheosis", "Upgrade ALL of your cards for the rest of combat.Exhaust.",
        2, 180, RARE, PLAYABLE, SKILL, NO, YES, NO) {};
    void upgrade() {
        is_upgraded_ = 1;
        name_ += '+';
        energy_cost_ -= 1;
    }
    void takeEffect()
    {
        
        auto combatSystem = CombatSystem::getInstance();
        // 对手牌进行升级
        for (auto card:combatSystem->hand)
        {
            combatSystem->upgradeCard(card);
        }
        
        // 对弃牌堆进行升级
        for (int i = 0;i < combatSystem->getDiscardPileNumber();i++)
        {
            auto card = combatSystem->discardPile.front();
            combatSystem->upgradeCard(card);
            combatSystem->discardPile.push(card);// 放入队列尾部
            combatSystem->discardPile.pop();// 将队列头弹出，不修改队列顺序
        }
        // 对抽牌堆进行升级 
        for (int i = 0;i < combatSystem->getDrawPileNumber();i++)
        {
            auto card = combatSystem->drawPile.front();
            combatSystem->upgradeCard(card);
            combatSystem->drawPile.push(card);
            combatSystem->drawPile.pop();
        }
    }
};
AUTO_REGISTER_CARD(Apotheosis)


/*
* 卡牌名称：眩晕
* 效果：不能被打出
*/
class dazed :public Card
{
public:
    dazed() : Card("dazed", "Cannot be hit",
        0, 0, RARE, FALSE, SKILL, NO, NO, NO) {};
    void takeeffectonturnend(std::shared_ptr<Card> card) {
        CCLOG("discard dazed");
        CombatSystem::getInstance()->exhaustCard(card);
    }
};
AUTO_REGISTER_CARD(dazed)
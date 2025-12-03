#include "IncludeAll.h"
#include "Enum.h"
#include "AttackEffectAdapter.h"
#include "BlockEffectAdapter.h"
#include "SecondWindEffectAdapter.h"
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
    Attack() : Card("Attack", "Deal 6 damage", 1, 20, COMMON, PLAYABLE, ATTACK, YES, NO, NO) {
        addEffect(std::make_shared<AttackEffectAdapter>(6));
    }
    void upgrade() {
        is_upgraded_ = 1;
        name_ += '+';
        description_ = "Deal 9 damage";
        // 移除旧效果，添加新效果
        effects_.clear();
        addEffect(std::make_shared<AttackEffectAdapter>(9));
    }
    void takeEffect(std::shared_ptr<Creature> target)
    {
        executeAllEffects(target);
    }
};
//���п���ע��
AUTO_REGISTER_CARD(Attack)

/*
* �������ƣ�Defense
* Ч��������5�ط��
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
        effects_.clear();
        addEffect(std::make_shared<BlockEffectAdapter>(8));
    }
    void takeEffect()
    {
        CCLOG("Defense played!");
        executeAllEffects();
    }
};
//���п���ע��
AUTO_REGISTER_CARD(Defense)


/*
* �������ƣ�������
* Ч�������ĵ�ǰ���������еķǹ����ƣ�ÿ�Ż��5���
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
        effects_.clear();
        addEffect(std::make_shared<SecondWindEffectAdapter>(7));
    }
    void takeEffect()
    {
        executeAllEffects();
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
            HandPileLayer::getInstance()->removeFromParent();
            Scene* selectScene = SelectScene::create();
            SelectScene* selectScenePtr = dynamic_cast<SelectScene*>(selectScene);
            auto scene = dynamic_cast<CombatScene*>(Director::getInstance()->getRunningScene());
            selectScenePtr->setCombatScene(scene);
            HandPileLayer::getInstance()->card_num_select_target = 1;
            cocos2d::Director::getInstance()->pushScene(selectScene);
            
            
        }
        if (tag == 1) {
            
            CombatSystem::getInstance()->drawCard(draw_num); 
            HandPileLayer::getInstance()->adjustHandPile();
            tag = 0;
        }
    }
};
//���п���ע��
AUTO_REGISTER_CARD(BurningContract)


/*
* �������ƣ���
* Ч������ʱ������ǰս���е�������
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
        // �����ƽ�������
        for (auto card:combatSystem->hand)
        {
            combatSystem->upgradeCard(card);
        }
        
        // �����ƶѽ�������
        for (int i = 0;i < combatSystem->getDiscardPileNumber();i++)
        {
            auto card = combatSystem->discardPile.front();
            combatSystem->upgradeCard(card);
            combatSystem->discardPile.push(card);// �������β��
            combatSystem->discardPile.pop();// ������ͷ���������޸Ķ���˳��
        }
        // �Գ��ƶѽ������� 
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
* �������ƣ�ѣ��
* Ч�������ܱ����������
*/
class dazed :public Card
{
public:
    dazed() : Card("dazed", "Cannot be hit",
        0, 0, NORMAL, FALSE, STATUS, NO, NO, NO) {};
    void takeeffectonturnend(std::shared_ptr<Card> card) {
        CCLOG("discard dazed");
        CombatSystem::getInstance()->exhaustCard(card);
    }
};
AUTO_REGISTER_CARD(dazed)


/*
* �������ƣ�����
* Ч�������ܱ�������غϽ�����������˺�
*/
class burn :public Card
{
public:
    burn() : Card("burn", "Cannot be hit, causing two points of damage at the end of the turn",
        0, 0, NORMAL, FALSE, STATUS, NO, NO, NO) {};
    void takeeffectonturnend(std::shared_ptr<Card> card) {
        CombatSystem::getInstance()->takeDamage(Player::getInstance(), 2);
    }
};
AUTO_REGISTER_CARD(burn)


/*
* �������ƣ�ʹ��
* Ч�������8���˺�������2�����ˡ�
*/
class trounce :public Card
{
public:
    trounce() : Card("trounce", "Causing 8 points of damage. Give 2 layers of vulnerability.",
        2, 40, RARE, PLAYABLE, ATTACK, YES, NO, NO) {};
    void upgrade() {
        is_upgraded_ = 1;
        name_ += '+';
        description_ = "Causing 10 points of damage. Give 3 layers of vulnerability.";
    }
    void takeEffect(std::shared_ptr<Creature> target)
    {
        int temp_attack = 8;
        int numeric_value = 2;
        if (is_upgraded_)
        {
            temp_attack += 2;
            numeric_value += 1;
        }
        CombatSystem::getInstance()->onAttack(Player::getInstance(), target,
            temp_attack, "Attack");
        CombatSystem::getInstance()->addBuff(BuffRegistry::createBuff("vulnerability"), numeric_value, target);
    }
};
AUTO_REGISTER_CARD(trounce)
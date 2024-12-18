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
//���п���ע��
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
            const auto& card = CombatSystem::getInstance()->hand[i];  // ��ȡ��ǰ����
            if (card->getType() != ATTACK) {
                ++num;
                CombatSystem::getInstance()->exhaustCard(i, "Second_wind"); //���ķǹ�����
                i--;
                CCLOG("Card at index %zu isn't an attack card", i);  // �����ǰ���Ƶ�����
                CombatSystem::getInstance()->Addblock(Player::getInstance(), temp_block); //���ӻ���
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
* Ч�������ܱ����
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
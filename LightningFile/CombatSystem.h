#pragma once
/*
 * ս��ϵͳ��Ҫ�����¼�
 */
#include<vector>
#include"cocos2d.h"
#include"Creature.h"
#include"Buff.h"
class CombatDeck;
class CombatScene;
class HandPileLayer;
class CombatSystem
{
public:
	static CombatSystem* getInstance();
	std::vector<std::shared_ptr<Creature>> Monsters_;                           // ����
	std::queue<std::shared_ptr<Card>> drawPile;                                 // ���ƶ�
	std::vector<std::shared_ptr<Card>> hand;                                    // ����
	std::queue<std::shared_ptr<Card>> discardPile;                              // ���ƶ�
	std::shared_ptr<Card> tem_card;

	void init(int type = -1);                                                                //��ʼ��ս��ϵͳ

	void onAttack(std::shared_ptr<Creature> attacker, std::shared_ptr<Creature> target,
		int& numeric_value_, std::string cardName = "", bool isForIntentionUpdate = false);

	void combatStart();                                                         //ս����ʼ

	void combatEnd();                                                           //ս��������

	void takeDamage(std::shared_ptr<Creature> target, int numeric_value_,
		std::shared_ptr<Creature> attacker = nullptr);                         //��Ҫ���Ǿ�����Ҫ����ʲô����

	void Addblock(std::shared_ptr<Creature> target, int numeric_value_);

	void exhaustCard(int num, std::string cardName = "");

	void exhaustCard();

	void exhaustCard(std::shared_ptr<Card> card);

	void startTurn(std::shared_ptr<Creature> creature);                        //�غϿ�ʼЧ��

	void endTurn(std::shared_ptr<Creature> creature);                          //�غϽ���Ч��

	void cardPlayed(std::shared_ptr<Card> card);                               //�������Ч��

	void endTurnCardPlayed();                                                 //�غϽ�������Ч��

	void cardPlayed(std::shared_ptr<Card> card, std::shared_ptr<Creature> creature);//������ҪĿ��Ŀ��ƽ�������

	void discardCard(std::shared_ptr<Card> card);                               //������������

	void upgradeCard(std::shared_ptr<Card> card);                               //ս����ʱ����

	void addEnergy(std::shared_ptr<Creature> user, int numeric_value_);         //��������

	void addBuff(std::shared_ptr<Buff> buff, int numeric_value,std::shared_ptr<Creature> target);                //����Buff

	void addHealth(std::shared_ptr<Creature> target, int numeric_value);       //�����ظ�

	void shuffleDeck();                                                         //����ϴ��

	void drawCard(int num);                                                     //���г���

	int getDrawPileNumber();                                                    //��ȡ���ƶѿ�������

	int getHandNumber();                                                        //��ȡ���ƿ�������

	int getDiscardPileNumber();                                                 //��ȡ���ƶѿ�������

	void addToHand(std::shared_ptr<Card> card, int num = 1);                    //�����Ʒ�������

	void addToDrawPile(std::shared_ptr<Card> card, int num = 1);                //�����Ʒ�����ƶ�

	void addToDiscardPile(std::shared_ptr<Card> card, int num = 1);             //�����Ʒ������ƶ�

	void onDeath(std::shared_ptr<Creature> creature);

	int getRoundNumber() const {                                                //���ص�ǰ�غ���
		return round_;
	}

	void tem_cardPlayed(std::shared_ptr<Card> card);

	std::shared_ptr<Creature> getMonsterPointer(Creature* creature);

	~CombatSystem() {};

	void use_tem_card();
private:
	static CombatSystem* instance_;
	int round_;
};
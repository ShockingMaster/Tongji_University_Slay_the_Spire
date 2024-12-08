#pragma once
/*
 * ս��ϵͳ��Ҫ�����¼�
 */
#include<vector>
#include"cocos2d.h"
#include"Creature.h"
#include"Buff.h"
class CombatDeck;
class CombatSystem
{
public:
	static CombatSystem* getInstance();
	std::vector<std::shared_ptr<Creature>> Monsters_;
	CombatDeck* combatDeck_;

	void onAttack(std::shared_ptr<Creature> attacker, std::shared_ptr<Creature> target,
		int numeric_value_, std::string cardName = "") ;

	void takeDamage(std::shared_ptr<Creature> target, int numeric_value_,
		std::shared_ptr<Creature> attacker = nullptr) ;                         //��Ҫ���Ǿ�����Ҫ����ʲô����

	void startTurn(std::shared_ptr<Creature> creature) ;                        //�غϿ�ʼЧ��

	void endTurn(std::shared_ptr<Creature> creature) ;                          //�غϽ���Ч��

	void cardPlayed(std::shared_ptr<Card> card) ;                               //�������Ч��

	void exhaustCard(std::shared_ptr<Card> card) ;                              //���Ŀ���

	void discardCard(std::shared_ptr<Card> card) ;                              //������������

	void addEnergy(std::shared_ptr<Creature> user, int numeric_value_) ;        //��������

	void addDebuff();                                           //����Debuff

	void addBuff();                                             //����Buff

	~CombatSystem() {};

private:
	static CombatSystem* instance_;
};
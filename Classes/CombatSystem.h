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
	std::queue<std::shared_ptr<Card>> exhaustPile;                              // �����ĵķ�Ĺ�ƶ�

	void init();                                                                //��ʼ��ս��ϵͳ

	std::vector<std::shared_ptr<Card>>& getHandPile() { return hand; }           //�������ƶ�

	void onAttack(std::shared_ptr<Creature> attacker, std::shared_ptr<Creature> target,
		int numeric_value_, std::string cardName = "");

	void combatStart();                                                         //ս����ʼ

	void combatEnd();                                                           //ս��������

	void takeDamage(std::shared_ptr<Creature> target, int numeric_value_,
		std::shared_ptr<Creature> attacker = nullptr);                         //��Ҫ���Ǿ�����Ҫ����ʲô����

	void startTurn(std::shared_ptr<Creature> creature);                        //�غϿ�ʼЧ��

	void endTurn(std::shared_ptr<Creature> creature);                          //�غϽ���Ч��

	void cardPlayed(std::shared_ptr<Card> card);                               //�������Ч��

	void exhaustCard(std::shared_ptr<Card> card);                              //���Ŀ���

	void discardCard(std::shared_ptr<Card> card);                              //������������

	void upgradeCard(std::shared_ptr<Card> card);                               //ս����ʱ����

	void addEnergy(std::shared_ptr<Creature> user, int numeric_value_);        //��������

	void addDebuff();                                                           //����Debuff

	void addBuff();                                                             //����Buff

	void shuffle();                                                             //����ϴ��

	void drawCard(int num);                                                            //���г���

	int getDrawPileNumber();                                                    //��ȡ���ƶѿ�������

	int getHandNumber();                                                        //��ȡ���ƿ�������

	int getDiscardPileNumber();                                                 //��ȡ���ƶѿ�������

	void addToHand(std::shared_ptr<Card> card, int num = 1);                    //�����Ʒ�������

	void addToDrawPile(std::shared_ptr<Card> card, int num = 1);                //�����Ʒ�����ƶ�

	void addToDiscardPile(std::shared_ptr<Card> card, int num = 1);             //�����Ʒ������ƶ�

	template<typename operation_type>
	void chooseTarget(const int num, operation_type operation);                 //ѡ��Ŀ�꿨�ƣ��������������Ļ����Ĳ���

	int getRoundNumber() const {                                                //���ص�ǰ�غ���
		return round_;
	}
	~CombatSystem() {};

private:
	static CombatSystem* instance_;
	int round_;
};

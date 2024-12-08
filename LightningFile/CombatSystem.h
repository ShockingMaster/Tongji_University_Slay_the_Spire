#pragma once
/*
 * 战斗系统需要定义事件
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
		std::shared_ptr<Creature> attacker = nullptr) ;                         //需要考虑具体需要传入什么参数

	void startTurn(std::shared_ptr<Creature> creature) ;                        //回合开始效果

	void endTurn(std::shared_ptr<Creature> creature) ;                          //回合结束效果

	void cardPlayed(std::shared_ptr<Card> card) ;                               //打出卡牌效果

	void exhaustCard(std::shared_ptr<Card> card) ;                              //消耗卡牌

	void discardCard(std::shared_ptr<Card> card) ;                              //主动丢弃卡牌

	void addEnergy(std::shared_ptr<Creature> user, int numeric_value_) ;        //增加能量

	void addDebuff();                                           //增加Debuff

	void addBuff();                                             //增加Buff

	~CombatSystem() {};

private:
	static CombatSystem* instance_;
};
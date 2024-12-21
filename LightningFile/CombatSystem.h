#pragma once
/*
 * 战斗系统需要定义事件
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
	std::vector<std::shared_ptr<Creature>> Monsters_;                           // 怪物
	std::queue<std::shared_ptr<Card>> drawPile;                                 // 抽牌堆
	std::vector<std::shared_ptr<Card>> hand;                                    // 手牌
	std::queue<std::shared_ptr<Card>> discardPile;                              // 弃牌堆
	std::shared_ptr<Card> tem_card;

	void init(int type = -1);                                                                //初始化战斗系统

	void onAttack(std::shared_ptr<Creature> attacker, std::shared_ptr<Creature> target,
		int& numeric_value_, std::string cardName = "", bool isForIntentionUpdate = false);

	void combatStart();                                                         //战斗开始

	void combatEnd();                                                           //战斗结束·

	void takeDamage(std::shared_ptr<Creature> target, int numeric_value_,
		std::shared_ptr<Creature> attacker = nullptr);                         //需要考虑具体需要传入什么参数

	void Addblock(std::shared_ptr<Creature> target, int numeric_value_);

	void exhaustCard(int num, std::string cardName = "");

	void exhaustCard();

	void exhaustCard(std::shared_ptr<Card> card);

	void startTurn(std::shared_ptr<Creature> creature);                        //回合开始效果

	void endTurn(std::shared_ptr<Creature> creature);                          //回合结束效果

	void cardPlayed(std::shared_ptr<Card> card);                               //打出卡牌效果

	void endTurnCardPlayed();                                                 //回合结束卡牌效果

	void cardPlayed(std::shared_ptr<Card> card, std::shared_ptr<Creature> creature);//对于需要目标的卡牌进行重载

	void discardCard(std::shared_ptr<Card> card);                               //主动丢弃卡牌

	void upgradeCard(std::shared_ptr<Card> card);                               //战斗临时升级

	void addEnergy(std::shared_ptr<Creature> user, int numeric_value_);         //增加能量

	void addBuff(std::shared_ptr<Buff> buff, int numeric_value,std::shared_ptr<Creature> target);                //增加Buff

	void addHealth(std::shared_ptr<Creature> target, int numeric_value);       //生命回复

	void shuffleDeck();                                                         //进行洗牌

	void drawCard(int num);                                                     //进行抽牌

	int getDrawPileNumber();                                                    //获取抽牌堆卡牌数量

	int getHandNumber();                                                        //获取手牌卡牌数量

	int getDiscardPileNumber();                                                 //获取弃牌堆卡牌数量

	void addToHand(std::shared_ptr<Card> card, int num = 1);                    //将卡牌放入手牌

	void addToDrawPile(std::shared_ptr<Card> card, int num = 1);                //将卡牌放入抽牌堆

	void addToDiscardPile(std::shared_ptr<Card> card, int num = 1);             //将卡牌放入弃牌堆

	void onDeath(std::shared_ptr<Creature> creature);

	int getRoundNumber() const {                                                //返回当前回合数
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
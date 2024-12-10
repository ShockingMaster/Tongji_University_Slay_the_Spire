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

	void init();                                                                //初始化战斗系统

	void onAttack(std::shared_ptr<Creature> attacker, std::shared_ptr<Creature> target,
		int numeric_value_, std::string cardName = "") ;

	void combatStart();                                                         //战斗开始

	void combatEnd();                                                           //战斗结束·

	void takeDamage(std::shared_ptr<Creature> target, int numeric_value_,
		std::shared_ptr<Creature> attacker = nullptr) ;                         //需要考虑具体需要传入什么参数

	void Addblock(std::shared_ptr<Creature> target, int numeric_value_,
		std::string cardName = "");

	void deleteCard(int num,std::string cardName = "");

	void startTurn(std::shared_ptr<Creature> creature) ;                        //回合开始效果

	void endTurn(std::shared_ptr<Creature> creature) ;                          //回合结束效果

	void cardPlayed(std::shared_ptr<Card> card) ;                               //打出卡牌效果

	void cardPlayed(std::shared_ptr<Card> card, std::shared_ptr<Creature> creature);//对于需要目标的卡牌进行重载

	void exhaustCard(std::shared_ptr<Card> card) ;                              //消耗卡牌

	void discardCard(std::shared_ptr<Card> card) ;                              //主动丢弃卡牌

	void upgradeCard(std::shared_ptr<Card> card);                               //战斗临时升级

	void addEnergy(std::shared_ptr<Creature> user, int numeric_value_) ;        //增加能量

	void addDebuff();                                                           //增加Debuff

	void addBuff();                                                             //增加Buff

	void shuffleDeck();                                                             //进行洗牌

	void drawCard(int num);                                                     //进行抽牌

	int getDrawPileNumber();                                                    //获取抽牌堆卡牌数量

	int getHandNumber();                                                        //获取手牌卡牌数量

	int getDiscardPileNumber();                                                 //获取弃牌堆卡牌数量

	void addToHand(std::shared_ptr<Card> card, int num = 1);                    //将卡牌放入手牌

	void addToDrawPile(std::shared_ptr<Card> card, int num = 1);                //将卡牌放入抽牌堆

	void addToDiscardPile(std::shared_ptr<Card> card, int num = 1);             //将卡牌放入弃牌堆

	template<typename operation_type>
	void chooseTarget(const int num, operation_type operation);                 //选择目标卡牌，进行升级或消耗或丢弃的操作

	int getRoundNumber() const {                                                //返回当前回合数
		return round_;
	}
	~CombatSystem() {};

private:
	static CombatSystem* instance_;
	int round_;
};
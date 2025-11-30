#pragma once
// Refactored with Dependency Injection Pattern
#include<vector>
#include"cocos2d.h"
#include"Creature.h"
#include"Buff.h"
#include <memory>

class CombatDeck;
class CombatScene;
class HandPileLayer;
class IRandomGenerator;
class IEventSystem;
class Player;

/**
 * @brief Combat system with dependency injection support
 *
 * This class has been refactored to support constructor-based dependency injection.
 * Instead of relying on singletons (getInstance()), it now accepts its dependencies
 * through the constructor, following the Dependency Inversion Principle.
 *
 * Benefits:
 * - Easier to test (can inject mock dependencies)
 * - Loose coupling (depends on interfaces, not concrete implementations)
 * - Explicit dependencies (clear from constructor signature)
 */
class CombatSystem
{
public:
	// Singleton pattern (kept for backward compatibility with legacy code)
	static CombatSystem* getInstance();

	// NEW: Constructor injection support for dependency injection
	CombatSystem(std::shared_ptr<IRandomGenerator> randomGen,
	             std::shared_ptr<IEventSystem> eventSystem,
	             std::shared_ptr<Player> player);

	// Default constructor (for singleton)
	CombatSystem();

	std::vector<std::shared_ptr<Creature>> Monsters_;
	std::queue<std::shared_ptr<Card>> drawPile;
	std::vector<std::shared_ptr<Card>> hand;
	std::queue<std::shared_ptr<Card>> discardPile;
	std::shared_ptr<Card> tem_card;

	void init(int type = -1);

	void onAttack(std::shared_ptr<Creature> attacker, std::shared_ptr<Creature> target,
		int& numeric_value_, std::string cardName = "", bool isForIntentionUpdate = false);

	void combatStart();
	void combatEnd();

	void takeDamage(std::shared_ptr<Creature> target, int numeric_value_,
		std::shared_ptr<Creature> attacker = nullptr);

	void Addblock(std::shared_ptr<Creature> target, int numeric_value_);
	void exhaustCard(int num, std::string cardName = "");
	void updateBuff(std::shared_ptr<Creature> creature);
	void exhaustCard();
	void exhaustCard(std::shared_ptr<Card> card);
	void startTurn(std::shared_ptr<Creature> creature);
	void endTurn(std::shared_ptr<Creature> creature);
	void cardPlayed(std::shared_ptr<Card> card);
	void endTurnCardPlayed();
	void cardPlayed(std::shared_ptr<Card> card, std::shared_ptr<Creature> creature);
	void discardCard(std::shared_ptr<Card> card);
	void upgradeCard(std::shared_ptr<Card> card);
	void addEnergy(std::shared_ptr<Creature> user, int numeric_value_);
	void addBuff(std::shared_ptr<Buff> buff, int numeric_value,std::shared_ptr<Creature> target);
	void addHealth(std::shared_ptr<Creature> target, int numeric_value);
	void shuffleDeck();
	void drawCard(int num);

	int getDrawPileNumber();
	int getHandNumber();
	int getDiscardPileNumber();

	void addToHand(std::shared_ptr<Card> card, int num = 1);
	void addToDrawPile(std::shared_ptr<Card> card, int num = 1);
	void addToDiscardPile(std::shared_ptr<Card> card, int num = 1);
	void onDeath(std::shared_ptr<Creature> creature);

	int getRoundNumber() const {
		return round_;
	}

	void tem_cardPlayed(std::shared_ptr<Card> card);
	std::shared_ptr<Creature> getMonsterPointer(Creature* creature);
	void use_tem_card();

	~CombatSystem() {};

private:
	static CombatSystem* instance_;

	// NEW: Injected dependencies
	std::shared_ptr<IRandomGenerator> randomGen_;
	std::shared_ptr<IEventSystem> eventSystem_;
	std::shared_ptr<Player> player_;

	int round_;
	int isLastCombat = 0;
};

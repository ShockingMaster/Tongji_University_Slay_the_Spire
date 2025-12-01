#pragma once
// Refactored with Dependency Injection Pattern and CombatDeck
#include "Buff.h"
#include "Creature.h"
#include "cocos2d.h"
#include <memory>
#include <vector>

class CombatDeck;
class CombatScene;
class HandPileLayer;
class IRandomGenerator;
class IEventSystem;
class Player;
class CombatUIController;
class Card;

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
class CombatSystem {
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
    std::unique_ptr<CombatDeck> deckManager_;
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
    void addBuff(std::shared_ptr<Buff> buff, int numeric_value, std::shared_ptr<Creature> target);
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

    int getRoundNumber() const
    {
        return round_;
    }

    void tem_cardPlayed(std::shared_ptr<Card> card);
    std::shared_ptr<Creature> getMonsterPointer(Creature* creature);
    void use_tem_card();

    // Get CombatDeck instance (for backward compatibility)
    CombatDeck* getDeckManager() const { return deckManager_.get(); }

    // UI Controller related methods
    void setUIController(std::shared_ptr<CombatUIController> uiController);
    void notifyUIPlayerUpdated(std::shared_ptr<Player> player);
    void notifyUIMonstersUpdated(const std::vector<std::shared_ptr<Monster>>& monsters);
    void notifyUICardDrawn(std::shared_ptr<Card> card);
    void notifyUICardRemoved(std::shared_ptr<Card> card);
    void notifyUIDrawPileUpdated(int count);
    void notifyUIDiscardPileUpdated(int count);
    void notifyUIEnergyUpdated(int current, int max);

    // NEW: Additional UI notification methods for complete UI separation
    void notifyUICreaturesUpdated(); // Update all creatures display
    void notifyUIDiscardAllHandCards(); // Discard all hand cards display
    void notifyUIRemoveCardSprite(std::shared_ptr<Card> card); // Remove card sprite by tag
    std::vector<std::shared_ptr<Card>> getSelectedCards(); // Get selected cards from UI

    ~CombatSystem() { };

private:
    static CombatSystem* instance_;

    // NEW: Injected dependencies
    std::shared_ptr<IRandomGenerator> randomGen_;
    std::shared_ptr<IEventSystem> eventSystem_;
    std::shared_ptr<Player> player_;

    // UI控制器（通过facade模式设置）
    std::shared_ptr<CombatUIController> uiController_;

    int round_;
    int isLastCombat = 0;
};

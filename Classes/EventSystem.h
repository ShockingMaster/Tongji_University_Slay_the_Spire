#pragma once
// Refactored with Dependency Injection Pattern
#include "Potion.h"
#include "cocos2d.h"
#include "Player.h"
#include "Relic.h"
#include "Card.h"
#include "CardLayer.h"
#include "IEventSystem.h"
#include <memory>
using namespace std;
using namespace cocos2d;

/**
 * @brief Concrete implementation of IEventSystem
 *
 * This class manages player state and game events.
 * After refactoring, it now implements the IEventSystem interface,
 * allowing it to be injected as a dependency instead of being accessed
 * through a singleton pattern.
 */
class EventSystem : public Node, public IEventSystem {
private:
    static EventSystem* instance;

public:
    int level;
    string name_;
    string character_;
    int health_;
    int fullHealth_;
    int maxPotions_;
    int coins_;
    vector<std::shared_ptr<Potion>> potions_;
    vector<std::shared_ptr<Relic>> relics_;
    vector<std::shared_ptr<Card>> cards_;

    EventSystem();
    ~EventSystem();

    bool init();

    // Singleton pattern (kept for backward compatibility)
    static EventSystem* getInstance();

    // Setup methods
    void setPlayerInfo(const string& name, const string& character, int fullHealth, int coins);
    void setHealth(int health);
    void setCoins(int coins);
    void setPotions(const vector<std::shared_ptr<Potion>>& potions);
    void setLevel(int level);
    void setFullHealth(int health_);

    // Interface implementations (override from IEventSystem)
    int getCurrentHealth() const override;
    int getFullHealth() const override;
    int getCoins() const override;
    void changeHealth(int healthChange) override;
    int changeCoins(int coinChange) override;
    void changeMaxHealth(int maxHealthChange) override;
    void addCard(std::shared_ptr<Card> card) override;
    int addPotion(std::shared_ptr<Potion> potion) override;
    void addRelic(std::shared_ptr<Relic> relic) override;
    int upgradeCard(std::shared_ptr<Card> card) override;
    int deleteCard(std::shared_ptr<Card> card) override;

    // Overloaded methods (non-interface)
    int upgradeCard(Card* card);
    int deleteCard(Card* card);
};

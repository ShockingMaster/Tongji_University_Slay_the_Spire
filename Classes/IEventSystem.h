#pragma once
// Refactored with Dependency Injection Pattern
#include <memory>
#include <string>

class Card;
class Relic;
class Potion;

/**
 * @brief Interface for event system services
 *
 * This interface provides access to player state and game events,
 * decoupling clients from the concrete EventSystem implementation.
 * Follows the Dependency Inversion Principle for better testability.
 */
class IEventSystem {
public:
    virtual ~IEventSystem() = default;

    /**
     * @brief Get current player health
     * @return Current health points
     */
    virtual int getCurrentHealth() const = 0;

    /**
     * @brief Get maximum player health
     * @return Maximum health points
     */
    virtual int getFullHealth() const = 0;

    /**
     * @brief Get current coins/gold
     * @return Number of coins
     */
    virtual int getCoins() const = 0;

    /**
     * @brief Change player health by delta amount
     * @param delta Health change (positive for heal, negative for damage)
     */
    virtual void changeHealth(int delta) = 0;

    /**
     * @brief Change coins by delta amount
     * @param delta Coin change (positive to add, negative to remove)
     * @return New coin amount
     */
    virtual int changeCoins(int delta) = 0;

    /**
     * @brief Change maximum health by delta amount
     * @param delta Max health change
     */
    virtual void changeMaxHealth(int delta) = 0;

    /**
     * @brief Add a card to player's deck
     * @param card Card to add
     */
    virtual void addCard(std::shared_ptr<Card> card) = 0;

    /**
     * @brief Add a potion to player's inventory
     * @param potion Potion to add
     * @return Success status (1 for success, 0 for failure)
     */
    virtual int addPotion(std::shared_ptr<Potion> potion) = 0;

    /**
     * @brief Add a relic to player's collection
     * @param relic Relic to add
     */
    virtual void addRelic(std::shared_ptr<Relic> relic) = 0;

    /**
     * @brief Upgrade a card in player's deck
     * @param card Card to upgrade
     * @return Success status
     */
    virtual int upgradeCard(std::shared_ptr<Card> card) = 0;

    /**
     * @brief Delete a card from player's deck
     * @param card Card to delete
     * @return Success status
     */
    virtual int deleteCard(std::shared_ptr<Card> card) = 0;
};

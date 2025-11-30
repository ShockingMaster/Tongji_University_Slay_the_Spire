#pragma once
// Refactored with Dependency Injection Pattern
#include <memory>
#include <vector>
#include "Enum.h"

class Card;
class Relic;
class Potion;
class Monster;

/**
 * @brief Interface for random generation services
 *
 * This interface decouples clients from the concrete RandomGenerator implementation,
 * allowing for easy testing with mock implementations and following the
 * Dependency Inversion Principle.
 */
class IRandomGenerator {
public:
    virtual ~IRandomGenerator() = default;

    /**
     * @brief Initialize the random generators with a seed string
     * @param seeds Seed string for deterministic generation
     */
    virtual void initializeGenerators(std::string seeds) = 0;

    /**
     * @brief Initialize the random generators with system time
     */
    virtual void initializeGenerators() = 0;

    /**
     * @brief Get the initial seed value
     * @return Initial seed used for generation
     */
    virtual unsigned int getInitialSeed() = 0;

    /**
     * @brief Generate a random number in range [min, max]
     * @param min Minimum value (inclusive)
     * @param max Maximum value (inclusive)
     * @return Random integer in specified range
     */
    virtual int getRandomNumber(int min, int max) = 0;

    /**
     * @brief Get a random card with optional type and rarity filtering
     * @param type Card type filter (default: ALL)
     * @param rarity Card rarity filter (default: ALL)
     * @return Shared pointer to a random card
     */
    virtual std::shared_ptr<Card> getRandomCard(int type = ALL, int rarity = ALL) = 0;

    /**
     * @brief Get a random monster of specified type
     * @param type Monster type (NORMAL, ELITE, BOSS)
     * @return Shared pointer to a random monster
     */
    virtual std::shared_ptr<Monster> getRandomMonster(int type = NORMAL) = 0;

    /**
     * @brief Get a random relic with optional rarity filtering
     * @param rarity Relic rarity filter (default: ALL)
     * @return Shared pointer to a random relic
     */
    virtual std::shared_ptr<Relic> getRandomRelic(int rarity = ALL) = 0;

    /**
     * @brief Get a random potion
     * @return Shared pointer to a random potion
     */
    virtual std::shared_ptr<Potion> getRandomPotion() = 0;

    /**
     * @brief Shuffle a vector in-place
     * @tparam Elemtype Type of elements in the vector
     * @param vec Vector to shuffle
     */
    template<typename Elemtype>
    void shuffleVector(std::vector<Elemtype>& vec);
};

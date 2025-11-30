#pragma once
// Refactored with Dependency Injection Pattern
#include <iostream>
#include <random>
#include <string>
#include <memory>
#include <vector>
#include <time.h>
#include <functional>
#include "Enum.h"
#include "IRandomGenerator.h"

enum RandomType {
    CARD = 0,
    COMBAT = 1,
    OTHER = 2
};

class Card;
class Relic;
class Potion;
class Monster;

/**
 * @brief Concrete implementation of IRandomGenerator
 *
 * This class implements random generation services for the game.
 * After refactoring, it now implements the IRandomGenerator interface,
 * allowing it to be injected as a dependency instead of being accessed
 * through a singleton pattern.
 */
class RandomGenerator : public IRandomGenerator
{
public:
    // Singleton pattern (kept for backward compatibility)
    static RandomGenerator* getInstance();

    // Prevent copy and assignment
    RandomGenerator(const RandomGenerator&) = delete;
    RandomGenerator& operator=(const RandomGenerator&) = delete;

    // Interface implementations (override from IRandomGenerator)
    void initializeGenerators(std::string seeds) override;
    void initializeGenerators() override;
    unsigned int getInitialSeed() override;
    int getRandomNumber(int min, int max) override;
    std::shared_ptr<Card> getRandomCard(int type = ALL, int rarity = ALL) override;
    std::shared_ptr<Monster> getRandomMonster(int type = NORMAL) override;
    std::shared_ptr<Relic> getRandomRelic(int rarity = ALL) override;
    std::shared_ptr<Potion> getRandomPotion() override;

    // Shuffle vector (template method)
    template<typename Elemtype>
    void shuffleVector(std::vector<Elemtype>& vec);

private:
    RandomGenerator() = default;
    unsigned int initialSeed_;
    std::mt19937 rng[3];  // Three separate random number generators
};

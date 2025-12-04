#pragma once
#include "EffectAdapter.h"

/**
 * @brief Strategy to damage player at turn end (for burn card).
 */
class TurnEndDamageEffectAdapter : public EffectAdapter {
public:
    explicit TurnEndDamageEffectAdapter(int damage) : damage_(damage) {}

    void execute(std::shared_ptr<Creature> source = nullptr,
                std::shared_ptr<Creature> target = nullptr,
                std::shared_ptr<Card> card = nullptr,
                int& numeric_value = 0) override {
        CombatSystem::getInstance()->takeDamage(Player::getInstance(), damage_);
    }

private:
    int damage_;
};

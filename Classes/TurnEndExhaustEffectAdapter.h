#pragma once
#include "EffectAdapter.h"

/**
 * @brief Strategy to exhaust the card at turn end (for status cards like dazed).
 */
class TurnEndExhaustEffectAdapter : public EffectAdapter {
public:
    void execute(std::shared_ptr<Creature> source = nullptr,
                std::shared_ptr<Creature> target = nullptr,
                std::shared_ptr<Card> card = nullptr,
                int& numeric_value = 0) override {
        if (card) {
            CombatSystem::getInstance()->exhaustCard(card);
        }
    }
};

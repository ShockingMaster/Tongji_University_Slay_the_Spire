#pragma once
#include "EffectAdapter.h"

/**
 * @brief Strategy for Apotheosis: upgrade all cards in combat.
 */
class ApotheosisEffectAdapter : public EffectAdapter {
public:
    void execute(std::shared_ptr<Creature> source = nullptr,
                std::shared_ptr<Creature> target = nullptr,
                std::shared_ptr<Card> card = nullptr,
                int& numeric_value = 0) override {
        auto combatSystem = CombatSystem::getInstance();
        // Upgrade hand
        for (auto& handCard : combatSystem->hand) {
            combatSystem->upgradeCard(handCard);
        }

        // Upgrade discard pile (preserve order)
        for (int i = 0; i < combatSystem->getDiscardPileNumber(); i++) {
            auto discardCard = combatSystem->discardPile.front();
            combatSystem->upgradeCard(discardCard);
            combatSystem->discardPile.push(discardCard);
            combatSystem->discardPile.pop();
        }

        // Upgrade draw pile (preserve order)
        for (int i = 0; i < combatSystem->getDrawPileNumber(); i++) {
            auto drawCard = combatSystem->drawPile.front();
            combatSystem->upgradeCard(drawCard);
            combatSystem->drawPile.push(drawCard);
            combatSystem->drawPile.pop();
        }
    }
};

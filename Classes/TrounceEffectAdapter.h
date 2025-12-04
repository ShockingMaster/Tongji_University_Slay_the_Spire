#pragma once
#include "EffectAdapter.h"

/**
 * @brief Strategy for Trounce: attack and apply vulnerability.
 */
class TrounceEffectAdapter : public EffectAdapter {
public:
    TrounceEffectAdapter(int baseDamage, int baseVulnerability,
                         int upgradeDamageDelta, int upgradeVulnerabilityDelta)
        : baseDamage_(baseDamage),
          baseVulnerability_(baseVulnerability),
          upgradeDamageDelta_(upgradeDamageDelta),
          upgradeVulnerabilityDelta_(upgradeVulnerabilityDelta) {}

    void execute(std::shared_ptr<Creature> source = nullptr,
                std::shared_ptr<Creature> target = nullptr,
                std::shared_ptr<Card> card = nullptr,
                int& numeric_value = 0) override {
        if (!target) {
            return;
        }
        int damage = baseDamage_;
        int vuln = baseVulnerability_;
        if (card && card->isUpgraded()) {
            damage += upgradeDamageDelta_;
            vuln += upgradeVulnerabilityDelta_;
        }
        CombatSystem::getInstance()->onAttack(Player::getInstance(), target, damage,
                                              card ? card->getName() : "");
        CombatSystem::getInstance()->addBuff(BuffRegistry::createBuff("vulnerability"), vuln, target);
    }

private:
    int baseDamage_;
    int baseVulnerability_;
    int upgradeDamageDelta_;
    int upgradeVulnerabilityDelta_;
};

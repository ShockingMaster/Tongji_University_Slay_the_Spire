#pragma once
#include "EffectAdapter.h"

class AttackEffectAdapter : public EffectAdapter {
public:
    explicit AttackEffectAdapter(int damage) : damage_(damage) {}
    
    void execute(std::shared_ptr<Creature> source = nullptr,
                std::shared_ptr<Creature> target = nullptr,
                std::shared_ptr<Card> card = nullptr,
                int& numeric_value = 0) override {
        if (source && target) {
            CombatSystem::getInstance()->onAttack(source, target, damage_, 
                                               card ? card->getName() : "");
        }
    }
    
private:
    int damage_;
};
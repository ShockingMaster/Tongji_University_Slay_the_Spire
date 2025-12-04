#pragma once
#include "EffectAdapter.h"

class SecondWindEffectAdapter : public EffectAdapter {
public:
    explicit SecondWindEffectAdapter(int blockPerCard) : block_per_card_(blockPerCard) {}
    
    void execute(std::shared_ptr<Creature> source = nullptr,
                std::shared_ptr<Creature> target = nullptr,
                std::shared_ptr<Card> card = nullptr,
                int& numeric_value = 0) override {
        if (source) {
            int num = 0;
            for (size_t i = 0; i < CombatSystem::getInstance()->hand.size(); ++i) {
                const auto& handCard = CombatSystem::getInstance()->hand[i];
                if (handCard->getType() != ATTACK) {
                    ++num;
                    CombatSystem::getInstance()->exhaustCard(i, "Second_wind");
                    i--;
                    CombatSystem::getInstance()->Addblock(source, block_per_card_);
                }
            }
        }
    }
    
private:
    int block_per_card_;
};
#pragma once
#include "EffectAdapter.h"

class BlockEffectAdapter : public EffectAdapter {
public:
    explicit BlockEffectAdapter(int block) : block_(block) {}
    
    void execute(std::shared_ptr<Creature> source = nullptr,
                std::shared_ptr<Creature> target = nullptr,
                std::shared_ptr<Card> card = nullptr,
                int& numeric_value = 0) override {
        if (source) {
            CombatSystem::getInstance()->Addblock(source, block_);
        }
    }
    
private:
    int block_;
};
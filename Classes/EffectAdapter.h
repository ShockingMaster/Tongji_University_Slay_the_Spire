#pragma once
#include "IEffect.h"

class EffectAdapter : public IEffect {
public:
    virtual ~EffectAdapter() = default;
    
    // 默认实现，子类可以选择性重写
    void execute(std::shared_ptr<Creature> source = nullptr,
                std::shared_ptr<Creature> target = nullptr,
                std::shared_ptr<Card> card = nullptr,
                int& numeric_value = 0) override {
        // 默认什么都不做
    }
};
#pragma once
#include "IncludeAll.h"

class IEffect {
public:
    virtual ~IEffect() = default;
    virtual void execute(std::shared_ptr<Creature> source = nullptr,
                       std::shared_ptr<Creature> target = nullptr,
                       std::shared_ptr<Card> card = nullptr,
                       int& numeric_value = 0) = 0;
};
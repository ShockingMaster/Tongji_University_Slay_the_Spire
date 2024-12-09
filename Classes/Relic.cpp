#include "IncludeAll.h"
#include "Enum.h"
#include "Relic.h"

// 构造函数
Relic::Relic(string name, string description, int trigger_type, int priority)
    : name_(std::move(name)), description_(std::move(description)),
    trigger_type_(trigger_type), priority_(priority) {}

// 析构函数
Relic::~Relic() {}

// 优先级比较
bool Relic::operator<(const Relic& other) const {
    return this->priority_ < other.priority_;
}


class BurningBlood : public Relic {
public:
    BurningBlood();

    void onTurnEnd() override;
};
BurningBlood::BurningBlood()
    : Relic("Burning Blood", "Heal 6 HP at the end of your turn if not at max HP", 1, 5) {}

void BurningBlood::onTurnEnd() {
    // 假设 `currentHP` 和 `maxHP` 是 `Creature` 类中的成员变量
        int healAmount = 6;
        
}
AUTO_REGISTER_RELIC(BurningBlood)

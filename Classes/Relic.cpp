#include "IncludeAll.h"
#include "Enum.h"
#include "Relic.h"

// ���캯��
Relic::Relic(string name, string description, int trigger_type, int priority)
    : name_(std::move(name)), description_(std::move(description)),
    trigger_type_(trigger_type), priority_(priority) {}

// ��������
Relic::~Relic() {}

// ���ȼ��Ƚ�
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
    // ���� `currentHP` �� `maxHP` �� `Creature` ���еĳ�Ա����
        int healAmount = 6;
        
}
AUTO_REGISTER_RELIC(BurningBlood)

#include "IncludeAll.h"
#include "Enum.h"
#include "Buff.h"
#include "Creature.h"  


// 生效方法，对对象生效
void Buff::takeEffect(Creature* target) {
    if (target) {
        cout << "Buff " << name << " applied to creature." << endl;
    }
}

// 生效方法：对数值生效
void Buff::takeEffect(int& numericValue) {
    numericValue += this->numericValue;  // 将 Buff 的数值加到目标的属性上
    cout << "Buff " << name << " increased value by " << numericValue << "." << endl;
}

// 更新持续时间
void Buff::updateDuration() {
    if (isActive && duration > 0) {
        --duration;  // 每次调用时减去 1
        if (duration == 0) {
            isActive = false;  // 持续时间结束，Buff失效
            cout << "Buff " << name << " has expired." << endl;
        }
    }
}

class HealingBuff : public Buff
{
public:
    HealingBuff(int healing_value);
    void take_effect(Creature* target)
    {
        target->updateHealth();
        target->updateBuff();
    }
};
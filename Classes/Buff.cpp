#include "IncludeAll.h"
#include "Enum.h"
#include "Buff.h"
#include "Creature.h"  

// ���캯��
Buff::Buff(string name, string description, TriggerType triggerType, int duration, int priority, bool isStackable, int numericValue)
    : name(name), description(description), triggerType(triggerType), duration(duration), priority(priority),
    isStackable(isStackable), numericValue(numericValue), isActive(true) {}

// ��Ч�������Զ�����Ч
void Buff::takeEffect(Creature* target) {
    if (target) {
        cout << "Buff " << name << " applied to creature." << endl;
    }
}

// ��Ч����������ֵ��Ч
void Buff::takeEffect(int& numericValue) {
    numericValue += this->numericValue;  // �� Buff ����ֵ�ӵ�Ŀ���������
    cout << "Buff " << name << " increased value by " << numericValue << "." << endl;
}

// ���³���ʱ��
void Buff::updateDuration() {
    if (isActive && duration > 0) {
        --duration;  // ÿ�ε���ʱ��ȥ 1
        if (duration == 0) {
            isActive = false;  // ����ʱ�������BuffʧЧ
            cout << "Buff " << name << " has expired." << endl;
        }
    }
}

// �Ƚ�����buff�����ȼ�
bool Buff::operator<(const Buff& other) const {
    return priority < other.priority;
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
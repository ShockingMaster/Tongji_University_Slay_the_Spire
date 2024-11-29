#pragma once
#include <iostream>
#include <string>
using namespace std;

class Creature;

// ö�����ͣ�Buff������ʽ
enum class TriggerType {
    ON_HIT,          // ����ʱ����
    ON_DEATH,        // ����ʱ����
    SHUFFLE,         // ϴ��ʱ����
    ON_TURN_START,   // �غϿ�ʼʱ����
    ON_TURN_END      // �غϽ���ʱ����
};

class Buff
{
public:
    // ��Ա����
    string name;           // Buff������
    string description;    // Buff������
    int duration;          // Buff�ĳ���ʱ��
    TriggerType triggerType; // Buff������ʽ
    int priority;          // Buff�Ĵ������ȼ�
    bool isStackable;      // �Ƿ���Ե���
    int numericValue;      // Buff����ֵ���磺��������׼�ȵȣ�
    bool isActive;         // Buff�Ƿ���Ч

    // ���캯��
    Buff(string name, string description, TriggerType triggerType, int duration, int priority, bool isStackable = true, int numericValue = 0)
        : name(name), description(description), triggerType(triggerType), duration(duration), priority(priority),
        isStackable(isStackable), numericValue(numericValue), isActive(true) {}

    // ��Ч����
    virtual void takeEffect(Creature* target);          // ��������Ч
    virtual void takeEffect(int& numericValue);         // ����ֵ��Ч��������ĳЩ���ԣ�

    // ���³���ʱ��
    void updateDuration();

    // ��������
    virtual ~Buff() {}

    // �Ƚ�����buff�����ȼ�
    bool operator<(const Buff& other) const {
        return priority < other.priority;
    }
};

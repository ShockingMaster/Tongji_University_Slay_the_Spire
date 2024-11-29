#pragma once
#include <iostream>
#include <string>
#include <Enum.h>
using namespace std;

class Creature;


class Buff
{
public:
    // ��Ա����
    string name;           // Buff������
    string description;    // Buff������
    int duration;          // Buff�ĳ���ʱ��
    int triggerType;       // Buff������ʽ
    int priority;          // Buff�Ĵ������ȼ�
    bool isStackable;      // �Ƿ���Ե���
    int numericValue;      // Buff����ֵ���磺��������׼�ȵȣ�
    bool isActive;         // Buff�Ƿ���Ч

    // ���캯��
    Buff(string name, string description, int triggerType, int duration, int priority, bool isStackable = true, int numericValue = 0)
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

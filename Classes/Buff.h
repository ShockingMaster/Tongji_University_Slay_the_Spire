#pragma once
#include <iostream>
#include <string>
using namespace std;

class Creature;

// 枚举类型：Buff触发方式
enum class TriggerType {
    ON_HIT,          // 攻击时触发
    ON_DEATH,        // 死亡时触发
    SHUFFLE,         // 洗牌时触发
    ON_TURN_START,   // 回合开始时触发
    ON_TURN_END      // 回合结束时触发
};

class Buff
{
public:
    // 成员变量
    string name;           // Buff的名称
    string description;    // Buff的描述
    int duration;          // Buff的持续时间
    TriggerType triggerType; // Buff触发方式
    int priority;          // Buff的触发优先级
    bool isStackable;      // 是否可以叠加
    int numericValue;      // Buff的数值（如：力量、精准度等）
    bool isActive;         // Buff是否有效

    // 构造函数
    Buff(string name, string description, TriggerType triggerType, int duration, int priority, bool isStackable = true, int numericValue = 0)
        : name(name), description(description), triggerType(triggerType), duration(duration), priority(priority),
        isStackable(isStackable), numericValue(numericValue), isActive(true) {}

    // 生效方法
    virtual void takeEffect(Creature* target);          // 对生物生效
    virtual void takeEffect(int& numericValue);         // 对数值生效（如增加某些属性）

    // 更新持续时间
    void updateDuration();

    // 析构函数
    virtual ~Buff() {}

    // 比较两个buff的优先级
    bool operator<(const Buff& other) const {
        return priority < other.priority;
    }
};

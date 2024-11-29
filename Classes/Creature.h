#pragma once
#include <vector>
#include <string>
#include <iostream>

using namespace std;

class Buff;

class Creature {
public:
    string name_;               // 生物名称
    string description_;        // 生物描述
    int health_;                // 当前生命值
    int fullhealth_;            // 最大生命值
    int block_;                 // 护甲值，减少部分伤害
    int can_end_turn_;          // 是否可以结束回合（可能用于某些状态效果判断）
    vector<Buff*> buffs_;       // 存储生物当前所有的增益/减益效果

    void updateHealth();        // 更新生物的生命值
    void updateBuff();          // 更新生物的增益/减益效果
    void updateblock();         // 更新生物的护甲值
    void addBuff(Buff* buff);   // 为生物添加一个新的 Buff 效果
    void takeDamage(int numeric_value);  // 受到伤害时的效果判定
    void healthLoss(int numeric_value);  // 计算并应用伤害后的生命损失
    void modifyValue(Creature* target, int TriggerType, int& numeric_value); // 对生物的数值进行修改的判定
    void buffSettle(Creature* target, int TriggerType); // 处理生物的 Buff 结算，不涉及数值修改
    void show_info();           // 显示生物的基本信息
    void take_damage(int damage);  // 受到伤害时的处理方法
    virtual void start_turn() = 0; // 开始回合的处理方法（虚函数）
    virtual void end_turn() = 0;   // 结束回合的处理方法（虚函数）
};

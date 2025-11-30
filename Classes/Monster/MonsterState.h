// Refactored with State Pattern
// MonsterState.h
// 怪物状态类 - 管理单个状态的行为和转换规则

#ifndef __MONSTER_STATE_H__
#define __MONSTER_STATE_H__

#include <string>
#include <vector>
#include <memory>
#include <functional>
#include <algorithm>
#include "TransitionCondition.h"

class Creature;

/**
 * @brief 状态转换规则
 *
 * 定义了从当前状态转换到目标状态的条件和优先级
 */
struct StateTransition {
    std::string targetStateName;  // 目标状态名称
    std::shared_ptr<TransitionCondition> condition;  // 转换条件
    int priority;  // 优先级（越大越优先）

    /**
     * @param target 目标状态名称
     * @param cond 转换条件
     * @param pri 优先级（100+=关键, 50-99=重要, 10-49=一般, 0-9=兜底）
     */
    StateTransition(const std::string& target,
                    std::shared_ptr<TransitionCondition> cond,
                    int pri)
        : targetStateName(target), condition(cond), priority(pri) {}
};

/**
 * @brief 怪物状态类
 *
 * 管理单个状态的行为（通过Lambda表达式定义）和转换规则
 * 每个状态包含：
 * 1. 状态名称
 * 2. 行为函数（Lambda）
 * 3. 转换规则列表
 * 4. UI意图信息（图标、攻击次数、攻击值）
 */
class MonsterState {
private:
    std::string stateName_;  // 状态名称
    std::function<void(std::shared_ptr<Creature>)> action_;  // 状态行为（Lambda表达式）
    std::vector<StateTransition> transitions_;  // 状态转换规则列表

    // UI意图信息（用于intentionDisplay）
    std::string intentionIcon_;  // 意图图标路径
    int attackTimes_;           // 攻击次数
    int attackValue_;           // 攻击伤害值

public:
    /**
     * @brief 构造函数
     * @param name 状态名称
     * @param action 状态行为（Lambda表达式）
     *
     * 示例：
     * auto state = std::make_shared<MonsterState>(
     *     "Attack",
     *     [](std::shared_ptr<Creature> monster) {
     *         CombatSystem::getInstance()->onAttack(monster, Player::getInstance(), 12, "");
     *     }
     * );
     */
    MonsterState(const std::string& name,
                 std::function<void(std::shared_ptr<Creature>)> action)
        : stateName_(name),
          action_(action),
          intentionIcon_(""),
          attackTimes_(0),
          attackValue_(0) {}

    /**
     * @brief 执行状态行为
     * @param monster 执行行为的怪物
     */
    void execute(std::shared_ptr<Creature> monster) {
        if (action_) {
            action_(monster);
        }
    }

    /**
     * @brief 检查状态转换条件（按优先级降序）
     * @param monster 要检查的怪物
     * @return 目标状态名称；如果无转换则返回空字符串
     *
     * 该方法会：
     * 1. 按优先级降序排序所有转换规则
     * 2. 逐个检查条件
     * 3. 返回第一个满足条件的目标状态
     */
    std::string checkTransitions(std::shared_ptr<Creature> monster) {
        // 按优先级降序排序
        auto sortedTransitions = transitions_;
        std::sort(sortedTransitions.begin(), sortedTransitions.end(),
                  [](const StateTransition& a, const StateTransition& b) {
                      return a.priority > b.priority;
                  });

        // 检查条件
        for (const auto& trans : sortedTransitions) {
            if (trans.condition && trans.condition->check(monster)) {
                return trans.targetStateName;
            }
        }

        return "";  // 无转换
    }

    /**
     * @brief 添加状态转换规则
     * @param targetState 目标状态名称
     * @param condition 转换条件
     * @param priority 优先级（越大越优先）
     *
     * 优先级建议：
     * - 100+: 关键转换（如半血狂暴）
     * - 50-99: 重要条件（如特定回合）
     * - 10-49: 一般条件（如随机概率）
     * - 0-9: 兜底条件（如默认循环）
     */
    void addTransition(const std::string& targetState,
                       std::shared_ptr<TransitionCondition> condition,
                       int priority) {
        transitions_.emplace_back(targetState, condition, priority);
    }

    /**
     * @brief 设置意图信息（用于UI显示）
     * @param icon 意图图标路径（如"attack.png"）
     * @param times 攻击次数（0表示非攻击行为）
     * @param value 攻击伤害值（0表示非攻击行为）
     *
     * 示例：
     * state->setIntention("attack.png", 1, 12);  // 单次12伤害
     * state->setIntention("attack.png", 3, 10);  // 三连击，每次10伤害
     * state->setIntention("defend.png", 0, 0);   // 防御行为
     */
    void setIntention(const std::string& icon, int times = 0, int value = 0) {
        intentionIcon_ = icon;
        attackTimes_ = times;
        attackValue_ = value;
    }

    // ========== Getters ==========

    /**
     * @brief 获取状态名称
     */
    std::string getName() const {
        return stateName_;
    }

    /**
     * @brief 获取意图图标路径
     */
    std::string getIntentionIcon() const {
        return intentionIcon_;
    }

    /**
     * @brief 获取攻击次数
     */
    int getAttackTimes() const {
        return attackTimes_;
    }

    /**
     * @brief 获取攻击伤害值
     */
    int getAttackValue() const {
        return attackValue_;
    }
};

#endif // __MONSTER_STATE_H__

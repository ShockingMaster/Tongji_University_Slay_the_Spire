// Refactored with State Pattern
// TransitionCondition.h
// 状态转换条件系统 - 定义了5种转换条件类型
// 用于控制怪物在不同状态之间的转换

#ifndef __TRANSITION_CONDITION_H__
#define __TRANSITION_CONDITION_H__

#include <memory>
#include <string>
#include <vector>
#include <random>

// 前向声明
class Creature;
class Monster;
class Player;

/**
 * @brief 状态转换条件基类
 *
 * 所有具体的转换条件都继承自这个基类，实现check()方法来判断条件是否满足
 */
class TransitionCondition {
public:
    virtual ~TransitionCondition() = default;

    /**
     * @brief 检查转换条件是否满足
     * @param monster 要检查的怪物
     * @return true表示条件满足，应该进行状态转换
     */
    virtual bool check(std::shared_ptr<Creature> monster) const = 0;

    /**
     * @brief 获取条件的文本描述（用于调试和文档）
     * @return 条件描述字符串
     */
    virtual std::string describe() const = 0;
};

// ========== 5种具体条件类 ==========

/**
 * @brief 血量条件 - 基于怪物当前血量百分比判断
 *
 * 示例：HealthCondition(0.5f, true) 表示血量 <= 50%
 */
class HealthCondition : public TransitionCondition {
private:
    float percentage_;  // 血量百分比阈值 (0.0~1.0)
    bool isLessThan_;   // true: <=, false: >

public:
    /**
     * @param percentage 血量百分比阈值 (0.0~1.0)
     * @param isLessThan true表示<=判断，false表示>判断
     */
    HealthCondition(float percentage, bool isLessThan);

    bool check(std::shared_ptr<Creature> monster) const override;
    std::string describe() const override;
};

/**
 * @brief 回合数条件 - 基于怪物当前回合数判断
 *
 * 示例：RoundCondition(4, GREATER_EQUAL) 表示回合 >= 4
 */
class RoundCondition : public TransitionCondition {
public:
    enum CompareType {
        EQUAL,          // ==
        GREATER,        // >
        LESS,           // <
        GREATER_EQUAL,  // >=
        LESS_EQUAL      // <=
    };

private:
    int targetRound_;     // 目标回合数
    CompareType compareType_;  // 比较类型

public:
    /**
     * @param round 目标回合数
     * @param type 比较类型
     */
    RoundCondition(int round, CompareType type);

    bool check(std::shared_ptr<Creature> monster) const override;
    std::string describe() const override;
};

/**
 * @brief 概率条件 - 基于随机概率判断
 *
 * 示例：ProbabilityCondition(0.6f) 表示60%概率满足
 */
class ProbabilityCondition : public TransitionCondition {
private:
    float probability_;  // 概率值 (0.0~1.0)

public:
    /**
     * @param prob 概率值 (0.0~1.0)，例如0.6表示60%
     */
    explicit ProbabilityCondition(float prob);

    bool check(std::shared_ptr<Creature> monster) const override;
    std::string describe() const override;
};

/**
 * @brief 玩家状态条件 - 基于玩家的状态（血量/格挡/能量）判断
 *
 * 示例：PlayerStateCondition(HEALTH, 30, true) 表示玩家血量 < 30
 */
class PlayerStateCondition : public TransitionCondition {
public:
    enum StateType {
        HEALTH,  // 生命值
        BLOCK,   // 格挡值
        ENERGY   // 能量值
    };

private:
    StateType stateType_;  // 状态类型
    int threshold_;        // 阈值
    bool isLessThan_;      // true: <, false: >

public:
    /**
     * @param type 状态类型（HEALTH/BLOCK/ENERGY）
     * @param value 阈值
     * @param isLess true表示<判断，false表示>判断
     */
    PlayerStateCondition(StateType type, int value, bool isLess);

    bool check(std::shared_ptr<Creature> monster) const override;
    std::string describe() const override;
};

/**
 * @brief 组合条件 - 将多个条件用AND或OR逻辑组合
 *
 * 示例：
 * auto composite = std::make_shared<CompositeCondition>(AND);
 * composite->addCondition(std::make_shared<HealthCondition>(0.5f, true));
 * composite->addCondition(std::make_shared<RoundCondition>(3, GREATER_EQUAL));
 * // 表示：血量<=50% AND 回合>=3
 */
class CompositeCondition : public TransitionCondition {
public:
    enum LogicType {
        AND,  // 所有条件都满足
        OR    // 至少一个条件满足
    };

private:
    LogicType logic_;  // 逻辑类型
    std::vector<std::shared_ptr<TransitionCondition>> conditions_;  // 子条件列表

public:
    /**
     * @param logic 逻辑类型（AND/OR）
     */
    explicit CompositeCondition(LogicType logic);

    /**
     * @brief 添加子条件
     * @param condition 要添加的条件
     */
    void addCondition(std::shared_ptr<TransitionCondition> condition);

    bool check(std::shared_ptr<Creature> monster) const override;
    std::string describe() const override;
};

#endif // __TRANSITION_CONDITION_H__

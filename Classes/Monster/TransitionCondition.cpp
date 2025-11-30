// Refactored with State Pattern
// TransitionCondition.cpp
// 状态转换条件系统的实现

#include "TransitionCondition.h"
#include "Creature.h"
#include "Monster.h"
#include "Player.h"

// ========== HealthCondition 实现 ==========

HealthCondition::HealthCondition(float percentage, bool isLessThan)
    : percentage_(percentage), isLessThan_(isLessThan) {}

bool HealthCondition::check(std::shared_ptr<Creature> monster) const {
    if (!monster) return false;

    // 计算当前血量百分比
    float currentPercentage = static_cast<float>(monster->getHealth()) /
                              static_cast<float>(monster->getMaxHealth());

    // 根据比较类型判断
    if (isLessThan_) {
        return currentPercentage <= percentage_;
    } else {
        return currentPercentage > percentage_;
    }
}

std::string HealthCondition::describe() const {
    std::string op = isLessThan_ ? "<=" : ">";
    int percentValue = static_cast<int>(percentage_ * 100);
    return "Health " + op + " " + std::to_string(percentValue) + "%";
}

// ========== RoundCondition 实现 ==========

RoundCondition::RoundCondition(int round, CompareType type)
    : targetRound_(round), compareType_(type) {}

bool RoundCondition::check(std::shared_ptr<Creature> monster) const {
    // 需要将Creature转换为Monster才能访问round_num
    auto monsterPtr = std::dynamic_pointer_cast<Monster>(monster);
    if (!monsterPtr) return false;

    int currentRound = monsterPtr->round_num;

    // 根据比较类型判断
    switch (compareType_) {
        case EQUAL:
            return currentRound == targetRound_;
        case GREATER:
            return currentRound > targetRound_;
        case LESS:
            return currentRound < targetRound_;
        case GREATER_EQUAL:
            return currentRound >= targetRound_;
        case LESS_EQUAL:
            return currentRound <= targetRound_;
        default:
            return false;
    }
}

std::string RoundCondition::describe() const {
    std::string op;
    switch (compareType_) {
        case EQUAL: op = "=="; break;
        case GREATER: op = ">"; break;
        case LESS: op = "<"; break;
        case GREATER_EQUAL: op = ">="; break;
        case LESS_EQUAL: op = "<="; break;
        default: op = "?"; break;
    }
    return "Round " + op + " " + std::to_string(targetRound_);
}

// ========== ProbabilityCondition 实现 ==========

ProbabilityCondition::ProbabilityCondition(float prob)
    : probability_(prob) {}

bool ProbabilityCondition::check(std::shared_ptr<Creature> monster) const {
    // 生成随机数
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dist(0.0, 1.0);

    return dist(gen) < probability_;
}

std::string ProbabilityCondition::describe() const {
    int percentValue = static_cast<int>(probability_ * 100);
    return std::to_string(percentValue) + "% Probability";
}

// ========== PlayerStateCondition 实现 ==========

PlayerStateCondition::PlayerStateCondition(StateType type, int value, bool isLess)
    : stateType_(type), threshold_(value), isLessThan_(isLess) {}

bool PlayerStateCondition::check(std::shared_ptr<Creature> monster) const {
    // 获取玩家实例
    auto player = Player::getInstance();
    if (!player) return false;

    int currentValue = 0;

    // 根据状态类型获取相应的值
    switch (stateType_) {
        case HEALTH:
            currentValue = player->getHealth();
            break;
        case BLOCK:
            currentValue = player->getBlockValue();
            break;
        case ENERGY:
            currentValue = player->getCurrentEnergy();
            break;
        default:
            return false;
    }

    // 比较判断
    if (isLessThan_) {
        return currentValue < threshold_;
    } else {
        return currentValue > threshold_;
    }
}

std::string PlayerStateCondition::describe() const {
    std::string stateTypeStr;
    switch (stateType_) {
        case HEALTH: stateTypeStr = "Player Health"; break;
        case BLOCK: stateTypeStr = "Player Block"; break;
        case ENERGY: stateTypeStr = "Player Energy"; break;
        default: stateTypeStr = "Player ?"; break;
    }

    std::string op = isLessThan_ ? "<" : ">";
    return stateTypeStr + " " + op + " " + std::to_string(threshold_);
}

// ========== CompositeCondition 实现 ==========

CompositeCondition::CompositeCondition(LogicType logic)
    : logic_(logic) {}

void CompositeCondition::addCondition(std::shared_ptr<TransitionCondition> condition) {
    if (condition) {
        conditions_.push_back(condition);
    }
}

bool CompositeCondition::check(std::shared_ptr<Creature> monster) const {
    // 空条件列表默认返回true
    if (conditions_.empty()) return true;

    if (logic_ == AND) {
        // AND逻辑：所有条件都必须满足
        for (const auto& cond : conditions_) {
            if (!cond->check(monster)) {
                return false;
            }
        }
        return true;
    } else {
        // OR逻辑：至少一个条件满足
        for (const auto& cond : conditions_) {
            if (cond->check(monster)) {
                return true;
            }
        }
        return false;
    }
}

std::string CompositeCondition::describe() const {
    if (conditions_.empty()) return "Empty Composite";

    std::string result = "(";
    std::string connector = (logic_ == AND) ? " AND " : " OR ";

    for (size_t i = 0; i < conditions_.size(); ++i) {
        result += conditions_[i]->describe();
        if (i < conditions_.size() - 1) {
            result += connector;
        }
    }

    result += ")";
    return result;
}

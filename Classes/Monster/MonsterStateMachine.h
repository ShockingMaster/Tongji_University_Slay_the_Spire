// Refactored with State Pattern
// MonsterStateMachine.h
// 怪物状态机 - 管理所有状态，控制状态转换

#ifndef __MONSTER_STATE_MACHINE_H__
#define __MONSTER_STATE_MACHINE_H__

#include <map>
#include <string>
#include <memory>
#include "MonsterState.h"
#include "cocos2d.h"

class Creature;

/**
 * @brief 怪物状态机
 *
 * 职责：
 * 1. 管理所有状态（通过map存储）
 * 2. 维护当前状态
 * 3. 控制状态转换
 * 4. 提供意图信息给UI
 *
 * 使用流程：
 * 1. 创建状态机：auto sm = std::make_shared<MonsterStateMachine>();
 * 2. 添加状态：sm->addState(state);
 * 3. 设置初始状态：sm->setInitialState("StateName");
 * 4. 每回合执行：sm->execute(monster); sm->updateState(monster);
 */
class MonsterStateMachine {
private:
    std::map<std::string, std::shared_ptr<MonsterState>> states_;  // 所有状态（名称 -> 状态）
    std::shared_ptr<MonsterState> currentState_;  // 当前状态

public:
    /**
     * @brief 构造函数
     */
    MonsterStateMachine() : currentState_(nullptr) {}

    /**
     * @brief 添加状态到状态机
     * @param state 要添加的状态
     *
     * 状态会通过其名称存储在map中，可以通过名称引用
     */
    void addState(std::shared_ptr<MonsterState> state) {
        if (state) {
            states_[state->getName()] = state;
        }
    }

    /**
     * @brief 设置初始状态
     * @param stateName 初始状态的名称
     *
     * 必须在添加状态后调用，否则会失败
     */
    void setInitialState(const std::string& stateName) {
        auto it = states_.find(stateName);
        if (it != states_.end()) {
            currentState_ = it->second;
        } else {
            CCLOG("Warning: Initial state '%s' not found in MonsterStateMachine", stateName.c_str());
        }
    }

    /**
     * @brief 执行当前状态的行为
     * @param monster 执行行为的怪物
     *
     * 通常在takeEffect()中调用
     */
    void execute(std::shared_ptr<Creature> monster) {
        if (!currentState_) {
            CCLOG("Warning: No current state in MonsterStateMachine");
            return;
        }
        currentState_->execute(monster);
    }

    /**
     * @brief 检查并更新状态
     * @param monster 要检查的怪物
     *
     * 流程：
     * 1. 调用当前状态的checkTransitions()
     * 2. 如果有转换，切换到目标状态
     *
     * 通常在execute()之后调用
     */
    void updateState(std::shared_ptr<Creature> monster) {
        if (!currentState_) return;

        // 检查转换条件
        std::string nextStateName = currentState_->checkTransitions(monster);

        // 如果有有效的下一状态，进行转换
        if (!nextStateName.empty()) {
            auto it = states_.find(nextStateName);
            if (it != states_.end()) {
                CCLOG("Monster state transition: %s -> %s",
                      currentState_->getName().c_str(),
                      nextStateName.c_str());
                currentState_ = it->second;
            } else {
                CCLOG("Warning: Target state '%s' not found", nextStateName.c_str());
            }
        }
    }

    /**
     * @brief 获取当前状态的意图图标
     * @return 意图图标路径（用于intentionDisplay）
     */
    std::string getIntentionIcon() const {
        if (currentState_) {
            return currentState_->getIntentionIcon();
        }
        return "";
    }

    /**
     * @brief 获取当前状态的攻击信息
     * @param times 输出参数：攻击次数
     * @param value 输出参数：攻击伤害值
     *
     * 用于intentionDisplay()中设置attack_times和attack_numeric_value
     */
    void getAttackInfo(int& times, int& value) const {
        if (currentState_) {
            times = currentState_->getAttackTimes();
            value = currentState_->getAttackValue();
        } else {
            times = 0;
            value = 0;
        }
    }

    /**
     * @brief 获取当前状态名称（用于调试）
     * @return 当前状态名称；如果无状态则返回"None"
     */
    std::string getCurrentStateName() const {
        if (currentState_) {
            return currentState_->getName();
        }
        return "None";
    }

    /**
     * @brief 检查状态机是否已初始化
     * @return true表示有当前状态
     */
    bool isInitialized() const {
        return currentState_ != nullptr;
    }
};

#endif // __MONSTER_STATE_MACHINE_H__

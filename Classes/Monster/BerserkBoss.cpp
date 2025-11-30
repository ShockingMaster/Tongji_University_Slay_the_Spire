// Refactored with State Pattern
// BerserkBoss.cpp
// 示例怪物：半血狂暴Boss
//
// 行为逻辑：
// - 血量 > 50%: 正常阶段（80%攻击12伤害，20%防御15格挡）
// - 血量 ≤ 50%: 立即进入狂暴阶段（获得5层力量Buff）
// - 狂暴后: 70%攻击20伤害，30%三连击（3x10伤害）

#include "Monster.h"
#include "MonsterStateMachine.h"
#include "MonsterState.h"
#include "TransitionCondition.h"
#include "CombatSystem.h"
#include "Player.h"
#include "Register.h"
#include "Enum.h"

/**
 * @brief 半血狂暴Boss - State模式示例
 *
 * 状态转换图：
 * [NormalAttack] ─80%→ [NormalAttack]
 *      │                     │
 *      20%                HP≤50% (优先级100)
 *      ↓                     ↓
 * [NormalDefend]────────>[BerserkEntry]──100%─>[BerserkAttack]
 *                                                    │      ↑
 *                                                    70%    │30%
 *                                                    ↓      │
 *                                               [BerserkMulti]
 */
class BerserkBoss : public Monster {
public:
    BerserkBoss() : Monster(BOSS, 250, "BerserkBoss", 1) {
        initializeStateMachine();
    }

    /**
     * @brief 初始化状态机
     *
     * 创建所有状态并设置转换规则
     */
    void initializeStateMachine() {
        stateMachine_ = std::make_shared<MonsterStateMachine>();

        // ===== 正常阶段（血量 > 50%）=====

        // 1. 正常攻击状态
        auto normalAttack = std::make_shared<MonsterState>(
            "NormalAttack",
            [](std::shared_ptr<Creature> monster) {
                int damage = 12;
                CombatSystem::getInstance()->onAttack(
                    monster,
                    Player::getInstance(),
                    damage,
                    ""
                );
            }
        );
        normalAttack->setIntention("attack.png", 1, 12);

        // 转换规则：
        // - 优先级100: 血量≤50%立即狂暴（最高优先级）
        // - 优先级10: 20%概率去防御
        // - 优先级5: 80%继续攻击（兜底）
        normalAttack->addTransition("BerserkEntry",
            std::make_shared<HealthCondition>(0.5f, true), 100);
        normalAttack->addTransition("NormalDefend",
            std::make_shared<ProbabilityCondition>(0.2f), 10);
        normalAttack->addTransition("NormalAttack",
            std::make_shared<ProbabilityCondition>(1.0f), 5);

        // 2. 正常防御状态
        auto normalDefend = std::make_shared<MonsterState>(
            "NormalDefend",
            [](std::shared_ptr<Creature> monster) {
                CombatSystem::getInstance()->Addblock(monster, 15);
            }
        );
        normalDefend->setIntention("defend.png", 0, 0);

        // 转换规则：
        // - 优先级100: 血量≤50%立即狂暴
        // - 优先级5: 100%回到攻击
        normalDefend->addTransition("BerserkEntry",
            std::make_shared<HealthCondition>(0.5f, true), 100);
        normalDefend->addTransition("NormalAttack",
            std::make_shared<ProbabilityCondition>(1.0f), 5);

        // ===== 狂暴阶段（血量 ≤ 50%）=====

        // 3. 狂暴入场（只执行一次）
        auto berserkEntry = std::make_shared<MonsterState>(
            "BerserkEntry",
            [](std::shared_ptr<Creature> monster) {
                // 获得5层力量Buff
                auto strengthBuff = BuffRegistry::createBuff("StrengthBuff");
                CombatSystem::getInstance()->addBuff(strengthBuff, 5, monster);
            }
        );
        berserkEntry->setIntention("defendBuff.png", 0, 0);

        // 转换规则：100%去狂暴攻击
        berserkEntry->addTransition("BerserkAttack",
            std::make_shared<ProbabilityCondition>(1.0f), 5);

        // 4. 狂暴攻击（高伤害）
        auto berserkAttack = std::make_shared<MonsterState>(
            "BerserkAttack",
            [](std::shared_ptr<Creature> monster) {
                int damage = 20;
                CombatSystem::getInstance()->onAttack(
                    monster,
                    Player::getInstance(),
                    damage,
                    ""
                );
            }
        );
        berserkAttack->setIntention("attack.png", 1, 20);

        // 转换规则：
        // - 优先级10: 30%三连击
        // - 优先级5: 70%继续攻击
        berserkAttack->addTransition("BerserkMulti",
            std::make_shared<ProbabilityCondition>(0.3f), 10);
        berserkAttack->addTransition("BerserkAttack",
            std::make_shared<ProbabilityCondition>(1.0f), 5);

        // 5. 狂暴三连击
        auto berserkMulti = std::make_shared<MonsterState>(
            "BerserkMulti",
            [](std::shared_ptr<Creature> monster) {
                // 3次攻击，每次10伤害
                for (int i = 0; i < 3; i++) {
                    int damage = 10;
                    CombatSystem::getInstance()->onAttack(
                        monster,
                        Player::getInstance(),
                        damage,
                        ""
                    );
                }
            }
        );
        berserkMulti->setIntention("attack.png", 3, 10);

        // 转换规则：100%回到狂暴攻击
        berserkMulti->addTransition("BerserkAttack",
            std::make_shared<ProbabilityCondition>(1.0f), 5);

        // 添加所有状态到状态机
        stateMachine_->addState(normalAttack);
        stateMachine_->addState(normalDefend);
        stateMachine_->addState(berserkEntry);
        stateMachine_->addState(berserkAttack);
        stateMachine_->addState(berserkMulti);

        // 设置初始状态
        stateMachine_->setInitialState("NormalAttack");
    }

    /**
     * @brief 执行怪物行动（重写）
     *
     * State模式下的标准实现：
     * 1. 先执行当前状态的行为
     * 2. 再检查并更新状态
     */
    void takeEffect() override {
        // 获取怪物自身的shared_ptr
        auto thisMonster = CombatSystem::getInstance()->getMonsterPointer(this);

        if (!thisMonster) {
            CCLOG("Error: Failed to get monster pointer in BerserkBoss::takeEffect");
            return;
        }

        // 确保状态机已初始化
        if (!stateMachine_ || !stateMachine_->isInitialized()) {
            CCLOG("Error: StateMachine not initialized in BerserkBoss");
            return;
        }

        // 1. 执行当前状态的行为
        stateMachine_->execute(thisMonster);

        // 2. 检查并更新状态
        stateMachine_->updateState(thisMonster);
    }

    /**
     * @brief 显示意图（重写）
     *
     * State模式下的标准实现：
     * 1. 获取状态机中的攻击信息
     * 2. 如果有攻击，预计算实际伤害（考虑Buff影响）
     * 3. 返回意图图标
     */
    std::string intentionDisplay() override {
        // 获取怪物自身的shared_ptr
        auto thisMonster = CombatSystem::getInstance()->getMonsterPointer(this);

        if (!thisMonster || !stateMachine_ || !stateMachine_->isInitialized()) {
            return "";
        }

        // 获取攻击信息
        stateMachine_->getAttackInfo(attack_times, attack_numeric_value);

        // 如果有攻击，预计算实际伤害（考虑Buff影响）
        if (attack_numeric_value > 0) {
            int damage = attack_numeric_value;
            CombatSystem::getInstance()->onAttack(
                thisMonster,
                Player::getInstance(),
                damage,
                "",
                true  // true表示仅用于意图更新，不造成实际伤害
            );
            attack_numeric_value = damage;  // 更新为考虑Buff后的伤害
        }

        // 返回意图图标
        return stateMachine_->getIntentionIcon();
    }
};

// 注册怪物到怪物注册表
AUTO_REGISTER_MONSTER(BerserkBoss)

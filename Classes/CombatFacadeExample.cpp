#include "CombatFacade.h"
#include "cocos2d.h"
#include "Player.h"
#include "Monster.h"
#include "Card.h"

USING_NS_CC;

/**
 * @brief CombatFacade使用示例
 *
 * 本文件展示了如何使用Facade模式重构后的CombatFacade来替代直接调用CombatSystem。
 * 通过使用CombatFacade，客户端代码可以：
 * 1. 简化API调用（只需了解少量方法）
 * 2. 避免直接操作UI层
 * 3. 自动获得事件通知支持
 * 4. 降低代码耦合度
 */

// 示例1：基本战斗流程
void exampleBasicCombatFlow() {
    // 1. 创建CombatFacade
    auto combatFacade = CombatFacade::create();
    if (!combatFacade) {
        CCLOG("Error: Failed to create CombatFacade");
        return;
    }

    // 2. 启动战斗
    combatFacade->startCombat(-1);  // 随机战斗

    // 3. 玩家回合开始
    combatFacade->startPlayerTurn();

    // 4. 出牌（无目标卡牌）
    // 假设第0张牌是攻击牌
    if (combatFacade->canPlayCard(0)) {
        combatFacade->playCard(0);
    }

    // 5. 出牌（有目标卡牌）
    // 假设第1张牌是技能牌，需要选择目标
    auto monsters = combatFacade->getMonsters();
    if (!monsters.empty() && combatFacade->canPlayCard(1)) {
        combatFacade->playCardWithTarget(1, 0);  // 对第一个怪物使用
    }

    // 6. 结束玩家回合
    combatFacade->endPlayerTurn();

    // 7. 检查战斗是否结束
    if (combatFacade->isCombatEnded()) {
        CCLOG("Combat ended!");
        combatFacade->endCombat();
    }
}

// 示例2：事件监听
void exampleEventListening() {
    auto combatFacade = CombatFacade::create();

    // 监听卡牌被使用事件
    combatFacade->addEventListener(CombatEventType::CARD_PLAYED, [](const CombatEventBus::Event& event) {
        if (event.data) {
            auto cardData = std::static_pointer_cast<CardEventData>(event.data);
            if (cardData) {
                CCLOG("事件：卡牌 %s 被使用", cardData->cardName.c_str());
            }
        }
    });

    // 监听攻击事件
    combatFacade->addEventListener(CombatEventType::ATTACK, [](const CombatEventBus::Event& event) {
        if (event.data) {
            auto attackData = std::static_pointer_cast<AttackEventData>(event.data);
            if (attackData) {
                CCLOG("事件：%s 攻击 %s，造成 %d 伤害",
                      attackData->attackerName.c_str(),
                      attackData->targetName.c_str(),
                      attackData->actualDamage);
            }
        }
    });

    // 监听回合事件
    combatFacade->addEventListener(CombatEventType::TURN_START, [](const CombatEventBus::Event& event) {
        if (event.data) {
            auto turnData = std::static_pointer_cast<TurnEventData>(event.data);
            if (turnData) {
                CCLOG("事件：%s 回合开始 (第%d回合)",
                      turnData->isPlayerTurn ? "玩家" : "怪物",
                      turnData->roundNumber);
            }
        }
    });
}

// 示例3：替代传统CombatSystem调用
void exampleReplaceOldAPI() {
    auto combatFacade = CombatFacade::create();

    // ========== 传统方式（直接调用CombatSystem） ==========
    // CombatSystem::getInstance()->onAttack(Player::getInstance(), monster, damage);
    // HandPileLayer::getInstance()->removeCard(card);
    // scene->creatureLayer->updateDisplay();

    // ========== 新方式（使用CombatFacade） ==========
    combatFacade->startCombat();

    // 直接调用简化API，无需关心UI更新
    combatFacade->playerAttack(0, 15);  // 对第0个怪物造成15伤害

    // 手牌管理也变得更简单
    combatFacade->drawCard(3);  // 抽3张牌
    combatFacade->discardCard(0);  // 弃掉第0张牌

    // 防御和治疗
    combatFacade->playerDefend(10);  // 加10护甲
    combatFacade->playerHeal(5);  // 治疗5点

    // 状态管理
    combatFacade->playerAddBuff("Strength", 2);  // 加力量buff
    combatFacade->monsterAddBuff(0, "Vulnerable", 1);  // 给怪物加易伤

    // 查询状态
    auto player = combatFacade->getPlayer();
    auto monsters = combatFacade->getMonsters();
    auto hand = combatFacade->getPlayerHand();

    CCLOG("玩家血量：%d/%d", player->getHealth(), player->getMaxHealth());
    CCLOG("怪物数量：%zu", monsters.size());
    CCLOG("手牌数量：%zu", hand.size());
    CCLOG("当前回合：%d", combatFacade->getCurrentRound());
}

// 示例4：UI层解耦
void exampleUIDecoupling() {
    // 传统方式：需要直接从CombatSystem操作UI
    // auto scene = (CombatScene*)Director::getInstance()->getRunningScene();
    // scene->creatureLayer->updateDisplay();
    // HandPileLayer::getInstance()->updateDrawPileDisplay();

    // 新方式：UI更新由CombatFacade自动处理
    auto combatFacade = CombatFacade::create();

    combatFacade->startCombat();
    combatFacade->startPlayerTurn();

    // 所有UI更新都在内部自动完成
    combatFacade->drawCard(5);
    combatFacade->playCard(0);
    combatFacade->endPlayerTurn();

    // UI状态查询（只读，不修改）
    CCLOG("抽牌堆：%d", combatFacade->getDrawPileSize());
    CCLOG("弃牌堆：%d", combatFacade->getDiscardPileSize());
    CCLOG("手牌：%d", combatFacade->getHandSize());
    CCLOG("当前回合：%d", combatFacade->getCurrentRound());
}

// 示例5：完整的战斗流程
void exampleCompleteBattle() {
    auto combatFacade = CombatFacade::create();
    combatFacade->startCombat();  // 启动战斗

    // 事件监听
    combatFacade->addEventListener(CombatEventType::COMBAT_END, [](const CombatEventBus::Event& event) {
        CCLOG("战斗结束！");
    });

    // 战斗循环
    while (!combatFacade->isCombatEnded()) {
        if (combatFacade->isPlayerTurn()) {
            // 玩家回合逻辑
            CCLOG("玩家回合开始");

            // 出牌策略：简化处理，出第一张能出的牌
            auto handSize = combatFacade->getHandSize();
            for (int i = 0; i < handSize; i++) {
                if (combatFacade->canPlayCard(i)) {
                    auto cardCost = combatFacade->getCardEnergyCost(i);
                    auto monsters = combatFacade->getMonsters();

                    // 简单AI：有目标就出有目标的牌，否则出无目标的牌
                    if (!monsters.empty()) {
                        combatFacade->playCardWithTarget(i, 0);
                    } else {
                        combatFacade->playCard(i);
                    }
                    break;
                }
            }

            // 结束回合
            combatFacade->endPlayerTurn();
        } else {
            // 怪物回合由系统自动处理
            CCLOG("怪物回合");
            // 无需手动处理，系统会自动执行所有怪物的AI
        }
    }

    combatFacade->endCombat();
}

// 示例6：测试代码示例
void exampleTestingWithFacade() {
    // 测试时可以使用mock CombatFacade
    // 这使得单元测试更加容易，因为不需要真实的UI层

    auto combatFacade = CombatFacade::create();
    combatFacade->startCombat();

    // 验证初始化状态
    assert(combatFacade->getPlayer() != nullptr);
    assert(combatFacade->getMonsters().size() > 0);

    // 验证回合流程
    assert(combatFacade->isPlayerTurn() == false);  // 初始不是玩家回合
    combatFacade->startCombat();  // 重新开始以确保是玩家回合

    // 验证卡牌操作
    int initialHandSize = combatFacade->getHandSize();
    combatFacade->drawCard(1);
    assert(combatFacade->getHandSize() == initialHandSize + 1);

    // 验证战斗交互
    auto monsters = combatFacade->getMonsters();
    if (!monsters.empty()) {
        int initialMonsterHealth = monsters[0]->getHealth();
        combatFacade->playerAttack(0, 10);
        CCLOG("Monster health after attack: %d", monsters[0]->getHealth());
    }

    CCLOG("All tests passed!");
}

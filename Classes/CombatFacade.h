#pragma once

#include "cocos2d.h"
#include <memory>
#include <vector>

// Forward declarations
class Player;
class Monster;
class Card;
class Buff;
class Creature;
class CombatSystem;
class CombatUIController;
class CombatEventBus;

/**
 * @brief CombatFacade - Combat System Facade Interface
 *
 * Provides a unified entry point for the combat system, hiding complex subsystem interactions (CombatSystem, UI, Event System),
 * offering a simplified, easy-to-use API for client code, reducing coupling.
 *
 * Design Principles:
 * 1. Single Responsibility: Only responsible for flow orchestration, not specific business logic
 * 2. Open/Closed: Open for extension, closed for modification
 * 3. Dependency Inversion: Depend on abstractions, not concrete implementations
 * 4. Interface Segregation: Provide minimal necessary interfaces
 *
 * Usage Example:
 * @code
 *   auto facade = CombatFacade::create();
 *   facade->startCombat();
 *   facade->playCard(0);  // Play the first card
 *   facade->endPlayerTurn();
 * @endcode
 */
class CombatFacade {
public:
    /// @brief Create CombatFacade instance
    /// @return Unique pointer managing the CombatFacade instance
    static std::unique_ptr<CombatFacade> create();

    /// @brief Destructor
    virtual ~CombatFacade();

    // ==================== Combat Flow Control ====================

    /// @brief Start combat
    /// @param combatType Combat type (-1 random, normal, elite, BOSS, etc.)
    /// @note Internally calls CombatSystem::init(), initializes player, deck, monsters
    void startCombat(int combatType = -1);

    /// @brief End combat
    /// @note Cleans up combat state, may show settlement screen
    void endCombat();

    /// @brief Start player turn
    /// @note Internally calls startTurn(Player), resets energy, triggers turn start effects
    void startPlayerTurn();

    /// @brief End player turn
    /// @note Internally calls endTurn(Player), switches to monster turn
    void endPlayerTurn();

    /// @brief Start monster turn
    /// @note Internally loops through all alive monsters, executes AI behaviors
    void startMonsterTurn();

    /// @brief Check if combat has ended
    /// @return true if player died or all monsters died
    bool isCombatEnded() const;

    // ==================== Card Operations ====================

    /// @brief Play card (no target)
    /// @param handIndex Hand card index
    /// @return Whether card was played successfully (may fail due to insufficient energy, etc.)
    bool playCard(int handIndex);

    /// @brief Play card (with target)
    /// @param handIndex Hand card index
    /// @param targetIndex Target monster index
    /// @return Whether card was played successfully
    bool playCardWithTarget(int handIndex, int targetIndex);

    /// @brief Draw cards
    /// @param count Number of cards to draw (default 1)
    /// @return Actual number of cards drawn
    int drawCard(int count = 1);

    /// @brief Discard card
    /// @param handIndex Hand card index
    /// @note Automatically moves card to discard pile
    void discardCard(int handIndex);

    /// @brief Exhaust card (remove from game, does not go to discard pile)
    /// @param handIndex Hand card index
    void exhaustCard(int handIndex);

    // ==================== Combat Interaction ====================

    /// @brief 玩家攻击
    /// @param targetIndex 目标怪物索引
    /// @param damage 基础伤害值
    /// @return 实际造成的伤害（考虑buff等）
    int playerAttack(int targetIndex, int damage);

    /// @brief 玩家防御（加护甲）
    /// @param blockAmount 护甲值
    void playerDefend(int blockAmount);

    /// @brief 对玩家造成伤害
    /// @param damage 伤害值
    /// @param attacker 攻击者（可选）
    /// @return 实际受到伤害（考虑护甲、buff等）
    int playerTakeDamage(int damage, Creature* attacker = nullptr);

    /// @brief 对怪物造成伤害
    /// @param targetIndex 目标怪物索引
    /// @param damage 伤害值
    /// @param attacker 攻击者（可选）
    /// @return 实际受到伤害
    int monsterTakeDamage(int targetIndex, int damage, Creature* attacker = nullptr);

    /// @brief 为玩家添加buff
    /// @param buffName buff名称
    /// @param stacks 层数（默认1）
    void playerAddBuff(const std::string& buffName, int stacks = 1);

    /// @brief 为怪物添加buff
    /// @param targetIndex 目标怪物索引
    /// @param buffName buff名称
    /// @param stacks 层数（默认1）
    void monsterAddBuff(int targetIndex, const std::string& buffName, int stacks = 1);

    /// @brief 玩家治疗
    /// @param healAmount 治疗量
    void playerHeal(int healAmount);

    // ==================== 查询接口（只读） ====================

    /// @brief 获取玩家实例
    /// @return 玩家智能指针
    std::shared_ptr<Player> getPlayer() const;

    /// @brief 获取怪物列表
    /// @return 怪物智能指针向量
    std::vector<std::shared_ptr<Monster>> getMonsters() const;

    /// @brief 获取怪物列表（作为Creature类型引用，用于CreatureLayer等需要引用的场景）
    /// @return Creature智能指针向量的引用
    std::vector<std::shared_ptr<Creature>>& getMonstersAsCreatures();

    /// @brief 获取玩家手牌
    /// @return 手牌智能指针向量
    std::vector<std::shared_ptr<Card>> getPlayerHand() const;

    /// @brief 获取抽牌堆数量
    int getDrawPileSize() const;

    /// @brief 获取弃牌堆数量
    int getDiscardPileSize() const;

    /// @brief 获取手牌数量
    int getHandSize() const;

    /// @brief 获取当前回合数
    int getCurrentRound() const;

    /// @brief 检查是否是玩家回合
    bool isPlayerTurn() const;

    /// @brief 检查玩家是否可以出牌
    /// @param handIndex 手牌索引
    /// @return true if 可以出牌
    bool canPlayCard(int handIndex) const;

    /// @brief 获取卡牌能量消耗
    /// @param handIndex 手牌索引
    /// @return 能量消耗（-1 if 无效索引）
    int getCardEnergyCost(int handIndex) const;

    // ==================== 事件监听 ====================

    /// @brief 添加战斗事件监听器
    /// @param eventType 事件类型
    /// @param callback 回调函数
    /// @note 事件类型包括：CARD_PLAYED, ATTACK, DAMAGE_TAKEN, TURN_START, TURN_END等
    template <typename Func>
    void addEventListener(int eventType, Func&& callback)
    {
        if (eventBus_) {
            eventBus_->addListener(eventType, std::forward<Func>(callback));
        }
    }

    /// @brief 移除战斗事件监听器
    /// @param eventType 事件类型
    void removeEventListener(int eventType);

protected:
    /// @brief 构造函数（保护，使用create创建）
    CombatFacade();

    /// @brief Initialize CombatFacade
    /// @return true if initialization is successful
    bool init();

private:
    // Internal subsystems
    // Raw pointers to reference Singleton instances (Facade does not own these)
    CombatSystem* combatSystem_;
    std::shared_ptr<CombatUIController> uiController_;
    CombatEventBus* eventBus_; // Raw pointer - singleton, not owned by Facade

    // State flags
    bool isInitialized_ = false;
    bool isCombatActive_ = false;
    bool isPlayerTurn_ = false;
    int currentRound_ = 0;

    // Disable copy and assignment
    CombatFacade(const CombatFacade&) = delete;
    CombatFacade& operator=(const CombatFacade&) = delete;
};

/**
 * @brief Combat Event Type Constants
 * @note Clients can register event listeners using these constants
 */
namespace CombatEventType {
const int CARD_PLAYED = 1001; /// Card played
const int ATTACK = 1002; /// Attack action
const int DAMAGE_TAKEN = 1003; /// Damage taken
const int BUFF_ADDED = 1004; /// Buff added
const int TURN_START = 1005; /// Turn start
const int TURN_END = 1006; /// Turn end
const int COMBAT_START = 1007; /// Combat start
const int COMBAT_END = 1008; /// Combat end
const int PLAYER_DEFEATED = 1009; /// Player defeated
const int MONSTER_DEFEATED = 1010; /// Monster defeated
}

/**
 * @brief Combat State Enum
 * @note Used to query current combat state
 */
enum class CombatState {
    NOT_STARTED, /// Combat not started
    PLAYER_TURN, /// Player turn
    MONSTER_TURN, /// Monster turn
    COMBAT_ENDED, /// Combat ended
    VICTORY, /// Victory
    DEFEAT /// Defeat
};

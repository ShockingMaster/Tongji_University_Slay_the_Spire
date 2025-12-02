#pragma once

#include <functional>
#include <map>
#include <vector>
#include <memory>
#include <mutex>
#include <atomic>
#include "cocos2d.h"

/**
 * @brief CombatEventBus - 战斗事件总线（发布-订阅模式）
 *
 * 解耦战斗系统中各组件的通信，提供事件驱动的交互方式。
 * 替代传统的直接方法调用，降低耦合度，提高扩展性。
 *
 * 设计特点：
 * 1. 单例模式：全局唯一的事件总线实例
 * 2. 线程安全：使用互斥锁保护事件监听器列表
 * 3. 高性能：事件同步传递，支持优先级
 * 4. 类型安全：使用模板支持类型安全的事件数据
 *
 * 使用示例：
 * @code
 *   // 注册监听器
 *   CombatEventBus::getInstance()->addListener(CARD_PLAYED, [](Event* e) {
 *       CCLOG("卡牌被使用");
 *   });
 *
 *   // 发布事件
 *   CombatEventBus::getInstance()->publish(CARD_PLAYED, eventData);
 * @endcode
 *
 * @note 这个EventBus是受控的单例（Controlled Singleton），
 *       后续可以通过依赖注入替换。
 */
class CombatEventBus
{
public:
    /// @brief 获取事件总线单例（受控单例）
    /// @return 事件总线实例引用
    static CombatEventBus* getInstance();

    /// @brief 销毁事件总线（主要用于测试）
    static void destroyInstance();

    /// @brief 事件数据结构体
    struct Event {
        int type;                          /// 事件类型（使用CombatEventType常量）
        std::shared_ptr<void> data;        /// 事件数据（泛型，使用shared_ptr<void>）
        cocos2d::Vec2 position;           /// 事件位置（可选）
        std::chrono::steady_clock::time_point timestamp; /// 时间戳

        Event(int t) : type(t), timestamp(std::chrono::steady_clock::now()) {}
        Event(int t, std::shared_ptr<void> d) : type(t), data(d), timestamp(std::chrono::steady_clock::now()) {}
    };

    /// @brief 事件监听器函数类型
    using EventListener = std::function<void(const Event&)>;

    // ==================== 监听器管理 ====================

    /// @brief 添加事件监听器
    /// @param eventType 事件类型
    /// @param listener 监听器函数
    /// @param priority 优先级（数字越大优先级越高，默认0）
    /// @return 监听器ID（用于移除）
    int addListener(int eventType, EventListener listener, int priority = 0);

    /// @brief 一次性监听器（执行一次后自动移除）
    /// @param eventType 事件类型
    /// @param listener 监听器函数
    /// @return 监听器ID
    int addOnceListener(int eventType, EventListener listener);

    /// @brief 移除监听器
    /// @param listenerId 监听器ID（addListener返回值）
    /// @return true if 移除成功
    bool removeListener(int listenerId);

    /// @brief 移除指定事件类型的所有监听器
    /// @param eventType 事件类型
    /// @return 移除的监听器数量
    int removeListeners(int eventType);

    /// @brief 清空所有监听器
    void clearAllListeners();

    /// @brief 获取监听器数量
    /// @param eventType 事件类型（-1表示所有）
    /// @return 监听器数量
    int getListenerCount(int eventType = -1) const;

    // ==================== 事件发布 ====================

    /// @brief 发布事件（同步）
    /// @param eventType 事件类型
    /// @param data 事件数据
    /// @note 同步调用所有监听器，注意性能影响
    void publish(int eventType, std::shared_ptr<void> data = nullptr);

    /// @brief 发布事件（带位置）
    /// @param eventType 事件类型
    /// @param data 事件数据
    /// @param position 事件位置
    void publish(int eventType, std::shared_ptr<void> data, const cocos2d::Vec2& position);

    /// @brief 发布事件（异步）
    /// @param eventType 事件类型
    /// @param data 事件数据
    /// @note 异步调用，不阻塞当前线程
    void publishAsync(int eventType, std::shared_ptr<void> data = nullptr);

    /// @brief 发布事件（延迟执行）
    /// @param eventType 事件类型
    /// @param data 事件数据
    /// @param delay 延迟时间（秒）
    void publishDelayed(int eventType, std::shared_ptr<void> data, float delay);

    // ==================== 便捷方法 ====================

    /// @brief 发布卡牌事件
    /// @param cardName 卡牌名称
    /// @param handIndex 手牌索引
    void publishCardPlayed(const std::string& cardName, int handIndex);

    /// @brief 发布攻击事件
    /// @param attackerName 攻击者名称
    /// @param targetName 目标名称
    /// @param damage 伤害值
    void publishAttack(const std::string& attackerName, const std::string& targetName, int damage);

    /// @brief 发布伤害事件
    /// @param targetName 目标名称
    /// @param damage 伤害值
    /// @param isBlocked 是否被格挡
    void publishDamageTaken(const std::string& targetName, int damage, bool isBlocked);

    /// @brief 发布回合事件
    /// @param isPlayerTurn 是否玩家回合
    /// @param roundNumber 回合数
    void publishTurnEvent(bool isPlayerTurn, int roundNumber);

    // ==================== 调试和监控 ====================

    /// @brief 启用事件追踪（用于调试）
    /// @param enabled 是否启用
    void setEventTracingEnabled(bool enabled);

    /// @brief 获取事件统计信息
    /// @return 统计信息字符串
    std::string getStatistics() const;

    /// @brief 打印当前所有监听器
    void printListeners() const;

protected:
    /// @brief 构造函数（私有单例）
    CombatEventBus();

    /// @brief 析构函数
    ~CombatEventBus();

private:
    // 单例相关
    static CombatEventBus* instance_;
    static std::atomic<bool> instanceCreated_;

    // 监听器管理
    struct ListenerInfo {
        int id;              /// 监听器ID
        EventListener func;  /// 监听器函数
        int priority;        /// 优先级

        ListenerInfo(int i, EventListener f, int p)
            : id(i), func(std::move(f)), priority(p) {}
    };

    using ListenerMap = std::map<int, std::vector<ListenerInfo>>; /// 事件类型 -> 监听器列表

    ListenerMap listeners_;           /// 监听器列表
    mutable std::mutex mutex_;        /// 互斥锁
    int nextListenerId_ = 1;          /// 下一个监听器ID

    // 事件追踪
    bool tracingEnabled_ = false;
    struct EventLog {
        int type;
        std::chrono::steady_clock::time_point time;
        size_t listenerCount;
    };
    mutable std::vector<EventLog> eventLog_;
    mutable std::mutex logMutex_;

    // 内部方法
    void executeListeners_(int eventType, const Event& event);
    void executeListener_(const ListenerInfo& listener, const Event& event);

    // 禁止拷贝和赋值
    CombatEventBus(const CombatEventBus&) = delete;
    CombatEventBus& operator=(const CombatEventBus&) = delete;
};

/**
 * @brief 事件数据基类
 * @note 客户端可以继承此类来传递自定义事件数据
 */
class BaseEventData : public std::enable_shared_from_this<BaseEventData>
{
public:
    virtual ~BaseEventData() = default;
    virtual std::string toString() const = 0;
};

/**
 * @brief 卡牌事件数据
 */
struct CardEventData : public BaseEventData {
    std::string cardName;
    int handIndex;
    bool hasTarget;
    int targetIndex;

    CardEventData(const std::string& name, int index, bool target = false, int tIndex = -1)
        : cardName(name), handIndex(index), hasTarget(target), targetIndex(tIndex) {}

    std::string toString() const override {
        return "CardEvent: " + cardName + " (index: " + std::to_string(handIndex) + ")";
    }
};

/**
 * @brief 攻击事件数据
 */
struct AttackEventData : public BaseEventData {
    std::string attackerName;
    std::string targetName;
    int baseDamage;
    int actualDamage;
    bool isBlocked;
    bool isCritical;

    AttackEventData(const std::string& attacker, const std::string& target,
                    int base, int actual, bool blocked, bool crit)
        : attackerName(attacker), targetName(target), baseDamage(base)
        , actualDamage(actual), isBlocked(blocked), isCritical(crit) {}

    std::string toString() const override {
        return "AttackEvent: " + attackerName + " -> " + targetName +
               " (" + std::to_string(actualDamage) + " dmg" +
               (isBlocked ? ", blocked" : "") +
               (isCritical ? ", crit" : "") + ")";
    }
};

/**
 * @brief 伤害事件数据
 */
struct DamageEventData : public BaseEventData {
    std::string targetName;
    int damage;
    bool isBlocked;
    int finalDamage;

    DamageEventData(const std::string& target, int dmg, bool blocked, int final)
        : targetName(target), damage(dmg), isBlocked(blocked), finalDamage(final) {}

    std::string toString() const override {
        return "DamageEvent: " + targetName + " takes " + std::to_string(finalDamage) +
               " damage" + (isBlocked ? " (blocked " + std::to_string(damage - finalDamage) + ")" : "");
    }
};

/**
 * @brief 回合事件数据
 */
struct TurnEventData : public BaseEventData {
    bool isPlayerTurn;
    int roundNumber;
    int turnCount;

    TurnEventData(bool playerTurn, int round, int turn)
        : isPlayerTurn(playerTurn), roundNumber(round), turnCount(turn) {}

    std::string toString() const override {
        return "TurnEvent: " + std::string(isPlayerTurn ? "Player" : "Monster") +
               " Turn #" + std::to_string(turnCount) + " (Round " + std::to_string(roundNumber) + ")";
    }
};

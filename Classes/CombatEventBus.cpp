#include "CombatEventBus.h"
#include <algorithm>
#include <sstream>

// 单例初始化
std::atomic<bool> CombatEventBus::instanceCreated_{false};
CombatEventBus* CombatEventBus::instance_ = nullptr;

// 全局互斥锁保护单例创建
std::mutex& getEventBusInstanceMutex() {
    static std::mutex instanceMutex;
    return instanceMutex;
}

CombatEventBus* CombatEventBus::getInstance() {
    if (!instanceCreated_.load()) {
        // 双重检查锁模式
        std::lock_guard<std::mutex> lock(getEventBusInstanceMutex());
        if (!instance_) {
            instance_ = new CombatEventBus();
            instanceCreated_.store(true);
        }
    }
    return instance_;
}

void CombatEventBus::destroyInstance() {
    std::lock_guard<std::mutex> lock(getEventBusInstanceMutex());
    if (instance_) {
        delete instance_;
        instance_ = nullptr;
        instanceCreated_.store(false);
    }
}

CombatEventBus::CombatEventBus()
    : nextListenerId_(1) {
}

CombatEventBus::~CombatEventBus() {
    clearAllListeners();
}

int CombatEventBus::addListener(int eventType, EventListener listener, int priority) {
    std::lock_guard<std::mutex> lock(mutex_);

    int listenerId = nextListenerId_++;
    ListenerInfo info(listenerId, std::move(listener), priority);

    // 按优先级插入（优先级高的在前面）
    auto& listeners = listeners_[eventType];
    auto insertPos = std::lower_bound(
        listeners.begin(), listeners.end(), priority,
        [](const ListenerInfo& a, int b) { return a.priority > b; });

    listeners.insert(insertPos, info);

    CCLOG("[EventBus] Listener added: id=%d, type=%d, priority=%d", listenerId, eventType, priority);
    return listenerId;
}

int CombatEventBus::addOnceListener(int eventType, EventListener listener) {
    int listenerId = addListener(eventType, [listenerId, this, eventType](const Event& e) {
        // 执行一次后自动移除
        listener(e);
        removeListener(listenerId);
    });
    return listenerId;
}

bool CombatEventBus::removeListener(int listenerId) {
    std::lock_guard<std::mutex> lock(mutex_);

    for (auto& pair : listeners_) {
        auto& vec = pair.second;
        auto it = std::find_if(vec.begin(), vec.end(),
            [listenerId](const ListenerInfo& info) { return info.id == listenerId; });

        if (it != vec.end()) {
            vec.erase(it);
            CCLOG("[EventBus] Listener removed: id=%d", listenerId);
            return true;
        }
    }
    return false;
}

int CombatEventBus::removeListeners(int eventType) {
    std::lock_guard<std::mutex> lock(mutex_);

    auto it = listeners_.find(eventType);
    if (it != listeners_.end()) {
        int count = it->second.size();
        listeners_.erase(it);
        CCLOG("[EventBus] Removed %d listeners for event type %d", count, eventType);
        return count;
    }
    return 0;
}

void CombatEventBus::clearAllListeners() {
    std::lock_guard<std::mutex> lock(mutex_);
    listeners_.clear();
    CCLOG("[EventBus] All listeners cleared");
}

int CombatEventBus::getListenerCount(int eventType) const {
    std::lock_guard<std::mutex> lock(mutex_);

    if (eventType == -1) {
        int total = 0;
        for (const auto& pair : listeners_) {
            total += pair.second.size();
        }
        return total;
    }

    auto it = listeners_.find(eventType);
    return (it != listeners_.end()) ? it->second.size() : 0;
}

void CombatEventBus::publish(int eventType, std::shared_ptr<void> data) {
    Event event(eventType, std::move(data));
    executeListeners_(eventType, event);
}

void CombatEventBus::publish(int eventType, std::shared_ptr<void> data, const cocos2d::Vec2& position) {
    Event event(eventType, std::move(data));
    event.position = position;
    executeListeners_(eventType, event);
}

void CombatEventBus::publishAsync(int eventType, std::shared_ptr<void> data) {
    // 使用Cocos2d调度器异步执行
    auto dispatcher = cocos2d::Director::getInstance()->getScheduler();
    dispatcher->performFunctionInCocosThread([this, eventType, data]() {
        publish(eventType, data);
    });
}

void CombatEventBus::publishDelayed(int eventType, std::shared_ptr<void> data, float delay) {
    // 使用Cocos2d调度器延迟执行
    auto dispatcher = cocos2d::Director::getInstance()->getScheduler();

    // 创建定时器任务
    struct TimerData {
        int eventType;
        std::shared_ptr<void> data;
    };

    auto timerData = std::make_shared<TimerData>(TimerData{eventType, data});

    dispatcher->schedule([this, timerData](float) {
        publish(timerData->eventType, timerData->data);
    }, this, delay, 0, 0.0f, false, "EventBusDelay");
}

void CombatEventBus::publishCardPlayed(const std::string& cardName, int handIndex) {
    auto data = std::make_shared<CardEventData>(cardName, handIndex);
    publish(CombatEventType::CARD_PLAYED, data);
}

void CombatEventBus::publishAttack(const std::string& attackerName, const std::string& targetName, int damage) {
    auto data = std::make_shared<AttackEventData>(attackerName, targetName, damage, damage, false, false);
    publish(CombatEventType::ATTACK, data);
}

void CombatEventBus::publishDamageTaken(const std::string& targetName, int damage, bool isBlocked) {
    int finalDamage = isBlocked ? 0 : damage;
    auto data = std::make_shared<DamageEventData>(targetName, damage, isBlocked, finalDamage);
    publish(CombatEventType::DAMAGE_TAKEN, data);
}

void CombatEventBus::publishTurnEvent(bool isPlayerTurn, int roundNumber) {
    // 注意：这里需要传入turnCount，简化处理用roundNumber代替
    auto data = std::make_shared<TurnEventData>(isPlayerTurn, roundNumber, roundNumber);
    int eventType = isPlayerTurn ? CombatEventType::TURN_START : CombatEventType::TURN_END;
    publish(eventType, data);
}

void CombatEventBus::setEventTracingEnabled(bool enabled) {
    tracingEnabled_ = enabled;
    if (enabled) {
        CCLOG("[EventBus] Event tracing enabled");
    } else {
        CCLOG("[EventBus] Event tracing disabled");
    }
}

std::string CombatEventBus::getStatistics() const {
    std::lock_guard<std::mutex> lock(mutex_);

    std::ostringstream oss;
    oss << "=== EventBus Statistics ===\n";
    oss << "Total listener count: " << getListenerCount() << "\n";

    int totalEvents = 0;
    {
        std::lock_guard<std::mutex> logLock(logMutex_);
        totalEvents = eventLog_.size();
        oss << "Total events published: " << totalEvents << "\n";

        if (!eventLog_.empty()) {
            oss << "\nLast 5 events:\n";
            auto it = eventLog_.end();
            std::advance(it, -5);
            for (; it != eventLog_.end(); ++it) {
                auto now = std::chrono::steady_clock::now();
                auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - it->time).count();

                oss << "  [+" << duration << "ms] Event " << it->type
                    << " -> " << it->listenerCount << " listeners\n";
            }
        }
    }

    oss << "\nListeners per event type:\n";
    for (const auto& pair : listeners_) {
        oss << "  Event " << pair.first << ": " << pair.second.size() << " listeners\n";
    }

    return oss.str();
}

void CombatEventBus::printListeners() const {
    std::lock_guard<std::mutex> lock(mutex_);

    CCLOG("=== EventBus Listeners ===");
    for (const auto& pair : listeners_) {
        CCLOG("Event %d (%zu listeners):", pair.first, pair.second.size());
        for (const auto& listener : pair.second) {
            CCLOG("  - ID: %d, Priority: %d", listener.id, listener.priority);
        }
    }
    CCLOG("============================");
}

void CombatEventBus::executeListeners_(int eventType, const Event& event) {
    // 记录事件日志
    if (tracingEnabled_) {
        std::lock_guard<std::mutex> logLock(logMutex_);
        eventLog_.push_back({eventType, event.timestamp, 0});
        // 限制日志大小
        if (eventLog_.size() > 100) {
            eventLog_.erase(eventLog_.begin());
        }
    }

    std::vector<ListenerInfo> listenersToExecute;
    {
        std::lock_guard<std::mutex> lock(mutex_);
        auto it = listeners_.find(eventType);
        if (it == listeners_.end()) {
            return;
        }
        listenersToExecute = it->second; // 复制一份以避免在执行时持有锁
    }

    // 更新日志中的监听器数量
    if (tracingEnabled_) {
        std::lock_guard<std::mutex> logLock(logMutex_);
        if (!eventLog_.empty()) {
            eventLog_.back().listenerCount = listenersToExecute.size();
        }
    }

    // 执行监听器
    for (const auto& listener : listenersToExecute) {
        try {
            executeListener_(listener, event);
        } catch (const std::exception& e) {
            CCLOG("[EventBus] Exception in listener %d: %s", listener.id, e.what());
        } catch (...) {
            CCLOG("[EventBus] Unknown exception in listener %d", listener.id);
        }
    }
}

void CombatEventBus::executeListener_(const ListenerInfo& listener, const Event& event) {
    // 检查监听器是否仍然存在（可能在执行过程中被移除）
    std::lock_guard<std::mutex> lock(mutex_);
    bool exists = false;
    for (const auto& pair : listeners_) {
        for (const auto& l : pair.second) {
            if (l.id == listener.id) {
                exists = true;
                break;
            }
        }
        if (exists) break;
    }

    if (exists) {
        // 在锁外部执行以避免死锁
        lock.unlock();
        listener.func(event);
    }
}

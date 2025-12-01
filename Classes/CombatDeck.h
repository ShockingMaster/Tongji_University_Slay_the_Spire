#pragma once

#include "cocos2d.h"
#include <functional>
#include <memory>
#include <queue>
#include <vector>

// Forward declarations
class Card;
class Player;
class IRandomGenerator;

/**
 * @brief CombatDeck - Card Management System
 *
 * Responsible for managing all card-related operations in combat:
 * 1. Storage and management of draw pile, discard pile, and hand
 * 2. Operations like shuffling, drawing, discarding, exhausting cards
 * 3. Card movement and state management
 *
 * Design Principles:
 * - Single Responsibility: Only responsible for card management, not combat logic
 * - Use Smart Pointers: Avoid memory leaks
 * - Support Event Notification: Notify observers when cards change
 */
class CombatDeck {
public:
    /// @brief Create CombatDeck instance
    /// @return Unique pointer managing the CombatDeck instance
    static std::unique_ptr<CombatDeck> create(std::shared_ptr<IRandomGenerator> randomGen = nullptr);

    /// @brief Constructor
    CombatDeck(std::shared_ptr<IRandomGenerator> randomGen = nullptr);

    /// @brief Destructor
    ~CombatDeck();

    /// @brief Initialize deck
    /// @param player Player instance (used to initialize starting deck)
    /// @return true if initialization is successful
    bool init(std::shared_ptr<Player> player);

    // ==================== Draw Pile Management ====================

    /// @brief Draw cards
    /// @param count Number of cards to draw
    /// @return Actual number of cards drawn
    int drawCards(int count);

    /// @brief Add card to draw pile
    /// @param card Card
    /// @param count Quantity
    void addToDrawPile(std::shared_ptr<Card> card, int count = 1);

    /// @brief Get draw pile size
    int getDrawPileSize() const { return drawPile_.size(); }

    /// @brief Check if draw pile is empty
    bool isDrawPileEmpty() const { return drawPile_.empty(); }

    /// @brief Shuffle (shuffle discard pile into draw pile)
    void shuffle();

    // ==================== Hand Management ====================

    /// @brief Get hand cards
    /// @return Hand card vector (read-only)
    std::vector<std::shared_ptr<Card>> getHand() const { return std::vector<std::shared_ptr<Card>>(hand_); }

    /// @brief Get hand size
    int getHandSize() const { return hand_.size(); }

    /// @brief Add card to hand
    /// @param card Card
    /// @return Whether addition was successful
    bool addToHand(std::shared_ptr<Card> card);

    /// @brief Remove card from hand
    /// @param card Card
    /// @return Whether removal was successful
    bool removeFromHand(std::shared_ptr<Card> card);

    /// @brief Remove card from hand by index
    /// @param index Index
    /// @return Removed card (returns nullptr if index is invalid)
    std::shared_ptr<Card> removeFromHand(int index);

    /// @brief Clear all hand cards
    void clearHand();

    /// @brief Adjust hand layout (for UI)
    void adjustHandLayout();

    // ==================== Discard Pile Management ====================

    /// @brief Add card to discard pile
    /// @param card Card
    /// @param count Quantity
    void addToDiscardPile(std::shared_ptr<Card> card, int count = 1);

    /// @brief 获取弃牌堆大小
    int getDiscardPileSize() const { return discardPile_.size(); }

    /// @brief 检查弃牌堆是否为空
    bool isDiscardPileEmpty() const { return discardPile_.empty(); }

    // ==================== 卡牌操作 ====================

    /// @brief 弃牌（将卡牌从手牌移到弃牌堆）
    /// @param card 卡牌
    /// @return 是否成功
    bool discardCard(std::shared_ptr<Card> card);

    /// @brief 弃牌（按索引）
    /// @param index 手牌索引
    /// @return 是否成功
    bool discardCard(int index);

    /// @brief 耗尽卡牌（从游戏中移除）
    /// @param card 卡牌
    /// @return 是否成功
    bool exhaustCard(std::shared_ptr<Card> card);

    /// @brief 耗尽卡牌（按索引）
    /// @param index 手牌索引
    /// @return 是否成功
    bool exhaustCard(int index);

    /// @brief 升级卡牌
    /// @param card 卡牌
    /// @return 是否成功
    bool upgradeCard(std::shared_ptr<Card> card);

    /// @brief 升级卡牌（按索引）
    /// @param index 手牌索引
    /// @return 是否成功
    bool upgradeCard(int index);

    // ==================== 查询方法 ====================

    /// @brief 检查手牌中是否有指定卡牌
    /// @param card 卡牌
    /// @return 是否存在
    bool hasCardInHand(std::shared_ptr<Card> card) const;

    /// @brief 获取手牌中卡牌的索引
    /// @param card 卡牌
    /// @return 索引（-1 if not found）
    int getCardIndexInHand(std::shared_ptr<Card> card) const;

    /// @brief 计算总卡牌数量（所有堆）
    /// @return 总数量
    int getTotalCardCount() const;

    // ==================== 事件通知 ====================

    /// @brief 卡牌变化事件回调
    using CardChangedCallback = std::function<void(const std::string& event, std::shared_ptr<Card> card)>;

    /// @brief 设置卡牌变化回调
    /// @param callback 回调函数
    void setCardChangedCallback(CardChangedCallback callback)
    {
        cardChangedCallback_ = callback;
    }

protected:
    /// @brief 通知卡牌变化
    /// @param event 事件类型
    /// @param card 涉及卡牌
    void notifyCardChanged(const std::string& event, std::shared_ptr<Card> card);

private:
    // 卡牌存储（使用智能指针）
    std::queue<std::shared_ptr<Card>> drawPile_; /// 抽牌堆
    std::vector<std::shared_ptr<Card>> hand_; /// 手牌（使用vector以便随机访问）
    std::queue<std::shared_ptr<Card>> discardPile_; /// 弃牌堆

    // 依赖
    std::shared_ptr<IRandomGenerator> randomGen_; /// 随机数生成器

    // 事件回调
    CardChangedCallback cardChangedCallback_;

    // 内部辅助方法
    bool moveCardToDiscard(std::shared_ptr<Card> card);
    bool removeCardFromVector(std::vector<std::shared_ptr<Card>>& vec, std::shared_ptr<Card> card);

    // 禁止拷贝和赋值
    CombatDeck(const CombatDeck&) = delete;
    CombatDeck& operator=(const CombatDeck&) = delete;
};

/**
 * @brief 卡牌事件类型常量
 */
namespace CardEvent {
const std::string DREW = "DREW";
const std::string ADDED_TO_HAND = "ADDED_TO_HAND";
const std::string REMOVED_FROM_HAND = "REMOVED_FROM_HAND";
const std::string DISCARDED = "DISCARDED";
const std::string EXHAUSTED = "EXHAUSTED";
const std::string UPGRADED = "UPGRADED";
const std::string SHUFFLED = "SHUFFLED";
}

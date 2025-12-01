#pragma once

#include "cocos2d.h"
#include <functional>
#include <memory>

// Forward declarations
class Player;
class Monster;
class Card;
class Creature;

/**
 * @brief CombatUIController - Combat UI Controller
 *
 * UI update logic separated from CombatSystem, responsible for:
 * 1. Updating player and monster displays (HP, armor, buffs, etc.)
 * 2. Updating hand card display (draw, discard, hand count)
 * 3. Updating energy display
 * 4. Handling UI animation effects
 *
 * This class decouples combat logic from UI rendering, allowing:
 * - Logic layer to be tested independently of UI layer
 * - Easy switching of different UI implementations (console, GUI, etc.)
 * - Improved code maintainability
 */
class CombatUIController {
public:
    /// @brief Create CombatUIController instance
    /// @return Unique pointer managing the CombatUIController instance
    static std::unique_ptr<CombatUIController> create();

    /// @brief Destructor
    virtual ~CombatUIController();

    /// @brief Initialize UI Controller
    /// @param scene Current running scene (Cocos2d Scene)
    /// @return true if initialization is successful
    bool init(cocos2d::Scene* scene);

    // ==================== Player UI Updates ====================

    /// @brief Update player display (HP, armor, energy, etc.)
    /// @param player Player instance
    /// @note Triggers CreatureLayer::updateDisplay()
    void updatePlayerDisplay(std::shared_ptr<Player> player);

    /// @brief Update player energy display
    /// @param currentEnergy Current energy
    /// @param maxEnergy Max energy
    /// @note Calls CombatScene::updateEnergyDisplay()
    void updateEnergyDisplay(int currentEnergy, int maxEnergy);

    /// @brief Update player buff display
    /// @param player Player instance
    /// @note Displays buff icons around player avatar
    void updatePlayerBuffs(std::shared_ptr<Player> player);

    // ==================== Monster UI Updates ====================

    /// @brief Update all monsters display
    /// @param monsters List of monsters
    /// @note Triggers CreatureLayer::updateDisplay(), updates all monster states
    void updateMonstersDisplay(const std::vector<std::shared_ptr<Monster>>& monsters);

    /// @brief Update specific monster display
    /// @param monster Monster instance
    /// @note Updates display for a single monster (performance optimization)
    void updateMonsterDisplay(std::shared_ptr<Monster> monster);

    /// @brief Show monster attack intention
    /// @param monster Monster instance
    /// @note Displays intention icon (attack, defend, etc.) above monster head
    void showMonsterIntention(std::shared_ptr<Monster> monster);

    // ==================== Hand Card UI Updates ====================

    /// @brief Add card to hand display
    /// @param card Card instance
    /// @note Calls HandPileLayer::drawCard()
    void addCardToHand(std::shared_ptr<Card> card);

    /// @brief Remove card from hand display
    /// @param card Card instance
    /// @note Calls HandPileLayer::removeCard()
    void removeCardFromHand(std::shared_ptr<Card> card);

    /// @brief Clear all hand cards
    /// @note Calls HandPileLayer::clearAll()
    void clearAllHandCards();

    /// @brief Adjust hand layout
    /// @note Calls HandPileLayer::adjustHandPile()
    void adjustHandLayout();

    /// @brief Update draw pile display
    /// @param count Remaining card count
    void updateDrawPileDisplay(int count);

    /// @brief Update discard pile display
    /// @param count 剩余卡牌数量
    void updateDiscardPileDisplay(int count);

    /// @brief 更新卡牌显示（例如升级后）
    /// @param card 卡牌实例
    void updateCardDisplay(std::shared_ptr<Card> card);

    /// @brief 获取当前选中的卡牌列表
    /// @return 选中的卡牌列表
    std::vector<std::shared_ptr<Card>> getSelectedCards();

    /// @brief 更新生物显示（自动判断是玩家还是怪物）
    /// @param creature 生物实例
    void updateCreatureDisplay(std::shared_ptr<Creature> creature);

    /// @brief 更新所有生物显示（直接调用 CreatureLayer::updateDisplay）
    /// @note 用于替代直接调用 scene->creatureLayer->updateDisplay()
    void updateAllCreaturesDisplay();

    /// @brief 丢弃所有手牌到弃牌堆（回合结束时使用）
    /// @note 清除手牌显示并更新弃牌堆
    void discardAllHandCardsDisplay();

    /// @brief 通过 Sprite Tag 移除卡牌
    /// @param cardPtr 卡牌指针（用于计算 tag）
    void removeCardSpriteByTag(void* cardPtr);

    // ==================== 动画效果 ====================

    /// @brief 播放攻击动画
    /// @param attacker 攻击者
    /// @param target 目标
    /// @param damage 伤害值
    /// @note 在攻击者和目标之间播放攻击动画
    void playAttackAnimation(std::shared_ptr<Creature> attacker, std::shared_ptr<Creature> target, int damage);

    /// @brief 播放受伤动画
    /// @param target 受伤者
    /// @param damage 伤害值
    /// @note 目标闪烁并显示伤害数字
    void playDamageAnimation(std::shared_ptr<Creature> target, int damage);

    /// @brief 播放防御动画
    /// @param target 防御者
    /// @param block 护甲值
    /// @note 显示护甲获得效果
    void playBlockAnimation(std::shared_ptr<Creature> target, int block);

    /// @brief 播放治疗动画
    /// @param target 治疗目标
    /// @param heal 治疗量
    void playHealAnimation(std::shared_ptr<Creature> target, int heal);

    /// @brief 播放buff添加动画
    /// @param target buff目标
    /// @param buffName buff名称
    void playBuffAnimation(std::shared_ptr<Creature> target, const std::string& buffName);

    // ==================== 战斗反馈 ====================

    /// @brief 显示战斗日志消息
    /// @param message 消息内容
    /// @param type 消息类型（INFO, WARNING, ERROR）
    /// @note 在战斗界面底部显示滚动消息
    void showBattleLog(const std::string& message, const std::string& type = "INFO");

    /// @brief 显示临时消息
    /// @param message 消息内容
    /// @param duration 显示时长（秒）
    /// @note 在屏幕中央显示短暂消息（如"伤害抵抗"）
    void showTempMessage(const std::string& message, float duration = 1.5f);

    /// @brief 显示胜利界面
    /// @note 战斗胜利后调用，显示奖励选择等
    void showVictoryScreen();

    /// @brief 显示失败界面
    /// @note 战斗失败后调用，显示结算信息
    void showDefeatScreen();

    // ==================== 状态查询 ====================

    /// @brief 检查UI是否就绪
    /// @return true if 所有UI组件已初始化
    bool isUIReady() const { return uiReady_; }

    /// @brief 获取当前场景
    /// @return Cocos2d场景指针
    cocos2d::Scene* getCurrentScene() const { return currentScene_; }

protected:
    /// @brief 构造函数（保护，使用create创建）
    CombatUIController();

private:
    // Cocos2d场景引用
    cocos2d::Scene* currentScene_ = nullptr;

    // UI层组件
    class HandPileLayer* handLayer_ = nullptr; /// 手牌层
    class CreatureLayer* creatureLayer_ = nullptr; /// 生物层

    // 状态标记
    bool uiReady_ = false;

    // 禁止拷贝和赋值
    CombatUIController(const CombatUIController&) = delete;
    CombatUIController& operator=(const CombatUIController&) = delete;

    // 内部辅助方法
    /// @brief 获取子节点
    template <typename T>
    T getChildByName(const std::string& name);

    /// @brief 安全更新UI（检查UI是否就绪）
    void safeUpdateUI(std::function<void()> updateFunc);
};

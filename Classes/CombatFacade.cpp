#include "CombatFacade.h"
#include "Card.h"
#include "CombatDeck.h"
#include "CombatEventBus.h"
#include "CombatSystem.h"
#include "CombatUIController.h"
#include "Creature.h"
#include "Monster.h"
#include "Player.h"
#include "cocos2d.h"

USING_NS_CC;

std::unique_ptr<CombatFacade> CombatFacade::create()
{
    std::unique_ptr<CombatFacade> facade(new CombatFacade());
    if (facade->init()) {
        return facade;
    }
    return nullptr;
}

CombatFacade::CombatFacade()
    : isInitialized_(false)
    , isCombatActive_(false)
    , isPlayerTurn_(false)
    , currentRound_(0)
{
    CCLOG("[Facade] CombatFacade created");
}

CombatFacade::~CombatFacade()
{
    CCLOG("[Facade] CombatFacade destroyed");
    // 清理事件监听器
    if (eventBus_) {
        eventBus_->clearAllListeners();
    }
}

bool CombatFacade::init()
{
    try {
        // 1. Get Event Bus (Singleton - not owned by Facade)
        eventBus_ = CombatEventBus::getInstance();
        if (!eventBus_) {
            CCLOG("[Facade] Error: Failed to get CombatEventBus instance");
            return false;
        }

        // 2. Get CombatSystem (Singleton)
        // Fix: Use the existing singleton instance instead of creating a new one
        // This ensures Facade operates on the same system as the rest of the game
        combatSystem_ = CombatSystem::getInstance();
        if (!combatSystem_) {
            CCLOG("[Facade] Error: Failed to get CombatSystem instance");
            return false;
        }

        // 3. Get current scene and create UI Controller
        auto scene = dynamic_cast<Scene*>(Director::getInstance()->getRunningScene());
        if (!scene) {
            CCLOG("[Facade] Error: No running scene found");
            return false;
        }

        uiController_ = CombatUIController::create();
        if (!uiController_) {
            CCLOG("[Facade] Error: Failed to create UIController");
            return false;
        }

        if (!uiController_->init(scene)) {
            CCLOG("[Facade] Error: Failed to initialize UIController");
            return false;
        }

        // Inject UI Controller into CombatSystem
        combatSystem_->setUIController(uiController_);

        isInitialized_ = true;
        CCLOG("[Facade] Initialized successfully");

        // Publish initialization complete event
        eventBus_->publish(CombatEventType::COMBAT_START, nullptr);

        return true;
    } catch (const std::exception& e) {
        CCLOG("[Facade] Exception during initialization: %s", e.what());
        return false;
    }
}

// ==================== 战斗流程控制 ====================

void CombatFacade::startCombat(int combatType)
{
    if (!isInitialized_) {
        CCLOG("[Facade] Error: Facade not initialized");
        return;
    }

    CCLOG("[Facade] Starting combat (type=%d)", combatType);

    try {
        // 初始化战斗系统
        combatSystem_->init(combatType);

        // 更新UI
        if (uiController_) {
            uiController_->updatePlayerDisplay(getPlayer());
            uiController_->updateMonstersDisplay(getMonsters());
            uiController_->updateDrawPileDisplay(combatSystem_->getDrawPileNumber());
            uiController_->updateDiscardPileDisplay(combatSystem_->getDiscardPileNumber());
        }

        isCombatActive_ = true;
        isPlayerTurn_ = true;
        currentRound_ = 1;

        // 发布战斗开始事件
        eventBus_->publish(CombatEventType::COMBAT_START, nullptr);

        CCLOG("[Facade] Combat started successfully");

    } catch (const std::exception& e) {
        CCLOG("[Facade] Exception in startCombat: %s", e.what());
    }
}

void CombatFacade::endCombat()
{
    if (!isCombatActive_) {
        CCLOG("[Facade] Warning: Combat not active");
        return;
    }

    CCLOG("[Facade] Ending combat");

    isCombatActive_ = false;
    isPlayerTurn_ = false;

    // 清理UI
    if (uiController_) {
        uiController_->clearAllHandCards();
        uiController_->showBattleLog("战斗结束", "INFO");
    }

    // 发布战斗结束事件
    eventBus_->publish(CombatEventType::COMBAT_END, nullptr);

    CCLOG("[Facade] Combat ended");
}

void CombatFacade::startPlayerTurn()
{
    if (!isCombatActive_) {
        CCLOG("[Facade] Warning: Combat not active");
        return;
    }

    CCLOG("[Facade] Starting player turn (round %d)", currentRound_);

    try {
        // 开始玩家回合
        combatSystem_->startTurn(getPlayer());

        // 更新UI
        if (uiController_) {
            auto player = getPlayer();
            uiController_->updatePlayerDisplay(player);
            uiController_->updateEnergyDisplay(player->getCurrentEnergy(), player->getMaxEnergy());
        }

        isPlayerTurn_ = true;

        // 发布回合开始事件
        eventBus_->publishTurnEvent(true, currentRound_);

        CCLOG("[Facade] Player turn started");

    } catch (const std::exception& e) {
        CCLOG("[Facade] Exception in startPlayerTurn: %s", e.what());
    }
}

void CombatFacade::endPlayerTurn()
{
    if (!isCombatActive_ || !isPlayerTurn_) {
        CCLOG("[Facade] Warning: Cannot end player turn (combat=%s, playerTurn=%s)",
            isCombatActive_ ? "active" : "inactive", isPlayerTurn_ ? "yes" : "no");
        return;
    }

    CCLOG("[Facade] Ending player turn");

    try {
        // 结束玩家回合
        combatSystem_->endTurnCardPlayed();
        combatSystem_->endTurn(getPlayer());

        isPlayerTurn_ = false;

        // 发布回合结束事件
        eventBus_->publishTurnEvent(false, currentRound_);

        // 开始怪物回合
        startMonsterTurn();

    } catch (const std::exception& e) {
        CCLOG("[Facade] Exception in endPlayerTurn: %s", e.what());
    }
}

void CombatFacade::startMonsterTurn()
{
    if (!isCombatActive_) {
        CCLOG("[Facade] Warning: Combat not active");
        return;
    }

    CCLOG("[Facade] Starting monster turn");

    try {
        auto monsters = getMonsters();

        for (const auto& monster : monsters) {
            if (monster->getHealth() > 0) {
                // 开始怪物回合
                combatSystem_->startTurn(monster);

                // 触发怪物AI
                auto monsterPtr = std::static_pointer_cast<Monster>(monster);
                if (monsterPtr) {
                    monsterPtr->takeEffect();
                }

                // 结束怪物回合
                combatSystem_->endTurn(monster);
            }
        }

        // 更新UI
        if (uiController_) {
            uiController_->updateMonstersDisplay(monsters);
        }

        // 回合数增加
        currentRound_++;

        // 开始下一轮玩家回合
        startPlayerTurn();

    } catch (const std::exception& e) {
        CCLOG("[Facade] Exception in startMonsterTurn: %s", e.what());
    }
}

bool CombatFacade::isCombatEnded() const
{
    if (!isCombatActive_) {
        return true;
    }

    auto player = getPlayer();
    if (player && player->getHealth() <= 0) {
        return true; // 玩家死亡
    }

    auto monsters = getMonsters();
    for (const auto& monster : monsters) {
        if (monster->getHealth() > 0) {
            return false; // 还有怪物存活
        }
    }

    return true; // 所有怪物死亡
}

// ==================== 卡牌操作 ====================

bool CombatFacade::playCard(int handIndex)
{
    if (!isCombatActive_ || !isPlayerTurn_) {
        CCLOG("[Facade] Warning: Cannot play card (combat=%s, playerTurn=%s)",
            isCombatActive_ ? "active" : "inactive", isPlayerTurn_ ? "yes" : "no");
        return false;
    }

    auto hand = getPlayerHand();
    if (handIndex < 0 || handIndex >= static_cast<int>(hand.size())) {
        CCLOG("[Facade] Error: Invalid hand index %d (size=%zu)", handIndex, hand.size());
        return false;
    }

    auto card = hand[handIndex];
    if (!card->getCanBePlayed() || card->getEnergyCost() > getPlayer()->getCurrentEnergy()) {
        CCLOG("[Facade] Warning: Cannot play card %s (cost=%d, energy=%d)",
            card->getName().c_str(), card->getEnergyCost(), getPlayer()->getCurrentEnergy());
        return false;
    }

    CCLOG("[Facade] Playing card: %s", card->getName().c_str());

    try {
        // 执行卡牌效果
        combatSystem_->cardPlayed(card);

        // 更新UI
        if (uiController_) {
            uiController_->removeCardFromHand(card);
            uiController_->updatePlayerDisplay(getPlayer());
            uiController_->updateMonstersDisplay(getMonsters());
        }

        // 发布卡牌事件
        eventBus_->publishCardPlayed(card->getName(), handIndex);

        CCLOG("[Facade] Card played successfully");

        return true;

    } catch (const std::exception& e) {
        CCLOG("[Facade] Exception in playCard: %s", e.what());
        return false;
    }
}

bool CombatFacade::playCardWithTarget(int handIndex, int targetIndex)
{
    if (!isCombatActive_ || !isPlayerTurn_) {
        return false;
    }

    auto hand = getPlayerHand();
    if (handIndex < 0 || handIndex >= static_cast<int>(hand.size())) {
        return false;
    }

    auto monsters = getMonsters();
    if (targetIndex < 0 || targetIndex >= static_cast<int>(monsters.size())) {
        return false;
    }

    auto card = hand[handIndex];
    if (!card->getCanBePlayed() || card->getEnergyCost() > getPlayer()->getCurrentEnergy()) {
        return false;
    }

    CCLOG("[Facade] Playing card with target: %s -> monster %d",
        card->getName().c_str(), targetIndex);

    try {
        auto target = std::static_pointer_cast<Creature>(monsters[targetIndex]);
        combatSystem_->cardPlayed(card, target);

        // 更新UI
        if (uiController_) {
            uiController_->removeCardFromHand(card);
            uiController_->updatePlayerDisplay(getPlayer());
            uiController_->updateMonstersDisplay(monsters);
        }

        // 发布事件
        eventBus_->publishCardPlayed(card->getName(), handIndex);

        return true;

    } catch (const std::exception& e) {
        CCLOG("[Facade] Exception in playCardWithTarget: %s", e.what());
        return false;
    }
}

int CombatFacade::drawCard(int count)
{
    if (!isCombatActive_) {
        CCLOG("[Facade] Warning: Cannot draw card (combat not active)");
        return 0;
    }

    CCLOG("[Facade] Drawing %d cards", count);

    try {
        combatSystem_->drawCard(count);

        // 更新UI
        if (uiController_) {
            uiController_->updateDrawPileDisplay(combatSystem_->getDrawPileNumber());
            uiController_->updateDiscardPileDisplay(combatSystem_->getDiscardPileNumber());
            uiController_->adjustHandLayout();
        }

        CCLOG("[Facade] Cards drawn successfully");

        return count; // 简化处理，假设都能抽到

    } catch (const std::exception& e) {
        CCLOG("[Facade] Exception in drawCard: %s", e.what());
        return 0;
    }
}

void CombatFacade::discardCard(int handIndex)
{
    if (!isCombatActive_) {
        return;
    }

    auto hand = getPlayerHand();
    if (handIndex < 0 || handIndex >= static_cast<int>(hand.size())) {
        return;
    }

    try {
        auto card = hand[handIndex];
        combatSystem_->discardCard(card);

        // 更新UI
        if (uiController_) {
            uiController_->removeCardFromHand(card);
            uiController_->updateDiscardPileDisplay(combatSystem_->getDiscardPileNumber());
        }

        CCLOG("[Facade] Card discarded: %s", card->getName().c_str());

    } catch (const std::exception& e) {
        CCLOG("[Facade] Exception in discardCard: %s", e.what());
    }
}

void CombatFacade::exhaustCard(int handIndex)
{
    if (!isCombatActive_) {
        return;
    }

    auto hand = getPlayerHand();
    if (handIndex < 0 || handIndex >= static_cast<int>(hand.size())) {
        return;
    }

    try {
        auto card = hand[handIndex];
        combatSystem_->exhaustCard(card);

        // 更新UI
        if (uiController_) {
            uiController_->removeCardFromHand(card);
        }

        CCLOG("[Facade] Card exhausted: %s", card->getName().c_str());

    } catch (const std::exception& e) {
        CCLOG("[Facade] Exception in exhaustCard: %s", e.what());
    }
}

// ==================== 战斗交互 ====================

int CombatFacade::playerAttack(int targetIndex, int damage)
{
    if (!isCombatActive_) {
        return 0;
    }

    auto monsters = getMonsters();
    if (targetIndex < 0 || targetIndex >= static_cast<int>(monsters.size())) {
        return 0;
    }

    try {
        auto target = std::static_pointer_cast<Creature>(monsters[targetIndex]);
        int actualDamage = damage;

        combatSystem_->onAttack(getPlayer(), target, actualDamage);

        // 更新UI
        if (uiController_) {
            uiController_->playAttackAnimation(getPlayer(), target, actualDamage);
            uiController_->playDamageAnimation(target, actualDamage);
            uiController_->updateMonstersDisplay(monsters);
        }

        // 发布事件
        eventBus_->publishAttack("Player", target->getName(), actualDamage);
        eventBus_->publishDamageTaken(target->getName(), actualDamage, false);

        CCLOG("[Facade] Player attacks %s for %d damage",
            target->getName().c_str(), actualDamage);

        return actualDamage;

    } catch (const std::exception& e) {
        CCLOG("[Facade] Exception in playerAttack: %s", e.what());
        return 0;
    }
}

void CombatFacade::playerDefend(int blockAmount)
{
    if (!isCombatActive_) {
        return;
    }

    try {
        combatSystem_->Addblock(getPlayer(), blockAmount);

        // 更新UI
        if (uiController_) {
            uiController_->playBlockAnimation(getPlayer(), blockAmount);
            uiController_->updatePlayerDisplay(getPlayer());
        }

        CCLOG("[Facade] Player defends with %d block", blockAmount);

    } catch (const std::exception& e) {
        CCLOG("[Facade] Exception in playerDefend: %s", e.what());
    }
}

int CombatFacade::playerTakeDamage(int damage, Creature* attacker)
{
    if (!isCombatActive_) {
        return 0;
    }

    try {
        auto target = getPlayer();
        int actualDamage = damage;

        // Note: Pass nullptr for attacker since we can't safely convert raw pointer to shared_ptr
        // The attacker info is only used for logging/events, not critical game logic
        combatSystem_->takeDamage(target, actualDamage, nullptr);

        // 更新UI
        if (uiController_) {
            uiController_->playDamageAnimation(target, actualDamage);
            uiController_->updatePlayerDisplay(target);
        }

        // 发布事件
        std::string attackerName = attacker ? attacker->getName() : "Unknown";
        eventBus_->publishDamageTaken(target->getName(), actualDamage, false);

        CCLOG("[Facade] Player takes %d damage from %s", actualDamage, attackerName.c_str());

        return actualDamage;

    } catch (const std::exception& e) {
        CCLOG("[Facade] Exception in playerTakeDamage: %s", e.what());
        return 0;
    }
}

int CombatFacade::monsterTakeDamage(int targetIndex, int damage, Creature* attacker)
{
    if (!isCombatActive_) {
        return 0;
    }

    auto monsters = getMonsters();
    if (targetIndex < 0 || targetIndex >= static_cast<int>(monsters.size())) {
        return 0;
    }

    try {
        auto target = std::static_pointer_cast<Creature>(monsters[targetIndex]);
        int actualDamage = damage;

        // Note: Pass nullptr for attacker since we can't safely convert raw pointer to shared_ptr
        // The attacker info is only used for logging/events, not critical game logic
        combatSystem_->takeDamage(target, actualDamage, nullptr);

        // 更新UI
        if (uiController_) {
            uiController_->playDamageAnimation(target, actualDamage);
            uiController_->updateMonstersDisplay(monsters);
        }

        // 发布事件
        eventBus_->publishDamageTaken(target->getName(), actualDamage, false);

        CCLOG("[Facade] Monster %s takes %d damage", target->getName().c_str(), actualDamage);

        return actualDamage;

    } catch (const std::exception& e) {
        CCLOG("[Facade] Exception in monsterTakeDamage: %s", e.what());
        return 0;
    }
}

void CombatFacade::playerAddBuff(const std::string& buffName, int stacks)
{
    if (!isCombatActive_) {
        return;
    }

    try {
        // 这里需要从BuffRegistry创建buff
        // 简化处理，假设buff已创建
        CCLOG("[Facade] Adding buff to player: %s (stacks=%d)", buffName.c_str(), stacks);

        // TODO: 实现真正的buff添加逻辑
        // 需要访问BuffRegistry::createBuff(buffName)

    } catch (const std::exception& e) {
        CCLOG("[Facade] Exception in playerAddBuff: %s", e.what());
    }
}

void CombatFacade::monsterAddBuff(int targetIndex, const std::string& buffName, int stacks)
{
    if (!isCombatActive_) {
        return;
    }

    auto monsters = getMonsters();
    if (targetIndex < 0 || targetIndex >= static_cast<int>(monsters.size())) {
        return;
    }

    try {
        CCLOG("[Facade] Adding buff to monster %d: %s (stacks=%d)",
            targetIndex, buffName.c_str(), stacks);

        // TODO: 实现真正的buff添加逻辑

    } catch (const std::exception& e) {
        CCLOG("[Facade] Exception in monsterAddBuff: %s", e.what());
    }
}

void CombatFacade::playerHeal(int healAmount)
{
    if (!isCombatActive_) {
        return;
    }

    try {
        combatSystem_->addHealth(getPlayer(), healAmount);

        // 更新UI
        if (uiController_) {
            uiController_->playHealAnimation(getPlayer(), healAmount);
            uiController_->updatePlayerDisplay(getPlayer());
        }

        CCLOG("[Facade] Player heals %d HP", healAmount);

    } catch (const std::exception& e) {
        CCLOG("[Facade] Exception in playerHeal: %s", e.what());
    }
}

// ==================== 查询接口 ====================

std::shared_ptr<Player> CombatFacade::getPlayer() const
{
    // Use Player singleton directly - it already returns shared_ptr
    return Player::getInstance();
}

std::vector<std::shared_ptr<Monster>> CombatFacade::getMonsters() const
{
    std::vector<std::shared_ptr<Monster>> result;
    if (!combatSystem_) {
        return result;
    }

    // Monsters_ is public in CombatSystem, so this is acceptable
    // But we should cast from Creature to Monster
    for (const auto& creature : combatSystem_->Monsters_) {
        auto monster = std::dynamic_pointer_cast<Monster>(creature);
        if (monster) {
            result.push_back(monster);
        }
    }
    return result;
}

std::vector<std::shared_ptr<Creature>>& CombatFacade::getMonstersAsCreatures()
{
    // Return direct reference to CombatSystem's Monsters_ vector
    // This allows CreatureLayer to work with the original vector
    return combatSystem_->Monsters_;
}

std::vector<std::shared_ptr<Card>> CombatFacade::getPlayerHand() const
{
    if (!combatSystem_ || !combatSystem_->getDeckManager()) {
        return std::vector<std::shared_ptr<Card>>();
    }

    // Use CombatDeck's public getHand() method instead of accessing internal member
    return combatSystem_->getDeckManager()->getHand();
}

int CombatFacade::getDrawPileSize() const
{
    return combatSystem_ ? combatSystem_->getDrawPileNumber() : 0;
}

int CombatFacade::getDiscardPileSize() const
{
    return combatSystem_ ? combatSystem_->getDiscardPileNumber() : 0;
}

int CombatFacade::getHandSize() const
{
    return combatSystem_ ? combatSystem_->getHandNumber() : 0;
}

int CombatFacade::getCurrentRound() const
{
    return currentRound_;
}

bool CombatFacade::isPlayerTurn() const
{
    return isPlayerTurn_;
}

bool CombatFacade::canPlayCard(int handIndex) const
{
    auto hand = getPlayerHand();
    if (handIndex < 0 || handIndex >= static_cast<int>(hand.size())) {
        return false;
    }

    auto card = hand[handIndex];
    auto player = getPlayer();
    return card && card->getCanBePlayed() && card->getEnergyCost() <= player->getCurrentEnergy();
}

int CombatFacade::getCardEnergyCost(int handIndex) const
{
    auto hand = getPlayerHand();
    if (handIndex < 0 || handIndex >= static_cast<int>(hand.size())) {
        return -1;
    }
    return hand[handIndex]->getEnergyCost();
}

// ==================== 事件监听 ====================

template <typename Func>
void CombatFacade::addEventListener(int eventType, Func&& callback)
{
    if (eventBus_) {
        eventBus_->addListener(eventType, std::forward<Func>(callback));
    }
}

void CombatFacade::removeEventListener(int eventType)
{
    if (eventBus_) {
        // 注意：这里需要传入监听器ID，简化处理清空所有
        eventBus_->removeListeners(eventType);
    }
}

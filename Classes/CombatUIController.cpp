#include "CombatUIController.h"
#include "Card.h"
#include "CardSpriteGenerator.h"
#include "CombatScene.h"
#include "CreatureLayer.h"
#include "HandPileLayer.h"
#include "Monster.h"
#include "Player.h"
#include "cocos2d.h"

USING_NS_CC;

std::unique_ptr<CombatUIController> CombatUIController::create()
{
    std::unique_ptr<CombatUIController> controller(new CombatUIController());
    return controller;
}

CombatUIController::CombatUIController()
    : currentScene_(nullptr)
    , handLayer_(nullptr)
    , creatureLayer_(nullptr)
    , uiReady_(false)
{
}

CombatUIController::~CombatUIController()
{
    // 清理资源
    currentScene_ = nullptr;
    handLayer_ = nullptr;
    creatureLayer_ = nullptr;
}

bool CombatUIController::init(Scene* scene)
{
    if (!scene) {
        CCLOG("[UIController] Error: Scene is null");
        return false;
    }

    currentScene_ = scene;

    // 获取UI层组件
    // 注意：这里直接从场景中查找子节点，需要确保节点名称正确
    auto castScene = dynamic_cast<CombatScene*>(scene);
    if (castScene) {
        // CombatScene的成员变量在头文件中定义，这里需要访问
        // 假设HandPileLayer和CreatureLayer是CombatScene的成员
        handLayer_ = castScene->handPileLayer; // 需要确认成员名称
        creatureLayer_ = castScene->creatureLayer;
    } else {
        // 如果不是CombatScene，尝试通过节点名称查找
        handLayer_ = dynamic_cast<HandPileLayer*>(scene->getChildByName("HandPileLayer"));
        creatureLayer_ = dynamic_cast<CreatureLayer*>(scene->getChildByName("CreatureLayer"));
    }

    // 检查UI组件是否找到
    if (!handLayer_) {
        CCLOG("[UIController] Warning: HandPileLayer not found");
    }

    if (!creatureLayer_) {
        CCLOG("[UIController] Warning: CreatureLayer not found");
    }

    uiReady_ = (handLayer_ != nullptr || creatureLayer_ != nullptr);

    CCLOG("[UIController] Initialized: handLayer=%s, creatureLayer=%s",
        handLayer_ ? "OK" : "NULL", creatureLayer_ ? "OK" : "NULL");

    return uiReady_;
}

void CombatUIController::updatePlayerDisplay(std::shared_ptr<Player> player)
{
    safeUpdateUI([this, player]() {
        if (creatureLayer_) {
            // CreatureLayer应该有一个updatePlayerDisplay方法
            // 这里我们直接调用updateDisplay，它会更新所有生物
            creatureLayer_->updateDisplay();
            CCLOG("[UIController] Player display updated");
        } else {
            CCLOG("[UIController] Warning: creatureLayer is null");
        }
    });
}

void CombatUIController::updateEnergyDisplay(int currentEnergy, int maxEnergy)
{
    safeUpdateUI([this, currentEnergy, maxEnergy]() {
        auto castScene = dynamic_cast<CombatScene*>(currentScene_);
        if (castScene && castScene->updateEnergyDisplay) {
            // 直接调用CombatScene的方法
            castScene->updateEnergyDisplay();
            CCLOG("[UIController] Energy display updated: %d/%d", currentEnergy, maxEnergy);
        } else {
            CCLOG("[UIController] Warning: Cannot update energy display");
        }
    });
}

void CombatUIController::updatePlayerBuffs(std::shared_ptr<Player> player)
{
    safeUpdateUI([this, player]() {
        // 玩家buff显示通常集成在CreatureLayer中
        if (creatureLayer_) {
            creatureLayer_->updateDisplay();
            CCLOG("[UIController] Player buffs updated");
        }
    });
}

void CombatUIController::updateMonstersDisplay(const std::vector<std::shared_ptr<Monster>>& monsters)
{
    // Capture monsters by value to avoid dangling reference if called asynchronously
    safeUpdateUI([this, monsters]() {
        if (creatureLayer_) {
            creatureLayer_->updateDisplay();
            CCLOG("[UIController] All monsters display updated (count: %zu)", monsters.size());
        } else {
            CCLOG("[UIController] Warning: creatureLayer is null");
        }
    });
}

void CombatUIController::updateMonsterDisplay(std::shared_ptr<Monster> monster)
{
    safeUpdateUI([this, monster]() {
        if (creatureLayer_) {
            // 单独更新一个怪物（如果有这个功能）
            creatureLayer_->updateDisplay();
            CCLOG("[UIController] Monster display updated: %s", monster->getName().c_str());
        }
    });
}

void CombatUIController::showMonsterIntention(std::shared_ptr<Monster> monster)
{
    safeUpdateUI([this, monster]() {
        // 显示怪物意图（攻击、防御等）
        // 这个功能可能需要CreatureLayer支持
        CCLOG("[UIController] Monster intention shown: %s", monster->getName().c_str());
    });
}

void CombatUIController::addCardToHand(std::shared_ptr<Card> card)
{
    safeUpdateUI([this, card]() {
        if (handLayer_ && handLayer_->drawCard) {
            handLayer_->drawCard(card.get());
            CCLOG("[UIController] Card added to hand: %s", card->getName().c_str());
        } else {
            CCLOG("[UIController] Warning: Cannot add card to hand");
        }
    });
}

void CombatUIController::removeCardFromHand(std::shared_ptr<Card> card)
{
    safeUpdateUI([this, card]() {
        if (handLayer_ && handLayer_->removeCard) {
            handLayer_->removeCard(card.get());
            CCLOG("[UIController] Card removed from hand: %s", card->getName().c_str());
        } else {
            CCLOG("[UIController] Warning: Cannot remove card from hand");
        }
    });
}

void CombatUIController::clearAllHandCards()
{
    safeUpdateUI([this]() {
        if (handLayer_ && handLayer_->clearAll) {
            handLayer_->clearAll();
            CCLOG("[UIController] All hand cards cleared");
        } else {
            CCLOG("[UIController] Warning: Cannot clear hand cards");
        }
    });
}

void CombatUIController::adjustHandLayout()
{
    safeUpdateUI([this]() {
        if (handLayer_ && handLayer_->adjustHandPile) {
            handLayer_->adjustHandPile();
            CCLOG("[UIController] Hand layout adjusted");
        } else {
            CCLOG("[UIController] Warning: Cannot adjust hand layout");
        }
    });
}

void CombatUIController::updateDrawPileDisplay(int count)
{
    safeUpdateUI([this, count]() {
        if (handLayer_ && handLayer_->updateDrawPileDisplay) {
            handLayer_->updateDrawPileDisplay();
            CCLOG("[UIController] Draw pile display updated: %d cards", count);
        } else {
            CCLOG("[UIController] Warning: Cannot update draw pile display");
        }
    });
}

void CombatUIController::updateDiscardPileDisplay(int count)
{
    safeUpdateUI([this, count]() {
        if (handLayer_ && handLayer_->updateDiscardPileDisplay) {
            handLayer_->updateDiscardPileDisplay();
            CCLOG("[UIController] Discard pile display updated: %d cards", count);
        } else {
            CCLOG("[UIController] Warning: Cannot update discard pile display");
        }
    });
}

void CombatUIController::playAttackAnimation(std::shared_ptr<Creature> attacker, std::shared_ptr<Creature> target, int damage)
{
    safeUpdateUI([this, attacker, target, damage]() {
        CCLOG("[UIController] Playing attack animation: %s -> %s (%d damage)",
            attacker->getName().c_str(), target->getName().c_str(), damage);
        // TODO: 实现具体的攻击动画
        // 可以使用Cocos2d的动作和动画系统
    });
}

void CombatUIController::playDamageAnimation(std::shared_ptr<Creature> target, int damage)
{
    safeUpdateUI([this, target, damage]() {
        CCLOG("[UIController] Playing damage animation: %s takes %d damage",
            target->getName().c_str(), damage);
        // TODO: 实现受伤动画（闪烁、伤害数字弹出等）
    });
}

void CombatUIController::playBlockAnimation(std::shared_ptr<Creature> target, int block)
{
    safeUpdateUI([this, target, block]() {
        CCLOG("[UIController] Playing block animation: %s gains %d block",
            target->getName().c_str(), block);
        // TODO: 实现防御动画
    });
}

void CombatUIController::playHealAnimation(std::shared_ptr<Creature> target, int heal)
{
    safeUpdateUI([this, target, heal]() {
        CCLOG("[UIController] Playing heal animation: %s heals %d",
            target->getName().c_str(), heal);
        // TODO: 实现治疗动画
    });
}

void CombatUIController::playBuffAnimation(std::shared_ptr<Creature> target, const std::string& buffName)
{
    safeUpdateUI([this, target, buffName]() {
        CCLOG("[UIController] Playing buff animation: %s gets %s",
            target->getName().c_str(), buffName.c_str());
        // TODO: 实现buff添加动画
    });
}

void CombatUIController::showBattleLog(const std::string& message, const std::string& type)
{
    safeUpdateUI([this, message, type]() {
        CCLOG("[UIController][%s] %s", type.c_str(), message.c_str());
        // TODO: 在UI底部显示滚动消息
    });
}

void CombatUIController::showTempMessage(const std::string& message, float duration)
{
    safeUpdateUI([this, message, duration]() {
        CCLOG("[UIController] Temp message: %s (%.1fs)", message.c_str(), duration);
        // TODO: 在屏幕中央显示短暂消息
    });
}

void CombatUIController::showVictoryScreen()
{
    safeUpdateUI([this]() {
        CCLOG("[UIController] Showing victory screen");
        // TODO: 显示胜利界面
        // 可能需要推送新场景或显示结算面板
    });
}

void CombatUIController::showDefeatScreen()
{
    safeUpdateUI([this]() {
        CCLOG("[UIController] Showing defeat screen");
        // TODO: 显示失败界面
        // 可能需要推送新场景或显示结算面板
    });
}

// Note: updateDiscardPileDisplay is already defined above (line ~206)
// Removed duplicate definition here

void CombatUIController::updateCardDisplay(std::shared_ptr<Card> card)
{
    safeUpdateUI([this, card]() {
        if (handLayer_) {
            auto cardSprite = handLayer_->getChildByTag(reinterpret_cast<intptr_t>(card.get()));
            if (cardSprite) {
                CardSpriteGenerator::updateCardSprite(card, cardSprite);
            }
        }
    });
}

std::vector<std::shared_ptr<Card>> CombatUIController::getSelectedCards()
{
    if (handLayer_) {
        return handLayer_->select_card_list;
    }
    return {};
}

void CombatUIController::updateCreatureDisplay(std::shared_ptr<Creature> creature)
{
    safeUpdateUI([this]() {
        if (creatureLayer_) {
            creatureLayer_->updateDisplay();
        }
    });
}

void CombatUIController::updateAllCreaturesDisplay()
{
    safeUpdateUI([this]() {
        if (creatureLayer_) {
            creatureLayer_->updateDisplay();
            CCLOG("[UIController] All creatures display updated");
        } else {
            CCLOG("[UIController] Warning: creatureLayer is null");
        }
    });
}

void CombatUIController::discardAllHandCardsDisplay()
{
    safeUpdateUI([this]() {
        if (handLayer_) {
            // 清空手牌显示
            handLayer_->clearAll();
            // 更新弃牌堆显示
            handLayer_->updateDiscardPileDisplay();
            CCLOG("[UIController] All hand cards discarded and display updated");
        } else {
            CCLOG("[UIController] Warning: handLayer is null");
        }
    });
}

void CombatUIController::removeCardSpriteByTag(void* cardPtr)
{
    safeUpdateUI([this, cardPtr]() {
        if (handLayer_) {
            auto cardSprite = handLayer_->getChildByTag(reinterpret_cast<intptr_t>(cardPtr));
            if (cardSprite) {
                handLayer_->removeChild(cardSprite);
                CCLOG("[UIController] Card sprite removed by tag");
            }
        } else {
            CCLOG("[UIController] Warning: handLayer is null");
        }
    });
}

void CombatUIController::safeUpdateUI(std::function<void()> updateFunc)
{
    if (!uiReady_) {
        CCLOG("[UIController] Warning: UI not ready, skipping update");
        return;
    }

    // 确保在Cocos2d主线程中执行
    auto director = Director::getInstance();
    if (director->getCurrentThread() == director->getOpenGLView()->getTaskScheduler()->getCurrentThreadId()) {
        // 已经在主线程，直接执行
        try {
            updateFunc();
        } catch (const std::exception& e) {
            CCLOG("[UIController] Exception in UI update: %s", e.what());
        }
    } else {
        // 不在主线程，使用调度器执行
        director->getScheduler()->performFunctionInCocosThread(updateFunc);
    }
}

#include "CombatDeck.h"
#include "Card.h"
#include "CardRegistry.h"
#include "IRandomGenerator.h"
#include "Player.h"
#include "cocos2d.h"

USING_NS_CC;

std::unique_ptr<CombatDeck> CombatDeck::create(std::shared_ptr<IRandomGenerator> randomGen)
{
    std::unique_ptr<CombatDeck> deck(new CombatDeck(randomGen));
    return deck;
}

CombatDeck::CombatDeck(std::shared_ptr<IRandomGenerator> randomGen)
    : randomGen_(randomGen)
{
    CCLOG("[CombatDeck] CombatDeck created");
}

CombatDeck::~CombatDeck()
{
    CCLOG("[CombatDeck] CombatDeck destroyed");
}

bool CombatDeck::init(std::shared_ptr<Player> player)
{
    if (!player) {
        CCLOG("[CombatDeck] Error: Player is null");
        return false;
    }

    CCLOG("[CombatDeck] Initializing deck for player: %s", player->getName().c_str());

    try {
        // Clear existing cards
        while (!drawPile_.empty())
            drawPile_.pop();
        while (!discardPile_.empty())
            discardPile_.pop();
        hand_.clear();

        // Load initial deck from CardRegistry
        // In a real scenario, this should come from Player's inventory
        std::vector<std::string> tempDeck = CardRegistry::getAllCardNames();
        if (tempDeck.empty()) {
            CCLOG("tempDeck is empty!");
        }

        // Add 4 copies of each card for testing (matching original logic)
        for (int i = 0; i < 4; i++) {
            for (auto name : tempDeck) {
                auto card = CardRegistry::createCard(name);
                if (card) {
                    addToDrawPile(card);
                    CCLOG("%s has been added!", name.c_str());
                }
            }
        }

        CCLOG("[CombatDeck] Initialized successfully - Draw pile: %zu, Discard pile: %zu",
            drawPile_.size(), discardPile_.size());

        // Shuffle the deck initially
        shuffle();

        return true;

    } catch (const std::exception& e) {
        CCLOG("[CombatDeck] Exception during initialization: %s", e.what());
        return false;
    }
}

int CombatDeck::drawCards(int count)
{
    CCLOG("[CombatDeck] Drawing %d cards", count);

    int drawnCount = 0;

    for (int i = 0; i < count; i++) {
        // 检查抽牌堆是否为空
        if (drawPile_.empty()) {
            // 如果抽牌堆为空，洗牌
            if (!discardPile_.empty()) {
                CCLOG("[CombatDeck] Draw pile empty, shuffling discard pile");
                shuffle();
            } else {
                CCLOG("[CombatDeck] No cards to draw");
                break;
            }
        }

        // 抽一张卡
        if (!drawPile_.empty()) {
            auto card = drawPile_.front();
            drawPile_.pop();

            // 添加到手牌
            if (addToHand(card)) {
                drawnCount++;
            } else {
                // 添加到手牌失败，放回抽牌堆
                CCLOG("[CombatDeck] Warning: Failed to add card to hand");
                drawPile_.push(card);
                break;
            }
        } else {
            break;
        }
    }

    CCLOG("[CombatDeck] Drew %d cards (hand size: %zu)", drawnCount, hand_.size());
    return drawnCount;
}

void CombatDeck::addToDrawPile(std::shared_ptr<Card> card, int count)
{
    if (!card) {
        CCLOG("[CombatDeck] Warning: Attempting to add null card to draw pile");
        return;
    }

    for (int i = 0; i < count; i++) {
        drawPile_.push(card);
    }

    CCLOG("[CombatDeck] Added %d copies of %s to draw pile (total draw pile: %zu)",
        count, card->getName().c_str(), drawPile_.size());

    notifyCardChanged("ADDED_TO_DRAW_PILE", card);
}

void CombatDeck::shuffle()
{
    CCLOG("[CombatDeck] Shuffling discard pile into draw pile");

    // 将弃牌堆转移到临时向量
    std::vector<std::shared_ptr<Card>> tempCards;
    while (!discardPile_.empty()) {
        tempCards.push_back(discardPile_.front());
        discardPile_.pop();
    }

    // 洗牌
    if (randomGen_ && !tempCards.empty()) {
        randomGen_->shuffleVector(tempCards);
    }

    // 加入抽牌堆
    for (const auto& card : tempCards) {
        drawPile_.push(card);
    }

    CCLOG("[CombatDeck] Shuffle complete (draw pile: %zu, discard pile: %zu)",
        drawPile_.size(), discardPile_.size());

    notifyCardChanged(CardEvent::SHUFFLED, nullptr);
}

bool CombatDeck::addToHand(std::shared_ptr<Card> card)
{
    if (!card) {
        CCLOG("[CombatDeck] Warning: Attempting to add null card to hand");
        return false;
    }

    hand_.push_back(card);
    CCLOG("[CombatDeck] Added %s to hand (hand size: %zu)", card->getName().c_str(), hand_.size());

    notifyCardChanged(CardEvent::ADDED_TO_HAND, card);
    return true;
}

bool CombatDeck::removeFromHand(std::shared_ptr<Card> card)
{
    if (!card) {
        return false;
    }

    bool removed = removeCardFromVector(hand_, card);
    if (removed) {
        CCLOG("[CombatDeck] Removed %s from hand (hand size: %zu)", card->getName().c_str(), hand_.size());
        notifyCardChanged(CardEvent::REMOVED_FROM_HAND, card);
    }

    return removed;
}

std::shared_ptr<Card> CombatDeck::removeFromHand(int index)
{
    if (index < 0 || index >= static_cast<int>(hand_.size())) {
        CCLOG("[CombatDeck] Warning: Invalid hand index %d (size: %zu)", index, hand_.size());
        return nullptr;
    }

    auto card = hand_[index];
    hand_.erase(hand_.begin() + index);

    CCLOG("[CombatDeck] Removed %s from hand at index %d", card->getName().c_str(), index);
    notifyCardChanged(CardEvent::REMOVED_FROM_HAND, card);

    return card;
}

void CombatDeck::clearHand()
{
    CCLOG("[CombatDeck] Clearing all hand cards (count: %zu)", hand_.size());

    for (const auto& card : hand_) {
        notifyCardChanged(CardEvent::REMOVED_FROM_HAND, card);
    }

    hand_.clear();
}

void CombatDeck::adjustHandLayout()
{
    // 这个方法通常由UI控制器调用，这里只是占位符
    CCLOG("[CombatDeck] Adjusting hand layout");
}

void CombatDeck::addToDiscardPile(std::shared_ptr<Card> card, int count)
{
    if (!card) {
        CCLOG("[CombatDeck] Warning: Attempting to add null card to discard pile");
        return;
    }

    for (int i = 0; i < count; i++) {
        discardPile_.push(card);
    }

    CCLOG("[CombatDeck] Added %d copies of %s to discard pile (total: %zu)",
        count, card->getName().c_str(), discardPile_.size());

    notifyCardChanged("ADDED_TO_DISCARD_PILE", card);
}

bool CombatDeck::discardCard(std::shared_ptr<Card> card)
{
    if (!card) {
        return false;
    }

    // 先从手牌移除
    if (!removeFromHand(card)) {
        return false;
    }

    // 加入弃牌堆
    moveCardToDiscard(card);

    CCLOG("[CombatDeck] Discarded %s", card->getName().c_str());
    return true;
}

bool CombatDeck::discardCard(int index)
{
    auto card = removeFromHand(index);
    if (card) {
        moveCardToDiscard(card);
        CCLOG("[CombatDeck] Discarded card at index %d", index);
        return true;
    }
    return false;
}

bool CombatDeck::exhaustCard(std::shared_ptr<Card> card)
{
    if (!card) {
        return false;
    }

    // 从手牌移除但不加入弃牌堆
    if (removeFromHand(card)) {
        CCLOG("[CombatDeck] Exhausted %s", card->getName().c_str());
        notifyCardChanged(CardEvent::EXHAUSTED, card);
        return true;
    }

    return false;
}

bool CombatDeck::exhaustCard(int index)
{
    auto card = removeFromHand(index);
    if (card) {
        CCLOG("[CombatDeck] Exhausted card at index %d", index);
        notifyCardChanged(CardEvent::EXHAUSTED, card);
        return true;
    }
    return false;
}

bool CombatDeck::upgradeCard(std::shared_ptr<Card> card)
{
    if (!card) {
        return false;
    }

    // 查找卡牌在手牌中的位置
    for (auto& handCard : hand_) {
        if (handCard == card) {
            if (!card->isUpgraded()) {
                card->upgrade();
                CCLOG("[CombatDeck] Upgraded %s", card->getName().c_str());
                notifyCardChanged(CardEvent::UPGRADED, card);
            } else {
                CCLOG("[CombatDeck] Card %s is already upgraded", card->getName().c_str());
            }
            return true;
        }
    }

    CCLOG("[CombatDeck] Warning: Card not found in hand for upgrade");
    return false;
}

bool CombatDeck::upgradeCard(int index)
{
    if (index < 0 || index >= static_cast<int>(hand_.size())) {
        CCLOG("[CombatDeck] Warning: Invalid hand index for upgrade: %d", index);
        return false;
    }

    return upgradeCard(hand_[index]);
}

bool CombatDeck::hasCardInHand(std::shared_ptr<Card> card) const
{
    if (!card) {
        return false;
    }

    for (const auto& handCard : hand_) {
        if (handCard == card) {
            return true;
        }
    }

    return false;
}

int CombatDeck::getCardIndexInHand(std::shared_ptr<Card> card) const
{
    if (!card) {
        return -1;
    }

    for (int i = 0; i < static_cast<int>(hand_.size()); i++) {
        if (hand_[i] == card) {
            return i;
        }
    }

    return -1;
}

int CombatDeck::getTotalCardCount() const
{
    return drawPile_.size() + hand_.size() + discardPile_.size();
}

void CombatDeck::notifyCardChanged(const std::string& event, std::shared_ptr<Card> card)
{
    if (cardChangedCallback_) {
        try {
            cardChangedCallback_(event, card);
        } catch (const std::exception& e) {
            CCLOG("[CombatDeck] Exception in card changed callback: %s", e.what());
        }
    }
}

bool CombatDeck::moveCardToDiscard(std::shared_ptr<Card> card)
{
    if (!card) {
        return false;
    }

    discardPile_.push(card);
    notifyCardChanged(CardEvent::DISCARDED, card);
    return true;
}

bool CombatDeck::removeCardFromVector(std::vector<std::shared_ptr<Card>>& vec, std::shared_ptr<Card> card)
{
    if (!card) {
        return false;
    }

    auto it = std::find(vec.begin(), vec.end(), card);
    if (it != vec.end()) {
        vec.erase(it);
        return true;
    }

    return false;
}

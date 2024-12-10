#pragma once
#include "cocos2d.h"
#include <memory>
#include <queue>
#include <vector>
using namespace cocos2d;
class Card;
class CombatSystem;
class CombatDeck;
class Creature;

class HandPileLayer : public cocos2d::Layer
{
private:
    static HandPileLayer* instance_;
    cocos2d::Label* drawPileNumLabel;
    cocos2d::Label* discardPileNumLabel;

public:

    bool init() override;

    // 返回单例
    static HandPileLayer* HandPileLayer::getInstance();

    // 启用拖动
    void enableCardDrag(Sprite* cardSprite, std::shared_ptr<Card> card);

    // 抽一张牌
    void drawCard(std::shared_ptr<Card> card);

    // 移除一张牌
    void removeCard(std::shared_ptr<Card> card);

    // 更新手牌显示
    void adjustHandPile();

    void HandPileLayer::updateDrawPileDisplay();//更新抽牌堆数量显示

    void HandPileLayer::updateDiscardPileDisplay();//更新手牌数量显示

    CREATE_FUNC(HandPileLayer);
};
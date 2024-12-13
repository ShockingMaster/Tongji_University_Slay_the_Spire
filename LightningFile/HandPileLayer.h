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


public:
    HandPileLayer():_currentSceneType(SceneType::SCENE_TYPE_1) {}
    enum class SceneType {
        SCENE_TYPE_1,
        SCENE_TYPE_2
    };

    void setSceneType(SceneType type) {
        _currentSceneType = type;
    }

    bool init() override;

    // 返回单例
    static HandPileLayer* HandPileLayer::getInstance();

    // 启用拖动
    void enableCardDrag(Sprite* cardSprite, std::shared_ptr<Card> card);


    void enableCardHighlight (Sprite* cardSprite, std::shared_ptr<Card> card);

    void switchToCardHighlight( std::shared_ptr<Card> card);

    void switchToenableCardDrag(std::shared_ptr<Card> card);

    void adjustfunction();
    

    // 抽一张牌
    void drawCard(std::shared_ptr<Card> card);

    // 移除一张牌
    void removeCard(std::shared_ptr<Card> card);

    // 更新手牌显示
    void adjustHandPile();

    void HandPileLayer::updateDrawPileDisplay();//更新抽牌堆数量显示

    void HandPileLayer::updateDiscardPileDisplay();//更新手牌数量显示

    CREATE_FUNC(HandPileLayer);


private:
    static HandPileLayer* instance_;
    cocos2d::Label* drawPileNumLabel;
    cocos2d::Label* discardPileNumLabel;
    SceneType _currentSceneType;
};
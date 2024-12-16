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
    int card_num_select = 0;
    int card_num_select_target = 0;
    bool canSwitchScene = false;
    std::vector<std::shared_ptr<Card>> select_card_list;

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
    void  exhaustCard();

    bool judge_select_num() {
        if (card_num_select_target == card_num_select) {
            return true;
        }
        else
            return false;
    }


    // 启用拖动
    void enableCardDrag(Sprite* cardSprite, std::shared_ptr<Card> card);


    void enableCardHighlight (Sprite* cardSprite, std::shared_ptr<Card> card);

    void switchToCardHighlight( std::shared_ptr<Card> card);

    void switchToenableCardDrag(std::shared_ptr<Card> card);
    

    // 抽一张牌
    void drawCard(std::shared_ptr<Card> card);

    // 移除一张牌
    void removeCard(std::shared_ptr<Card> card);

    // 更新手牌显示
    void adjustHandPile();

    void switchToblank(std::shared_ptr<Card> card);

    void HandPileLayer::updateDrawPileDisplay();//更新抽牌堆数量显示

    void HandPileLayer::updateDiscardPileDisplay();//更新手牌数量显示

    CREATE_FUNC(HandPileLayer);


private:
    static HandPileLayer* instance_;
    cocos2d::Label* drawPileNumLabel;
    cocos2d::Label* discardPileNumLabel;
    SceneType _currentSceneType;
};
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

    // ���ص���
    static HandPileLayer* HandPileLayer::getInstance();
    void  exhaustCard();

    bool judge_select_num() {
        if (card_num_select_target == card_num_select) {
            return true;
        }
        else
            return false;
    }


    // �����϶�
    void enableCardDrag(Sprite* cardSprite, std::shared_ptr<Card> card);


    void enableCardHighlight (Sprite* cardSprite, std::shared_ptr<Card> card);

    void switchToCardHighlight( std::shared_ptr<Card> card);

    void switchToenableCardDrag(std::shared_ptr<Card> card);
    

    // ��һ����
    void drawCard(std::shared_ptr<Card> card);

    // �Ƴ�һ����
    void removeCard(std::shared_ptr<Card> card);

    // ����������ʾ
    void adjustHandPile();

    void switchToblank(std::shared_ptr<Card> card);

    void HandPileLayer::updateDrawPileDisplay();//���³��ƶ�������ʾ

    void HandPileLayer::updateDiscardPileDisplay();//��������������ʾ

    CREATE_FUNC(HandPileLayer);


private:
    static HandPileLayer* instance_;
    cocos2d::Label* drawPileNumLabel;
    cocos2d::Label* discardPileNumLabel;
    SceneType _currentSceneType;
};
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

    // ���ص���
    static HandPileLayer* HandPileLayer::getInstance();

    // �����϶�
    void enableCardDrag(Sprite* cardSprite, std::shared_ptr<Card> card);

    // ��һ����
    void drawCard(std::shared_ptr<Card> card);

    // �Ƴ�һ����
    void removeCard(std::shared_ptr<Card> card);

    // ����������ʾ
    void adjustHandPile();

    void HandPileLayer::updateDrawPileDisplay();//���³��ƶ�������ʾ

    void HandPileLayer::updateDiscardPileDisplay();//��������������ʾ

    CREATE_FUNC(HandPileLayer);
};
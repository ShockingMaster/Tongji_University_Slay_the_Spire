#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "Card.h"
#include "CardLayer.h"

enum CardLayerOperation {
    Show = 1,
    Delete = 2,
    Upgrade = 3,
};

class CardLayer : public cocos2d::Layer {
public:
    CardLayer();
    ~CardLayer();
    int operation;          //要进行的行为
    static CardLayer* create(std::vector<std::shared_ptr<Card>> cards, int op);
    bool init(std::vector<std::shared_ptr<Card>> cards, int op);
    void displayCards();
    void updateCardBoundingBoxes();
    void closeLayerCallback(cocos2d::Ref* sender);
    void onMouseScroll(cocos2d::EventMouse* event);
    std::vector<std::shared_ptr<Card>> _cards;
private:
    std::vector<cocos2d::Sprite*>_cardSprites;
    cocos2d::ui::ScrollView* _scrollView;  // 卡牌的滚动视图
    cocos2d::Sprite* _background;  // 背景图片
}; 
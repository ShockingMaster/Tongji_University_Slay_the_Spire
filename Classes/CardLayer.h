#pragma once
#ifndef CARD_LAYER_h
#define CARD_LAYER_h

#include "cocos2d.h"
#include "Card.h" 
#include "ui/CocosGUI.h" 

#pragma once
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "Card.h"

class CardLayer : public cocos2d::Layer {
public:
    CardLayer();
    ~CardLayer();

    static CardLayer* create(std::vector<Card*> cards);
    bool init(std::vector<Card*> cards);
    void displayCards();

    void closeLayerCallback(cocos2d::Ref* sender);

private:
    std::vector<Card*> _cards;
    cocos2d::ui::ScrollView* _scrollView;  // ¿¨ÅÆµÄ¹ö¶¯ÊÓÍ¼
    cocos2d::Sprite* _background;  // ±³¾°Í¼Æ¬
};



#endif
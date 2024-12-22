#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "Player.h"

enum ShowLayerOperation {
    GetCoin = 1,
    SingleCard = 2,
    OneRelic = 3,
    ManyCardSelect = 4,
    OnePotion=5
};

class ShowLayer : public cocos2d::Layer {
public:
    ShowLayer();
    ~ShowLayer();
    int operation;          //要进行的行为
    static  ShowLayer* create(int op);
    bool init(int op);
    void closeLayerCallback(cocos2d::Ref* sender);
private:
    cocos2d::Sprite* _background;  // 背景图片
};
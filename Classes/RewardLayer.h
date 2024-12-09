#pragma once
#ifndef __REWARD_LAYER_H__
#define __REWARD_LAYER_H__

#include "cocos2d.h"
#include "Card.h"
#include "Relic.h"
class RewardLayer : public cocos2d::Layer
{
public:
    // 构造和析构函数
    RewardLayer();
    ~RewardLayer();
    // 创建 RewardLayer 层
    static RewardLayer* create();

    // 初始化 RewardLayer
    virtual bool init();

    // 按钮点击事件回调
    void onCloseButtonClicked(cocos2d::Ref* sender);

 

private:

};

#endif // __REWARD_LAYER_H__

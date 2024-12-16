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
    bool coins;     //奖励是否包含金币
    bool singlecard;  //获得一卡牌
    bool selectcard;  //获得选择卡牌的机会
    bool potion;    //奖励是否包含药水
    bool relic;     //奖励是否包含遗物
    
    RewardLayer();
    ~RewardLayer();
    // 创建 RewardLayer 层
    static RewardLayer* create(bool coins, bool singlecard, bool selectcard, bool potion, bool relic);

    // 初始化 RewardLayer
    virtual bool init(bool coins, bool singlecard, bool selectcard, bool potion, bool relic);

    // 按钮点击事件回调
    void onCloseButtonClicked(cocos2d::Ref* sender);

 

private:

};

#endif // __REWARD_LAYER_H__

#pragma once
#ifndef __REWARD_LAYER_H__
#define __REWARD_LAYER_H__

#include "cocos2d.h"
#include "Card.h"
#include "Relic.h"
class RewardLayer : public cocos2d::Layer
{
public:
    // �������������
    RewardLayer();
    ~RewardLayer();
    // ���� RewardLayer ��
    static RewardLayer* create();

    // ��ʼ�� RewardLayer
    virtual bool init();

    // ��ť����¼��ص�
    void onCloseButtonClicked(cocos2d::Ref* sender);

 

private:

};

#endif // __REWARD_LAYER_H__

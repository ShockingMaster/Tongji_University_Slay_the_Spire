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
    bool coins;     //�����Ƿ�������
    bool singlecard;  //���һ����
    bool selectcard;  //���ѡ���ƵĻ���
    bool potion;    //�����Ƿ����ҩˮ
    bool relic;     //�����Ƿ��������
    
    RewardLayer();
    ~RewardLayer();
    // ���� RewardLayer ��
    static RewardLayer* create(bool coins, bool potion, bool relic, bool singlecard, bool selectcard);

    // ��ʼ�� RewardLayer
    virtual bool init(bool coins, bool potion, bool relic, bool singlecard, bool selectcard);

    // ��ť����¼��ص�
    void onCloseButtonClicked(cocos2d::Ref* sender);

 

private:

};

#endif // __REWARD_LAYER_H__

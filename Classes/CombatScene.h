#pragma once
#include "CombatDeck.h"
#include "cocos2d.h"
#include "CombatNode.h"

class CombatScene : public cocos2d::Scene {
public:

    CombatNode combat_node_;

    CombatDeck combat_deck_;

    // ���������ľ�̬����
    static cocos2d::Scene* createScene();

    // ��ʼ������
    virtual bool init();

    // CREATE_FUNC���Զ�����Ĭ�ϵ� init() ����
    CREATE_FUNC(CombatScene);
};

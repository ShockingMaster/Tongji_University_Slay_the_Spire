#pragma once

#include "cocos2d.h"

class ChestScene : public cocos2d::Scene {
public:

    // ���������ľ�̬����
    static cocos2d::Scene* createScene();

    // ��ʼ������
    virtual bool init();

    // CREATE_FUNC���Զ�����Ĭ�ϵ� init() ����
    CREATE_FUNC(ChestScene);
};



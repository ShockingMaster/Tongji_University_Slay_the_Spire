#pragma once

#include "cocos2d.h"

class EndingScene : public cocos2d::Scene {
public:
    // ʤ����־��true ��ʾʤ����false ��ʾʧ��
    bool victory;

    // ���������ľ�̬����
    static EndingScene* create(bool isVictory);

    // ��ʼ������
    virtual bool init(bool isVictory);

    // ��������
    virtual ~EndingScene() {}
};

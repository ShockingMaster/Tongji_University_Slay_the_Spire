#pragma once

#ifndef __SHOP_SCENE_H__
#define __SHOP_SCENE_H__

#include "cocos2d.h"

class Shop : public cocos2d::Scene {
public:
 
    // ���������ľ�̬����
    static cocos2d::Scene* createScene();

    // ��ʼ������
    virtual bool init();

    // CREATE_FUNC���Զ�����Ĭ�ϵ� init() ����
    CREATE_FUNC(Shop);
};

#endif // __SHOP_SCENE_H__


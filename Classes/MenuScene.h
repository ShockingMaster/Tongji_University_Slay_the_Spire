#pragma once                      // ȷ��ͷ�ļ�ֻ������һ��
#ifndef _MENU_SCENE_H_             
#define _MENU_SCENE_H_

#include "cocos2d.h"              

/**
 * MenuScene�ࣺ��Ϸ���˵������࣬�̳���cocos2d::Scene��
 * ���ڴ�����������Ϸ�����˵����档
 */
class MenuScene : public cocos2d::Scene {
public:
    /**
     * ����������������̬����������һ���µ�MenuScene����ʵ����
     * @return ����������һ���µĳ���ʵ��
     */
    static cocos2d::Scene* createScene();

    /**
     * ��ʼ����������ʼ���˵����������ñ�������ť��Ԫ�ء�
     * @return �����ʼ���ɹ��򷵻�true��ʧ���򷵻�false
     */
    virtual bool init();

    // ʹ��CREATE_FUNC���Զ����ɴ���������create()��
    CREATE_FUNC(MenuScene);
};

#endif  // _MENU_SCENE_H_

#pragma once
#ifndef _INITIAL_SCENE_H_
#define _INITIAL_SCENE_H_

#include "cocos2d.h"

/*
 * Class Name:     InitialScene
 * Class Function: ��ʼ������
 */
class InitialScene : public cocos2d::Scene {
public:
    /**
     * ����������һ����������
     * @return һ��ָ�� cocos2d::Scene ��ָ�룬��ʾ�ó�ʼ������
     */
    static cocos2d::Scene* createScene();

    /**
     * ��ʼ��������
     * @return �����ʼ���ɹ����� true�����򷵻� false��
     */
    virtual bool init();

    /**
     * ʹ�� CREATE_FUNC ������ create ������
     * CREATE_FUNC �� cocos2d-x �ṩ�ĺ꣬���ڼ򻯶��󴴽����̡�
     */
    CREATE_FUNC(InitialScene);

private:
    /**
     * ��鵥���ַ��ĺϷ��ԡ�
     * @param ch Ҫ�����ַ���֧�� Unicode ���룩��
     * @return ����ַ��Ϸ����� true�����򷵻� false��
     * @note ���Ը���������Ϸ��ַ��ķ�Χ������ֻ������ĸ�����ֵȡ�
     */
    bool isValidCharacter(const char32_t ch);

    /**
     * ����ַ����ĺϷ��ԡ�
     * @param str Ҫ�����ַ�����
     * @return ����ַ����Ϸ����� true�����򷵻� false��
     * @note �÷������ܵ��� isValidCharacter �������ַ����е�ÿ���ַ���
     */
    bool isValidString(const std::string& str);
};

#endif // !1
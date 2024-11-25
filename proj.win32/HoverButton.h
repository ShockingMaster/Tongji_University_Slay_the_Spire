#pragma once
#ifndef HOVER_BUTTON_H
#define HOVER_BUTTON_H

#include "cocos2d.h"
#include "ui/CocosGUI.h"



 // HoverButton ��̳��� cocos2d::ui::Button������ʵ��֧�������ͣ�͵��״̬�л��İ�ť��
 // �ṩ�Զ���ͼƬ�Ĺ��ܣ�Ĭ��״̬ͼƬ�������ͣ״̬ͼƬ������״̬ͼƬ��

class HoverButton : public cocos2d::ui::Button {
public:
    /**
     * ��̬�������������ڴ��� HoverButton ����
     * @param defaultButtonImage ��ť��Ĭ��ͼƬ·����
     * @param hoverButtonImage �����ͣʱ��ͼƬ·����
     * @param activeButtonImage ��갴��ʱ��ͼƬ·����
     * @return ����һ�� HoverButton ����
     */
    static HoverButton* create(const std::string& defaultButtonImage,
        const std::string& hoverButtonImage,
        const std::string& activeButtonImage);

    /**
     * ��ʼ�� HoverButton ����
     * @param defaultButtonImage ��ť��Ĭ��ͼƬ·����
     * @param hoverButtonImage �����ͣʱ��ͼƬ·����
     * @param activeButtonImage ��갴��ʱ��ͼƬ·����
     * @return �����ʼ���ɹ����� true�����򷵻� false��
     */
    virtual bool init(const std::string& defaultButtonImage,
        const std::string& hoverButtonImage,
        const std::string& activeButtonImage);

    /**
     * ����ƶ��¼��ص�������
     * ���ڼ�������ڰ�ť�ϵ��ƶ������л�����ͣͼƬ��
     * @param event ����¼���
     */
    void onMouseMove(cocos2d::Event* event);

    /**
     * ����ɿ��¼��ص�������
     * ���ڼ�����갴���ɿ��Ķ��������л���Ĭ��ͼƬ��
     * @param event ����¼���
     */
    void onMouseUp(cocos2d::Event* event);

private:
    // ��ť��Ĭ��ͼƬ·����
    std::string defaultButtonImage;

    // �����ͣ״̬�µ�ͼƬ·����
    std::string hoverButtonImage;

    // ��갴��״̬�µ�ͼƬ·����
    std::string activeButtonImage;

    // ָʾĬ��ͼƬ����ͣͼƬ�Ƿ���ͬ�ı�־��
    // ���Ϊ true������ͣͼƬ��Ĭ��ͼƬһ�£���ʡ��Դ���ء�
    bool sameImages;
};

#endif // HOVER_BUTTON_H

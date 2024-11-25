#pragma once
#ifndef HOVER_BUTTON_H
#define HOVER_BUTTON_H

#include "cocos2d.h"
#include "ui/CocosGUI.h"



 // HoverButton 类继承自 cocos2d::ui::Button，用于实现支持鼠标悬停和点击状态切换的按钮。
 // 提供自定义图片的功能：默认状态图片、鼠标悬停状态图片、按下状态图片。

class HoverButton : public cocos2d::ui::Button {
public:
    /**
     * 静态创建函数，用于创建 HoverButton 对象。
     * @param defaultButtonImage 按钮的默认图片路径。
     * @param hoverButtonImage 鼠标悬停时的图片路径。
     * @param activeButtonImage 鼠标按下时的图片路径。
     * @return 返回一个 HoverButton 对象。
     */
    static HoverButton* create(const std::string& defaultButtonImage,
        const std::string& hoverButtonImage,
        const std::string& activeButtonImage);

    /**
     * 初始化 HoverButton 对象。
     * @param defaultButtonImage 按钮的默认图片路径。
     * @param hoverButtonImage 鼠标悬停时的图片路径。
     * @param activeButtonImage 鼠标按下时的图片路径。
     * @return 如果初始化成功返回 true，否则返回 false。
     */
    virtual bool init(const std::string& defaultButtonImage,
        const std::string& hoverButtonImage,
        const std::string& activeButtonImage);

    /**
     * 鼠标移动事件回调函数。
     * 用于监听鼠标在按钮上的移动，并切换到悬停图片。
     * @param event 鼠标事件。
     */
    void onMouseMove(cocos2d::Event* event);

    /**
     * 鼠标松开事件回调函数。
     * 用于监听鼠标按键松开的动作，并切换到默认图片。
     * @param event 鼠标事件。
     */
    void onMouseUp(cocos2d::Event* event);

private:
    // 按钮的默认图片路径。
    std::string defaultButtonImage;

    // 鼠标悬停状态下的图片路径。
    std::string hoverButtonImage;

    // 鼠标按下状态下的图片路径。
    std::string activeButtonImage;

    // 指示默认图片和悬停图片是否相同的标志。
    // 如果为 true，则悬停图片和默认图片一致，节省资源加载。
    bool sameImages;
};

#endif // HOVER_BUTTON_H

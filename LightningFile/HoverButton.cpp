#include "HoverButton.h"

// 创建一个新的 HoverButton 实例
HoverButton* HoverButton::create(const std::string& defaultButtonImage, const std::string& hoverButtonImage, const std::string& activeButtonImage)
{
    HoverButton* button = new(std::nothrow) HoverButton();
    if (button && button->init(defaultButtonImage, hoverButtonImage, activeButtonImage)) {
        button->autorelease();
        return button;
    }
    CC_SAFE_DELETE(button);
    return nullptr;
}


// 初始化 HoverButton 实例
bool HoverButton::init(const std::string& defaultButtonImage, const std::string& hoverButtonImage, const std::string& activeButtonImage)
{
    // 初始化
    if (!Button::init(defaultButtonImage, activeButtonImage, "")) {
        return false;
    }
    this->defaultButtonImage = defaultButtonImage;
    this->hoverButtonImage = hoverButtonImage;
    this->activeButtonImage = activeButtonImage;

    // 鼠标事件监听
    auto listener = cocos2d::EventListenerMouse::create();
    listener->onMouseMove = CC_CALLBACK_1(HoverButton::onMouseMove, this);
    listener->onMouseUp = CC_CALLBACK_1(HoverButton::onMouseUp, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    // 检查按钮图片是否一致
    sameImages = (defaultButtonImage == hoverButtonImage && hoverButtonImage == activeButtonImage);

    return true;
}

// 鼠标移动事件的处理函数
void HoverButton::onMouseMove(cocos2d::Event* event)
{
    const auto mouseEvent = dynamic_cast<cocos2d::EventMouse*>(event);
    if (mouseEvent && this->getBoundingBox().containsPoint(this->getParent()->convertToNodeSpace(mouseEvent->getLocationInView()))) {
        if (sameImages) {
            // 图片一致时放大 1.25 倍
            this->setScale(1.5f);
        }
        else {
            // 图片不一致时更换为 hover 图片
            this->loadTextureNormal(hoverButtonImage);
        }
    }
    else {
        // 鼠标移出时恢复默认图片与大小
        this->setScale(1.25f);
        this->loadTextureNormal(defaultButtonImage);
    }
}

// 鼠标释放事件的处理函数
void HoverButton::onMouseUp(cocos2d::Event* event)
{
    const auto mouseEvent = dynamic_cast<cocos2d::EventMouse*>(event);
    if (mouseEvent && this->getBoundingBox().containsPoint(this->getParent()->convertToNodeSpace(mouseEvent->getLocationInView()))) {
        if (!sameImages) {
            // 图片不一致时切换到 active 图片
            this->loadTextureNormal(activeButtonImage);
        }
    }
}

#include "EventScene.h"
#include "IncludeAll.h"
bool EventScene::init() 
{
    if (!Scene::init()) {
        return false;
    }

    // 创建背景
    auto background = Sprite::create(backgroundImage_);
    background->setPosition(Vec2(1100, 100));
    this->addChild(background);


    // 设置背景图片
    auto eventbackground = Sprite::create(eventImage_);
    eventbackground->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2 - 550,
        Director::getInstance()->getVisibleSize().height / 2 - 100));
    this->addChild(eventbackground);


    // 显示事件描述文本
    auto eventLabel = Label::createWithSystemFont(eventText_, "Fonts/Kreon-Bold.ttf", 45);
    eventLabel->setPosition(Vec2(1350, 600));
    eventLabel->setColor(Color3B::WHITE);
    this->addChild(eventLabel);

    // 动态计算按钮的位置
    float startY = 300;
    for (size_t i = 0; i < buttons_.size(); ++i) {
        auto button = HoverButton::create("button4(1).png", "button4(2).png", "button4(1).png");
        button->setPosition(Vec2(1500, startY - i * 150));
        this->addChild(button);

        // 创建按钮文本
        auto buttonLabel = Label::createWithSystemFont(buttons_[i].first, "Fonts/Kreon-Bold.ttf", 45);
        buttonLabel->setPosition(Vec2(1200, startY - i * 150));
        buttonLabel->setColor(Color3B::WHITE);
        this->addChild(buttonLabel);

        // 按钮点击事件
        button->addClickEventListener([=](Ref* sender) {
            buttons_[i].second();  // 执行按钮的点击事件
            });
    }

    return true;
}

EventScene* EventScene::create(const std::string& backgroundImage, const std::string& eventImage,
    const std::string& eventText, const std::vector<std::pair<std::string, std::function<void()>>>& buttons) {
    EventScene* ret = new EventScene(backgroundImage, eventImage, eventText, buttons);
    if (ret && ret->init()) {
        ret->autorelease();
        return ret;
    }
    else {
        delete ret;
        return nullptr;
    }
}


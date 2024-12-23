#pragma once
#include "cocos2d.h"
#include "HoverButton.h"
USING_NS_CC;
class EventScene : public Scene {
public:
    EventScene(const std::string& backgroundImage, const std::string& eventImage, const std::string& eventText,
        const std::vector<std::pair<std::string, std::function<void()>>>& buttons)
        : backgroundImage_(backgroundImage), eventImage_(eventImage), eventText_(eventText), buttons_(buttons) {}

    virtual bool init() override;

    static EventScene* create(const std::string& backgroundImage, const std::string& eventImage,
        const std::string& eventText, const std::vector<std::pair<std::string, std::function<void()>>>& buttons);

protected:
    std::string backgroundImage_;
    std::string eventImage_;
    std::string eventText_;
    std::vector<std::pair<std::string, std::function<void()>>> buttons_;
};


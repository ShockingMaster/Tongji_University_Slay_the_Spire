#pragma once
#include "EventScene.h"
class Event1 : public EventScene {
public:
    Event1()
        : EventScene("walkscene.jpg", "addict.jpg", u8"你想要从一群披着斗篷的人旁边偷偷潜行过去...\n他看起来疯疯癫癫的，但并没有危险", {
            {u8"给他85金币\n换取一个遗物", [this]() { onGiveGold(); }},
            {u8"不理会，离开", [this]() { onLeave(); }}
            }) {}

    void onGiveGold();

    void onLeave();

    static Event1* create() {
        Event1* ret = new Event1();
        if (ret && ret->init()) {
            ret->autorelease();
            return ret;
        }
        else {
            delete ret;
            return nullptr;
        }
    }
};
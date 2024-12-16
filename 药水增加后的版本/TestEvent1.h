#pragma once
#include "EventScene.h"
class Event1 : public EventScene {
public:
    Event1()
        : EventScene("walkscene.jpg", "addict.jpg", u8"����Ҫ��һȺ���Ŷ�������Ա�͵͵Ǳ�й�ȥ...\n�������������ģ�����û��Σ��", {
            {u8"����85���\n��ȡһ������", [this]() { onGiveGold(); }},
            {u8"����ᣬ�뿪", [this]() { onLeave(); }}
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
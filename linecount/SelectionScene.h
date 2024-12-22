
#pragma once
#ifndef _SELECTION_SCENE_H_
#define _SELECTION_SCENE_H_

#include "cocos2d.h"


class SelectionScene : public cocos2d::Scene {
public:

    static cocos2d::Scene* createScene();

    virtual bool init();

    CREATE_FUNC(SelectionScene);
};

#endif
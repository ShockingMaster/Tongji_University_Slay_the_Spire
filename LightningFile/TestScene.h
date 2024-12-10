#include "cocos2d.h"
class CombatScene;
class TestScene : public cocos2d::Scene {
public:
    bool init();

    void switchToCombatScene(cocos2d::Ref* sender);

    void switchToSelectScene(cocos2d::Ref* sender);

    CREATE_FUNC(TestScene);
};

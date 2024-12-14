#pragma once
#include "cocos2d.h"
#include <memory>
#include <queue>
#include <vector>
using namespace cocos2d;
class Card;
class CombatSystem;
class CombatDeck;
class Creature;

class SelectScene : public cocos2d::Scene
{
private:
    //测试使用：
    cocos2d::Rect playArea;
    cocos2d::Label* energyLabel;
    std::function<void()> callback_;
    
    static SelectScene* instance_;
    HoverButton* switchSceneButton;
public:
    bool init();

    static SelectScene* SelectScene::getInstance();


    static SelectScene* createScene(int someValue) {
        SelectScene* scene = SelectScene::create();
        scene->initWithValue(someValue);  // 初始化场景时传递参数
        return scene;
    }

    bool initWithValue(int Value) {
        if (!Scene::init()) {
            return false;
        }

        card_select_num = Value;

        return true;
    }
    

    //测试使用
    int isMyTurn;

    static void onEndTurnClicked(cocos2d::Ref* sender);//当回合结束按钮被点击时

    void updateEnergyDisplay();//更新能量显示

    void update_button();

    void setCallback(std::function<void()> callback);

    cocos2d::Rect getplayArea()
    {
        return playArea;
    }
    CREATE_FUNC(SelectScene);

    int card_select_num = 0;
};
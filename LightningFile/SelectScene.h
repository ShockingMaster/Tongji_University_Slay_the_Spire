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
    //����ʹ�ã�
    cocos2d::Rect playArea;
    cocos2d::Label* energyLabel;
    static SelectScene* instance_;
    HoverButton* switchSceneButton;
    CombatScene* combatScene = nullptr;
public:
    bool init();

    void setCombatScene(CombatScene* scene) {
        combatScene = scene;  // ���� CombatScene ָ��
    }

    static SelectScene* SelectScene::getInstance();


    static SelectScene* createScene(int someValue) {
        SelectScene* scene = SelectScene::create();
        scene->initWithValue(someValue);  // ?????????????????
        return scene;
    }

    bool initWithValue(int Value) {
        if (!Scene::init()) {
            return false;
        }

        card_select_num = Value;

        return true;
    }

    //����ʹ��
    int isMyTurn;

    static void onEndTurnClicked(cocos2d::Ref* sender);//���غϽ�����ť�����ʱ

    void updateEnergyDisplay();//����������ʾ

    void update_button();


    cocos2d::Rect getplayArea()
    {
        return playArea;
    }
    CREATE_FUNC(SelectScene);

    int card_select_num = 0;
};
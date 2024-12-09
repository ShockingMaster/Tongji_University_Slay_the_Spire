#include "cocos2d.h"
#include <memory>
#include <queue>
#include <vector>
using namespace cocos2d;
class Card;
class CombatSystem;
class CombatDeck;
class Creature;

class HandPileLayer : public cocos2d::Layer
{
private:
    static HandPileLayer* instance_;
public:
    bool init() override;
	
    static HandPileLayer* HandPileLayer::getInstance();//返回单例

    //启用拖动
    void enableCardDrag(Sprite* cardSprite, std::shared_ptr<Card> card);

    // 抽一张牌
    void drawCard(std::shared_ptr<Card> card);

    // 更新手牌显示
    void adjustHandPile();

    CREATE_FUNC(HandPileLayer);
};

class CombatScene : public cocos2d::Scene
{
private:
    HandPileLayer* handPileLayer;
    //测试使用：
    cocos2d::Rect playArea;
    cocos2d::Label* energyLabel;
    cocos2d::Label* drawPileNumLabel;
    cocos2d::Label* discardPileNumLabel;
public:
    bool init();
    
    //测试使用
    int isMyTurn;

    static void onEndTurnClicked(cocos2d::Ref* sender);//当回合结束按钮被点击时

    void updateEnergyDisplay();//更新能量显示

    void CombatScene::updateDrawPileDisplay();//更新抽牌堆数量显示

    void CombatScene::updateDiscardPileDisplay();//更新手牌数量显示

    cocos2d::Rect getplayArea()
    {
        return playArea;
    }
    CREATE_FUNC(CombatScene);
};
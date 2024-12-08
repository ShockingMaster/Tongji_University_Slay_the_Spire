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
    CombatSystem* combatSystem;
    std::vector<std::shared_ptr<Card>> hand;    // 引用手牌堆
    std::queue<std::shared_ptr<Card>> discardPile;  // 引用弃牌堆
    std::vector<cocos2d::Sprite*> cardSprites;  // 存储手牌中的卡牌精灵
    cocos2d::Sprite* draggedCard;               // 当前被拖动的卡牌
    cocos2d::Vec2 dragStartPos;                 // 记录卡牌的起始位置
    static HandPileLayer* instance_;
public:
    bool init() override;
	

    static HandPileLayer* HandPileLayer::getInstance();//返回单例

    // 每次更新手牌显示
    void updateHandPile();

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
public:
    bool init();
    //测试使用
    int isMyTurn;
    static void onEndTurnClicked(cocos2d::Ref* sender);//当回合结束按钮被点击时

    cocos2d::Rect getplayArea()
    {
        return playArea;
    }
    CREATE_FUNC(CombatScene);
};
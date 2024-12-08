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
    std::vector<std::shared_ptr<Card>> hand;    // �������ƶ�
    std::queue<std::shared_ptr<Card>> discardPile;  // �������ƶ�
    std::vector<cocos2d::Sprite*> cardSprites;  // �洢�����еĿ��ƾ���
    cocos2d::Sprite* draggedCard;               // ��ǰ���϶��Ŀ���
    cocos2d::Vec2 dragStartPos;                 // ��¼���Ƶ���ʼλ��
    static HandPileLayer* instance_;
public:
    bool init() override;
	

    static HandPileLayer* HandPileLayer::getInstance();//���ص���

    // ÿ�θ���������ʾ
    void updateHandPile();

    //�����϶�
    void enableCardDrag(Sprite* cardSprite, std::shared_ptr<Card> card);

    // ��һ����
    void drawCard(std::shared_ptr<Card> card);

    // ����������ʾ
    void adjustHandPile();

    CREATE_FUNC(HandPileLayer);
};

class CombatScene : public cocos2d::Scene
{
private:
    HandPileLayer* handPileLayer;

    //����ʹ�ã�
    cocos2d::Rect playArea;
public:
    bool init();
    //����ʹ��
    int isMyTurn;
    static void onEndTurnClicked(cocos2d::Ref* sender);//���غϽ�����ť�����ʱ

    cocos2d::Rect getplayArea()
    {
        return playArea;
    }
    CREATE_FUNC(CombatScene);
};
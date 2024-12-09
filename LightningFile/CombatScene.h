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
	
    static HandPileLayer* HandPileLayer::getInstance();//���ص���

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
    cocos2d::Label* energyLabel;
    cocos2d::Label* drawPileNumLabel;
    cocos2d::Label* discardPileNumLabel;
public:
    bool init();
    
    //����ʹ��
    int isMyTurn;

    static void onEndTurnClicked(cocos2d::Ref* sender);//���غϽ�����ť�����ʱ

    void updateEnergyDisplay();//����������ʾ

    void CombatScene::updateDrawPileDisplay();//���³��ƶ�������ʾ

    void CombatScene::updateDiscardPileDisplay();//��������������ʾ

    cocos2d::Rect getplayArea()
    {
        return playArea;
    }
    CREATE_FUNC(CombatScene);
};
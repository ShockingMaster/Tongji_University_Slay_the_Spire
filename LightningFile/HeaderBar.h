#pragma once
#ifndef _HEADER_BAR_H_
#define _HEADER_BAR_H_

#include "Potion.h"
#include "cocos2d.h"
#include "Player.h"
#include "Relic.h"
#include "Card.h"
#include "CardLayer.h"
#include <memory> // For smart pointers
using namespace std;
using namespace cocos2d;

class HeaderBar : public Node {
private:
    string name;                // �������
    string character;           // ��ҽ�ɫ
    int health;                 // ��ǰ����ֵ
    int fullHealth;             // �������ֵ
    int maxPotions;             // ҩˮ�������
    int coins;                  // �������
    vector<Potion*> potions;    // ҩˮ
    vector<Relic*> relics;      // ����
    Label* nameLabel;           // ��ʾ���Ƶı�ǩ
    Label* healthLabel;         // ��ʾ����ֵ�ı�ǩ
    Label* coinsLabel;          // ��ʾ��������ı�ǩ
    Label* levelLabel;          // ��ʾ��ǰ�ؿ��ı�ǩ
    Node* potionIcons;          // ��ʾҩˮͼ�������
    Node* relicIcons;           // ��ʾҩˮͼ�������
    Sprite* healthIcon;         // ����ͼ��
    Sprite* coinsIcon;          // ���ͼ��
    Sprite* potionIcon;         // ҩˮͼ��
    vector<Card*> cards;        // ��ǰ����
    static HeaderBar* instance;

public:
    // ���캯������������
    int level;  // ��ǰ�ؿ�
    HeaderBar();
    ~HeaderBar();

    // ��ʼ��ͷ��
    bool init(shared_ptr<Player> player);

    // ��ȡΨһʵ��
    static HeaderBar* getInstance();

    // ����ͷ����Ϣ
    void updateHeader(shared_ptr<Player> player);

    // ��̬����������Cocos2dԼ����
    static HeaderBar* create(shared_ptr<Player> player);

    // ���úͻ�ȡ����
    void setPlayerInfo(const string& name, const string& character, int fullHealth, int coins);
    void setPlayerInfo(shared_ptr<Player> player);

    // ���������Ϣ�ĺ���
    void setHealth(int health);
    void setCoins(int coins);
    void setPotions(const vector<Potion*>& potions);
    void setLevel(int level);

    // ��ȡͷ����Ϣ
    int getCurrentHealth();
    int getFullHealth();
    int getCoins();

    // �¼�ϵͳ
    int changeCoins(int coinChange);                   //�޸Ľ��
    void changeHealth(int healthChange);               //�޸�����ֵ����Ҫ�ж��Ƿ�����
    void changeMaxHealth(int maxHealthChange);         //�޸��������ֵ
    int upgradeCard(std::shared_ptr<Card> card);       //�Կ��ƽ�����������
    int deleteCard(std::shared_ptr<Card> card);        //�Կ��ƽ�������ɾ��
    int addPotion(std::shared_ptr<Potion> potion);     //���ҩˮ
    void addRelic(std::shared_ptr<Relic> relic);       //�������
    void climbFloor();                                 //����¥��
};

// GlobalNode �࣬���ڴ洢��פԪ��
class GlobalNode : public Node {
public:
    static GlobalNode* getInstance() {
        static GlobalNode instance;
        return &instance;
    }

    void addToScene(Scene* scene) {
        scene->addChild(this);  // ����פ�ڵ���ӵ�������
    }

    void removeFromScene() {
        if (this->getParent()) {
            this->getParent()->removeChild(this);  // �Ƴ���פ�ڵ�
        }
    }

    void addHeaderBar(HeaderBar* headerBar) {
        this->addChild(headerBar);
    }

private:
    GlobalNode() {}  // ��ֹ�ⲿʵ����
};

#endif // _HEADER_BAR_H_

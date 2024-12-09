#pragma once
#ifndef _HEADER_BAR_H_
#define _HEADER_BAR_H_

#include "Potion.h"
#include "cocos2d.h"
#include "Player.h"
#include"Relic.h"
#include"Card.h"
#include "CardLayer.h"
using namespace std;
using namespace cocos2d;



class HeaderBar : public Node {
private:
    string name;              // �������
    string character;         // ��ҽ�ɫ
    int health;               // ��ǰ����ֵ
    int fullHealth;           // �������ֵ
    int coins;                // �������
    vector<Potion*> potions;  // ҩˮ
    vector<Relic*> relics;    // ����
    Label* nameLabel;         // ��ʾ���Ƶı�ǩ
    Label* healthLabel;       // ��ʾ����ֵ�ı�ǩ
    Label* coinsLabel;        // ��ʾ��������ı�ǩ
    Label* levelLabel;        // ��ʾ��ǰ�ؿ��ı�ǩ
    Node* potionIcons;        // ��ʾҩˮͼ�������
    Node* relicIcons;        // ��ʾҩˮͼ�������
    Sprite* healthIcon;       // ����ͼ��
    Sprite* coinsIcon;        // ���ͼ��
    Sprite* potionIcon;       // ҩˮͼ��
    vector<Card*> cards;
public:
    // ���캯������������
    int level; // ��ǰ�ؿ�
    HeaderBar();
    ~HeaderBar();

    // ��ʼ��ͷ��
    bool init(Player* player);

    // ����ͷ����Ϣ
    void updateHeader(Player* player);

    // ��̬����������Cocos2dԼ����
    static HeaderBar* create(Player* player);

    // ���úͻ�ȡ����
    void setPlayerInfo(const string& name, const string& character, int fullHealth, int coins);
    void setPlayerInfo(Player* player);

    // ���������Ϣ�ĺ���
    void setHealth(int health);
    void setCoins(int coins);
    void setPotions(const vector<Potion*>& potions);
    void setLevel(int level);
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


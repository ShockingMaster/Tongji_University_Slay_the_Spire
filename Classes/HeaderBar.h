#pragma once
#ifndef _HEADER_BAR_H_
#define _HEADER_BAR_H_

#include "Potion.h"
#include "cocos2d.h"
#include "Player.h"

using namespace std;
using namespace cocos2d;



class HearderBar : public Node {
private:
    string name;              // �������
    string character;         // ��ҽ�ɫ
    int health;               // ��ǰ����ֵ
    int fullHealth;           // �������ֵ
    int coins;                // �������
    vector<Potion*> potions;  // ҩˮ
    int level; // ��ǰ�ؿ�

    Label* nameLabel;         // ��ʾ���Ƶı�ǩ
    Label* healthLabel;       // ��ʾ����ֵ�ı�ǩ
    Label* coinsLabel;        // ��ʾ��������ı�ǩ
    Label* levelLabel;        // ��ʾ��ǰ�ؿ��ı�ǩ
    Node* potionIcons;        // ��ʾҩˮͼ�������
    Sprite* healthIcon;       // ����ͼ��
    Sprite* coinsIcon;        // ���ͼ��
    Sprite* potionIcon;       // ҩˮͼ��

public:
    // ���캯������������
    HearderBar();
    ~HearderBar();

    // ��ʼ��ͷ��
    bool init(Player* player);

    // ����ͷ����Ϣ
    void updateHeader(Player* player);

    // ��ʾͷ������
    void displayHeader();

    // ��̬����������Cocos2dԼ����
    static HearderBar* create(Player* player);

    // ���úͻ�ȡ����
    void setPlayerInfo(const string& name, const string& character, int fullHealth, int coins);
    void setPlayerInfo(Player* player);

    // ���������Ϣ�ĺ���
    void setHealth(int health);
    void setCoins(int coins);
    void setPotions(const vector<Potion*>& potions);
    void setLevel(int level);
};

#endif // _HEADER_BAR_H_


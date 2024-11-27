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
    string name;              // 玩家名称
    string character;         // 玩家角色
    int health;               // 当前生命值
    int fullHealth;           // 最大生命值
    int coins;                // 金币数量
    vector<Potion*> potions;  // 药水
    int level; // 当前关卡

    Label* nameLabel;         // 显示名称的标签
    Label* healthLabel;       // 显示生命值的标签
    Label* coinsLabel;        // 显示金币数量的标签
    Label* levelLabel;        // 显示当前关卡的标签
    Node* potionIcons;        // 显示药水图标的容器
    Sprite* healthIcon;       // 生命图标
    Sprite* coinsIcon;        // 金币图标
    Sprite* potionIcon;       // 药水图标

public:
    // 构造函数和析构函数
    HearderBar();
    ~HearderBar();

    // 初始化头栏
    bool init(Player* player);

    // 更新头栏信息
    void updateHeader(Player* player);

    // 显示头栏内容
    void displayHeader();

    // 静态创建函数（Cocos2d约定）
    static HearderBar* create(Player* player);

    // 设置和获取函数
    void setPlayerInfo(const string& name, const string& character, int fullHealth, int coins);
    void setPlayerInfo(Player* player);

    // 设置玩家信息的函数
    void setHealth(int health);
    void setCoins(int coins);
    void setPotions(const vector<Potion*>& potions);
    void setLevel(int level);
};

#endif // _HEADER_BAR_H_


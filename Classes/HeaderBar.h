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

class EventSystem;
class HeaderBar : public Node {
private:
    string name;                                // 玩家名称
    string character;                           // 玩家角色
    int health;                                 // 当前生命值
    int fullHealth;                             // 最大生命值
    int maxPotions;                             // 药水最大数量
    int coins;                                  // 金币数量
    vector<std::shared_ptr<Potion>> potions;    // 药水
    vector<std::shared_ptr<Relic>> relics;      // 遗物
    Label* nameLabel;                           // 显示名称的标签
    Label* healthLabel;                         // 显示生命值的标签
    Label* coinsLabel;                          // 显示金币数量的标签
    Label* levelLabel;                          // 显示当前关卡的标签
    Node* potionIcons;                          // 显示药水图标的容器
    Node* relicIcons;                           // 显示药水图标的容器
    Sprite* healthIcon;                         // 生命图标
    Sprite* coinsIcon;                          // 金币图标
    Sprite* potionIcon;                         // 药水图标
    vector<std::shared_ptr<Card>> cards;        // 当前卡牌

public:
    // 构造函数和析构函数
    int level;  // 当前关卡
    HeaderBar();
    ~HeaderBar();

    // 初始化头栏
    bool init(EventSystem* eventSystem);

    // 更新头栏信息
    void updateHeader(EventSystem* eventSystem);

    // 静态创建函数（Cocos2d约定）
    static HeaderBar* create(EventSystem* eventSystem);

    // 设置和获取函数
    void setPlayerInfo(const string& name, const string& character, int fullHealth, int coins);
    void setPlayerInfo(EventSystem* eventSystem);

    // 设置玩家信息的函数
    void setHealth(int health);
    void setCoins(int coins);
    void setPotions(const vector<std::shared_ptr<Potion>>& potions);
    void setLevel(int level);

    // 获取头栏信息
    int getCurrentHealth();
    int getFullHealth();
    int getCoins();
};

// GlobalNode 类，用于存储常驻元素
class GlobalNode : public Node {
public:
    static GlobalNode* getInstance() {
        static GlobalNode instance;
        return &instance;
    }

    void addToScene(Scene* scene) {
        scene->addChild(this);  // 将常驻节点添加到场景中
    }

    void removeFromScene() {
        if (this->getParent()) {
            this->getParent()->removeChild(this);  // 移除常驻节点
        }
    }

    void addHeaderBar(HeaderBar* headerBar) {
        this->addChild(headerBar);
    }

private:
    GlobalNode() {}  // 防止外部实例化
};

#endif // _HEADER_BAR_H_

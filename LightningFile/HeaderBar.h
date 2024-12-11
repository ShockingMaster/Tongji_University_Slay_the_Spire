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
    string name;                // 玩家名称
    string character;           // 玩家角色
    int health;                 // 当前生命值
    int fullHealth;             // 最大生命值
    int maxPotions;             // 药水最大数量
    int coins;                  // 金币数量
    vector<Potion*> potions;    // 药水
    vector<Relic*> relics;      // 遗物
    Label* nameLabel;           // 显示名称的标签
    Label* healthLabel;         // 显示生命值的标签
    Label* coinsLabel;          // 显示金币数量的标签
    Label* levelLabel;          // 显示当前关卡的标签
    Node* potionIcons;          // 显示药水图标的容器
    Node* relicIcons;           // 显示药水图标的容器
    Sprite* healthIcon;         // 生命图标
    Sprite* coinsIcon;          // 金币图标
    Sprite* potionIcon;         // 药水图标
    vector<Card*> cards;        // 当前卡牌
    static HeaderBar* instance;

public:
    // 构造函数和析构函数
    int level;  // 当前关卡
    HeaderBar();
    ~HeaderBar();

    // 初始化头栏
    bool init(shared_ptr<Player> player);

    // 获取唯一实例
    static HeaderBar* getInstance();

    // 更新头栏信息
    void updateHeader(shared_ptr<Player> player);

    // 静态创建函数（Cocos2d约定）
    static HeaderBar* create(shared_ptr<Player> player);

    // 设置和获取函数
    void setPlayerInfo(const string& name, const string& character, int fullHealth, int coins);
    void setPlayerInfo(shared_ptr<Player> player);

    // 设置玩家信息的函数
    void setHealth(int health);
    void setCoins(int coins);
    void setPotions(const vector<Potion*>& potions);
    void setLevel(int level);

    // 获取头栏信息
    int getCurrentHealth();
    int getFullHealth();
    int getCoins();

    // 事件系统
    int changeCoins(int coinChange);                   //修改金币
    void changeHealth(int healthChange);               //修改生命值：需要判断是否死亡
    void changeMaxHealth(int maxHealthChange);         //修改最大生命值
    int upgradeCard(std::shared_ptr<Card> card);       //对卡牌进行永久升级
    int deleteCard(std::shared_ptr<Card> card);        //对卡牌进行永久删除
    int addPotion(std::shared_ptr<Potion> potion);     //添加药水
    void addRelic(std::shared_ptr<Relic> relic);       //添加遗物
    void climbFloor();                                 //上升楼层
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

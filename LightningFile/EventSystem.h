#pragma once
#include "Potion.h"
#include "cocos2d.h"
#include "Player.h"
#include "Relic.h"
#include "Card.h"
#include "CardLayer.h"
#include <memory> // For smart pointers
using namespace std;
using namespace cocos2d;

class EventSystem : public Node {
private:

    static EventSystem* instance;

public:
    int level;  // 当前关卡
    string name_;                                // 玩家名称
    string character_;                           // 玩家角色
    int health_;                                 // 当前生命值
    int fullHealth_;                             // 最大生命值
    int maxPotions_;                             // 药水最大数量
    int coins_;                                  // 金币数量
    vector<std::shared_ptr<Potion>> potions_;    // 药水
    vector<std::shared_ptr<Relic>> relics_;      // 遗物
    vector<std::shared_ptr<Card>> cards_;        // 当前卡牌
    // 构造函数和析构函数
    EventSystem();
    ~EventSystem();

    // 初始化头栏
    bool init();

    // 获取唯一实例
    static EventSystem* getInstance();

    // 设置和获取函数
    void setPlayerInfo(const string& name, const string& character, int fullHealth, int coins);

    // 设置玩家信息的函数
    void setHealth(int health);
    void setCoins(int coins);
    void setPotions(const vector<std::shared_ptr<Potion>>& potions);
    void setLevel(int level);
    void setFullHealth(int health_);

    // 获取头栏信息
    int getCurrentHealth();
    int getFullHealth();
    int getCoins();

    // 事件系统
    int changeCoins(int coinChange);                   //修改金币
    void changeHealth(int healthChange);               //修改生命值：需要判断是否死亡
    void changeMaxHealth(int maxHealthChange);         //修改最大生命值
    int upgradeCard(std::shared_ptr<Card> card);       //对卡牌进行永久升级
    int upgradeCard(Card* card);
    int deleteCard(std::shared_ptr<Card> card);        //对卡牌进行永久删除
    int deleteCard(Card* card);
    void addCard(std::shared_ptr<Card> card);          //添加卡牌
    int addPotion(std::shared_ptr<Potion> potion);     //添加药水
    void addRelic(std::shared_ptr<Relic> relic);       //添加遗物
};

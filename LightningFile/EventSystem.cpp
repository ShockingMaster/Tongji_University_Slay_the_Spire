#include "EventSystem.h"
#include "IncludeAll.h"
#include "cocos2d.h"
#include "MapScene.h"
#include "CardLayer.h"
#include "AudioPlayer.h"
using namespace std;
using namespace cocos2d;

extern int  currentLevel;

EventSystem* EventSystem::instance = nullptr;  // 定义静态成员变量


// 构造函数
EventSystem::EventSystem()
    : name_(""), character_(""), health_(0), fullHealth_(0), coins_(0), level(0) {}

// 析构函数
EventSystem::~EventSystem() {}

// 初始化头栏
bool EventSystem::init() {

    return true;
}

// 返回唯一实例
EventSystem* EventSystem::getInstance()
{
    if (!instance) {
        instance = new EventSystem();
    }
    return instance;
}

// 设置玩家信息
void EventSystem::setPlayerInfo(const string& name, const string& character, int fullHealth, int coins) {
    this->name_ = name;
    this->character_ = character;
    this->fullHealth_ = fullHealth;
    this->coins_ = coins;
}


// 设置生命值
void EventSystem::setHealth(int health) {
    this->health_ = health;
}

// 设置最大生命值
void EventSystem::setFullHealth(int health_) {
    this->fullHealth_ = health_;
}

// 设置金币
void EventSystem::setCoins(int coins) {
    this->coins_ = coins;
}

// 设置药水
void EventSystem::setPotions(const vector<std::shared_ptr<Potion>>& potions) {
    this->potions_ = potions;
}

// 设置关卡
void EventSystem::setLevel(int level) {
    this->level = level;
}

// 返回当前生命值
int EventSystem::getCurrentHealth()
{
    return health_;
}

// 返回最大生命值
int EventSystem::getFullHealth()
{
    return fullHealth_;
}

// 返回当前金币
int EventSystem::getCoins()
{
    return coins_;
}

// 对当前金币进行修改，正数代表增加金币，负数代表减少金币
int EventSystem::changeCoins(int coinChange)
{
    // 如果当前金币不足，则返回0
    if (coins_ + coinChange < 0)
    {
        return 0;
    }
    else
    {
        coins_ += coinChange;
        return 1;
    }
}

//对当前生命值进行修改
void EventSystem::changeHealth(int healthChange)
{
    // 触发遗物效果
    int tempHealthChange = healthChange;
    for (auto Relic : EventSystem::getInstance()->relics_)
    {
        //Relic->onHealthChange(tempHealthChange);
    }

    // 当生命值低于0时，触发遗物效果
    if (health_ + tempHealthChange < 0)
    {
        for (auto Relic : EventSystem::getInstance()->relics_)
        {
            //Relic->onDeath();
        }
        // 如果仍然低于0，则游戏结束
        if (health_ + tempHealthChange < 0)
        {
            //GameOver!
        }
    }

    // 对生命值进行修改
    health_ = min(fullHealth_, health_ + tempHealthChange);
}

void EventSystem::changeMaxHealth(int maxHealthChange)
{
    int tempMaxHealthChange = maxHealthChange;
    for (auto Relic : EventSystem::getInstance()->relics_)
    {
        //Relic->onChangeMaxHealth(tempMaxHealthChange);
    }
    // 先进行最大生命上限的修改
    fullHealth_ = fullHealth_ + tempMaxHealthChange;

    // 如果当前生命最大上限值小于当前生命值，则修改当前生命值
    if (fullHealth_ < health_)
    {
        EventSystem::getInstance()->changeHealth(fullHealth_ - health_);
    }

}

int EventSystem::upgradeCard(std::shared_ptr<Card> card)
{

    return 0;
}

int EventSystem::upgradeCard(Card* card)
{
    return 0;
}

int EventSystem::deleteCard(std::shared_ptr<Card> card)
{
    return 1;
}


int EventSystem::deleteCard(Card* card)
{
    return 1;
}


int EventSystem::addPotion(std::shared_ptr<Potion> potion)
{
    return 0;
}

void EventSystem::addRelic(std::shared_ptr<Relic> relic)
{

}

void EventSystem::climbFloor()
{

}


#include "EventSystem.h"
#include "IncludeAll.h"
#include "cocos2d.h"
#include "MapScene.h"
#include "CardLayer.h"
#include "AudioPlayer.h"
#include "Register.h"
using namespace std;
using namespace cocos2d;

extern int  currentLevel;

EventSystem* EventSystem::instance = nullptr;  // 定义静态成员变量


// 构造函数
EventSystem::EventSystem()
    : name_(""), character_(""), health_(0), fullHealth_(0), coins_(0), level(0) {}

// 析构函数
EventSystem::~EventSystem() {}

// 初始化
bool EventSystem::init() 
{
    // 设定数值
    this->coins_ = 99;
    this->maxPotions_ = 2;
    this->health_ = 80;
    this->fullHealth_ = 80;
    
    // 清空药水和卡组
    cards_.clear();
    potions_.clear();

    for (int i = 0;i < 5;i++)
    {
        auto AttackCard = CardRegistry::createCard("Attack");
        this->addCard(AttackCard);
    }
    for (int i = 0; i < 4; i++)
    {
        auto DefenseCard = CardRegistry::createCard("Defense");
        this->addCard(DefenseCard);
    }
    this->addCard(CardRegistry::createCard("Apotheosis"));
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
    int tempCoinChange = coinChange;
    for (auto Relic : EventSystem::getInstance()->relics_)
    {
        Relic->onCoinChange(tempCoinChange);
    }
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
        Relic->onHealthChange(tempHealthChange);
    }

    // 当生命值低于0时，触发遗物效果
    if (health_ + tempHealthChange < 0)
    {
        for (auto Relic : EventSystem::getInstance()->relics_)
        {
            Relic->onDeath();
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

// 对最大生命上限进行修改
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

// 对卡牌进行永久升级
int EventSystem::upgradeCard(std::shared_ptr<Card> card)
{
    for (int i = 0;i < EventSystem::getInstance()->cards_.size();i++)
    {
        // 如果找到了卡牌
        if (card == EventSystem::getInstance()->cards_[i])
        {
            // 如果卡牌已经被升级过，那么返回0
            if (card->isUpgraded())
            {
                return 0;
            }
            else
            {
                card->upgrade();
                return 1;
            }
        }
    }
    // 没有找到卡牌同样返回0
    return 0;
}


// 对卡牌进行永久升级
int EventSystem::upgradeCard(Card* card)
{
    for (int i = 0;i < EventSystem::getInstance()->cards_.size();i++)
    {
        // 如果找到了卡牌
        if (card == EventSystem::getInstance()->cards_[i].get())
        {
            // 如果卡牌已经被升级过
            if (EventSystem::getInstance()->cards_[i]->isUpgraded())
            {
                return 0;
            }
            // 如果卡牌没有被升级过，那么进行升级，同时返回1
            else
            {
                EventSystem::getInstance()->cards_[i]->upgrade();
                return 1;
            }
        }
    }
    // 没有找到返回0
    return 0;
}

// 对卡牌进行永久删除
int EventSystem::deleteCard(std::shared_ptr<Card> card)
{
    for (int i = 0;i < EventSystem::getInstance()->cards_.size();i++)
    {
        // 如果找到了卡牌，对卡牌进行删除
        if (card == EventSystem::getInstance()->cards_[i])
        {
            auto it = cards_.begin() + i;
            cards_.erase(it);
            return 1;
        }
    }
    return 0;
}

// 对卡牌进行永久删除
int EventSystem::deleteCard(Card* card)
{
    for (int i = 0;i < EventSystem::getInstance()->cards_.size();i++)
    {
        //如果找到了卡牌，对卡牌进行删除
        if (card == EventSystem::getInstance()->cards_[i].get())
        {
            auto it = cards_.begin() + i;
            cards_.erase(it);
            return 1;
        }
    }
    return 0;
}

// 添加卡牌
void EventSystem::addCard(std::shared_ptr<Card> card)
{
    for (auto Relic : EventSystem::getInstance()->relics_)
    {
        Relic->onAddCard(card);
    }
    if (card)
    {
        EventSystem::getInstance()->cards_.push_back(card);
    }
}

// 添加药水
int EventSystem::addPotion(std::shared_ptr<Potion> potion)
{
    // 查看当前的药水数量是否达到上限
    if (EventSystem::getInstance()->potions_.size() >= 2)
    {
        return 0;
    }
    for (auto Relic : EventSystem::getInstance()->relics_)
    {
        Relic->onAddPotion(potion);
    }
    if (potion)
    {
        EventSystem::getInstance()->potions_.push_back(potion);
    }
    return 1;
}

// 添加遗物
void EventSystem::addRelic(std::shared_ptr<Relic> relic)
{
    for (auto Relic : EventSystem::getInstance()->relics_)
    {
        Relic->onAddRelic(relic);
    }
    if (relic)
    {
        EventSystem::getInstance()->relics_.push_back(relic);
    }
}


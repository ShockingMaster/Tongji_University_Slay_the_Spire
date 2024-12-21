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

EventSystem* EventSystem::instance = nullptr;  // ���徲̬��Ա����


// ���캯��
EventSystem::EventSystem()
    : name_(""), character_(""), health_(0), fullHealth_(0), coins_(0), level(0) {}

// ��������
EventSystem::~EventSystem() {}

// ��ʼ��
bool EventSystem::init() 
{
    // �趨��ֵ
    this->coins_ = 99;
    this->maxPotions_ = 2;
    this->health_ = 80;
    this->fullHealth_ = 80;
    
    // ���ҩˮ�Ϳ���
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

// ����Ψһʵ��
EventSystem* EventSystem::getInstance()
{
    if (!instance) {
        instance = new EventSystem();
    }
    return instance;
}

// ���������Ϣ
void EventSystem::setPlayerInfo(const string& name, const string& character, int fullHealth, int coins) {
    this->name_ = name;
    this->character_ = character;
    this->fullHealth_ = fullHealth;
    this->coins_ = coins;
}


// ��������ֵ
void EventSystem::setHealth(int health) {
    this->health_ = health;
}

// �����������ֵ
void EventSystem::setFullHealth(int health_) {
    this->fullHealth_ = health_;
}

// ���ý��
void EventSystem::setCoins(int coins) {
    this->coins_ = coins;
}

// ����ҩˮ
void EventSystem::setPotions(const vector<std::shared_ptr<Potion>>& potions) {
    this->potions_ = potions;
}

// ���ùؿ�
void EventSystem::setLevel(int level) {
    this->level = level;
}

// ���ص�ǰ����ֵ
int EventSystem::getCurrentHealth()
{
    return health_;
}

// �����������ֵ
int EventSystem::getFullHealth()
{
    return fullHealth_;
}

// ���ص�ǰ���
int EventSystem::getCoins()
{
    return coins_;
}

// �Ե�ǰ��ҽ����޸ģ������������ӽ�ң�����������ٽ��
int EventSystem::changeCoins(int coinChange)
{
    int tempCoinChange = coinChange;
    for (auto Relic : EventSystem::getInstance()->relics_)
    {
        Relic->onCoinChange(tempCoinChange);
    }
    // �����ǰ��Ҳ��㣬�򷵻�0
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

//�Ե�ǰ����ֵ�����޸�
void EventSystem::changeHealth(int healthChange)
{
    // ��������Ч��
    int tempHealthChange = healthChange;
    for (auto Relic : EventSystem::getInstance()->relics_)
    {
        Relic->onHealthChange(tempHealthChange);
    }

    // ������ֵ����0ʱ����������Ч��
    if (health_ + tempHealthChange < 0)
    {
        for (auto Relic : EventSystem::getInstance()->relics_)
        {
            Relic->onDeath();
        }
        // �����Ȼ����0������Ϸ����
        if (health_ + tempHealthChange < 0)
        {
            //GameOver!
        }
    }

    // ������ֵ�����޸�
    health_ = min(fullHealth_, health_ + tempHealthChange);
}

// ������������޽����޸�
void EventSystem::changeMaxHealth(int maxHealthChange)
{
    int tempMaxHealthChange = maxHealthChange;
    for (auto Relic : EventSystem::getInstance()->relics_)
    {
        //Relic->onChangeMaxHealth(tempMaxHealthChange);
    }
    // �Ƚ�������������޵��޸�
    fullHealth_ = fullHealth_ + tempMaxHealthChange;

    // �����ǰ�����������ֵС�ڵ�ǰ����ֵ�����޸ĵ�ǰ����ֵ
    if (fullHealth_ < health_)
    {
        EventSystem::getInstance()->changeHealth(fullHealth_ - health_);
    }

}

// �Կ��ƽ�����������
int EventSystem::upgradeCard(std::shared_ptr<Card> card)
{
    for (int i = 0;i < EventSystem::getInstance()->cards_.size();i++)
    {
        // ����ҵ��˿���
        if (card == EventSystem::getInstance()->cards_[i])
        {
            // ��������Ѿ�������������ô����0
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
    // û���ҵ�����ͬ������0
    return 0;
}


// �Կ��ƽ�����������
int EventSystem::upgradeCard(Card* card)
{
    for (int i = 0;i < EventSystem::getInstance()->cards_.size();i++)
    {
        // ����ҵ��˿���
        if (card == EventSystem::getInstance()->cards_[i].get())
        {
            // ��������Ѿ���������
            if (EventSystem::getInstance()->cards_[i]->isUpgraded())
            {
                return 0;
            }
            // �������û�б�����������ô����������ͬʱ����1
            else
            {
                EventSystem::getInstance()->cards_[i]->upgrade();
                return 1;
            }
        }
    }
    // û���ҵ�����0
    return 0;
}

// �Կ��ƽ�������ɾ��
int EventSystem::deleteCard(std::shared_ptr<Card> card)
{
    for (int i = 0;i < EventSystem::getInstance()->cards_.size();i++)
    {
        // ����ҵ��˿��ƣ��Կ��ƽ���ɾ��
        if (card == EventSystem::getInstance()->cards_[i])
        {
            auto it = cards_.begin() + i;
            cards_.erase(it);
            return 1;
        }
    }
    return 0;
}

// �Կ��ƽ�������ɾ��
int EventSystem::deleteCard(Card* card)
{
    for (int i = 0;i < EventSystem::getInstance()->cards_.size();i++)
    {
        //����ҵ��˿��ƣ��Կ��ƽ���ɾ��
        if (card == EventSystem::getInstance()->cards_[i].get())
        {
            auto it = cards_.begin() + i;
            cards_.erase(it);
            return 1;
        }
    }
    return 0;
}

// ��ӿ���
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

// ���ҩˮ
int EventSystem::addPotion(std::shared_ptr<Potion> potion)
{
    // �鿴��ǰ��ҩˮ�����Ƿ�ﵽ����
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

// �������
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


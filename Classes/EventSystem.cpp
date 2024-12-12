#include "EventSystem.h"
#include "IncludeAll.h"
#include "cocos2d.h"
#include "MapScene.h"
#include "CardLayer.h"
#include "AudioPlayer.h"
using namespace std;
using namespace cocos2d;

extern int  currentLevel;

EventSystem* EventSystem::instance = nullptr;  // ���徲̬��Ա����


// ���캯��
EventSystem::EventSystem()
    : name_(""), character_(""), health_(0), fullHealth_(0), coins_(0), level(0) {}

// ��������
EventSystem::~EventSystem() {}

// ��ʼ��ͷ��
bool EventSystem::init() {

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
        //Relic->onHealthChange(tempHealthChange);
    }

    // ������ֵ����0ʱ����������Ч��
    if (health_ + tempHealthChange < 0)
    {
        for (auto Relic : EventSystem::getInstance()->relics_)
        {
            //Relic->onDeath();
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


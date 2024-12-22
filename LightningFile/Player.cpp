#include "IncludeAll.h"
#include "Enum.h"

std::shared_ptr<Player> Player::instance_ = nullptr;  // ���徲̬��Ա����

std::shared_ptr<Player> Player::getInstance() {
    if (!instance_) {
        instance_ = std::make_shared<Player>();
    }
    return instance_;
}

void Player::startTurn()
{
    block_ = 0;
}

void Player::endTurn() {
    currentEnergy_ = 0;
}

void Player::energyChange(int changeValue)
{
    currentEnergy_ += changeValue;
}

void Player::addPotion(Potion* potion)
{
}

/*
* ��ɫ��ʼ����Ҫ����ʼ��Ѫ���񵲣���buff�б���գ�
* ���������б����buff
*/
void Player::init()
{
    // ��ʼ����ǰѪ������ǰ����ֵ����ǰ�񵲣���ǰ����ֵ���������ֵ
    health_ = EventSystem::getInstance()->health_;
    max_health_ = EventSystem::getInstance()->fullHealth_;
    // health_ = 50;
    // max_health_ = 100;
    block_ = 0;
    currentEnergy_ = 0;
    maxEnergy_ = 4;
    
}


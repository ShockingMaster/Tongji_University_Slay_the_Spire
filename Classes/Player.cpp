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
    //��ʼ��Ѫ��
    health_ = 10;
    block_ = 10;
    currentEnergy_ = 0;
    maxEnergy_ = 4;
}


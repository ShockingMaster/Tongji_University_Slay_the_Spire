#include "IncludeAll.h"
#include "Enum.h"

std::shared_ptr<Player> Player::instance_ = nullptr;  // ���徲̬��Ա����

std::shared_ptr<Player> Player::getInstance() {
    if (!instance_) {
        instance_ = std::make_shared<Player>();
    }
    return instance_;
}

void Player::startTurn() {
    block_ = 0;  // Reset block at the start of the turn
}

void Player::endTurn() {
    // Discard cards, apply end-turn Buffs, etc.
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
}

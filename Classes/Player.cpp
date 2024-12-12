#include "IncludeAll.h"
#include "Enum.h"

std::shared_ptr<Player> Player::instance_ = nullptr;  // 定义静态成员变量

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
* 角色初始化需要：初始化血量格挡，将buff列表清空，
* 遍历遗物列表，添加buff
*/
void Player::init()
{
    //初始化血量
    health_ = 10;
    block_ = 10;
    currentEnergy_ = 0;
    maxEnergy_ = 4;
}


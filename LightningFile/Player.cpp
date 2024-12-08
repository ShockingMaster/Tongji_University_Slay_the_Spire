#include "IncludeAll.h"
#include "Enum.h"

std::shared_ptr<Player> Player::instance_ = nullptr;  // 定义静态成员变量

std::shared_ptr<Player> Player::getInstance() {
    if (!instance_) {
        instance_ = std::make_shared<Player>();
    }
    return instance_;
}

void Player::start_turn() {
    block_ = 0;  // Reset block at the start of the turn
}

void Player::end_turn() {
    // Discard cards, apply end-turn Buffs, etc.
}

void Player::addPotion(Potion* potion)
{
}

void Player::init()
{
}

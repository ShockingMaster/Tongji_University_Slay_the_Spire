#include "IncludeAll.h"
#include "Enum.h"

Player* Player::instance_ = nullptr;

// ���� getInstance ����  
Player* Player::getInstance() {
    if (instance_ == nullptr) {
        instance_ = new Player(); // ����Ψһʵ��  
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

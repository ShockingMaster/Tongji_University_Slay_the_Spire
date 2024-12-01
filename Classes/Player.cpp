#include "IncludeAll.h"
#include "Enum.h"

Player* Player::instance_ = nullptr;

// 定义 getInstance 方法  
Player* Player::getInstance() {
    if (instance_ == nullptr) {
        instance_ = new Player(); // 创建唯一实例  
    }
    return instance_;
}

void Player::start_turn() {
    block_ = 0; 
}

void Player::end_turn() {
    
}

void Player::addPotion(Potion* potion)
{
}

void Player::init()
{
}

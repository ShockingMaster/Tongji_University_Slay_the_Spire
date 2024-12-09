#include "IncludeAll.h"
#include "Enum.h"

Player* Player::instance_ = nullptr;

Player* Player::getInstance() {
    if (instance_ == nullptr) {
        instance_ = new Player(); 
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
    energy_ = 0;
    maxEnergy_ = 3;
}

void Player::addEnergy(int numeric_value_)
{
}

int Player::getEnergyValue()
{
    return energy_;
}
int Player::getMaxEnergyValue()
{
    return maxEnergy_;
}
#include "IncludeAll.h"
#include "Enum.h"
std::string Creature::getName() const
{
	return name_;
}
void Creature::addBlock(int numeric_value)
{
	block_ += numeric_value;
}

void Creature::loseBlock(int numeric_value)
{
	block_ -= numeric_value;
	block_ = max(0, block_);
}

void Creature::loseHealth(int numeric_value)
{
	health_ -= numeric_value;
}

void Creature::addHealth(int numeric_value)
{
	health_ += numeric_value;
	// 防止超过最大生命值
	health_ = min(health_, max_health_);
}

int Creature::getBlockValue() const
{
	return block_;
}

int Creature::getHealth() const
{
	return health_;
}

int Creature::getMaxHealth() const
{
	return max_health_;
}

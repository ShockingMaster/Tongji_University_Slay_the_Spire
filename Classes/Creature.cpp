#include "IncludeAll.h"
#include "Enum.h"
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

#include "IncludeAll.h"

CombatSystem* CombatSystem::instance_ = nullptr;

//返回CombatSytem的唯一实例
CombatSystem* CombatSystem::getInstance()
{
	if (instance_ == nullptr) 
	{
		instance_ = new CombatSystem(); // 创建唯一实例  
	}
	return instance_;
}

/*
 * 函数名称：onAttack
 * 参数：攻击者指针，被攻击者指针，攻击原数值，卡牌名称（为了判断卡牌是否为特殊类型）
 * 功能：完成攻击者的攻击相关的buff结算，触发攻击类buff并改写攻击数值
 */
void CombatSystem::onAttack(std::shared_ptr<Creature> user, std::shared_ptr<Creature> target, int numeric_value_, std::string cardName)
{
	//首先遍历使用者的buff列表，触发所有buff的onAttack效果
	for (auto Buff:user->buffs_)
	{
		if (Buff != nullptr)
			Buff->onAttack(numeric_value_, cardName, user, target);
	}

	//再遍历目标的buff列表，触发所有buff的onAttacked效果
	for (auto Buff : target->buffs_)
	{
		if (Buff != nullptr)
			Buff->onAttacked(numeric_value_, user, target);
	}

	//防止被减至负数
	numeric_value_ = max(numeric_value_, 0);

	takeDamage(target, numeric_value_);
}

/*
 * 函数名称：takeDamage
 * 参数：被攻击者指针，被攻击的数值，攻击者的指针
 * 功能：触发被攻击类buff并改写被攻击数值
 */
void CombatSystem::takeDamage(std::shared_ptr<Creature> target, int numeric_value_, std::shared_ptr<Creature> attacker)
{
	if (target == nullptr)
	{
		CCLOG("TakeDamage传入目标为空！");
		return;
	}
	// 共有三种情况：
	// 1.目标的格挡大于等于伤害总量，此时不能击穿敌方护甲,此时事件为护甲减少
	if (numeric_value_ <= target->getBlockValue())
	{
		for (auto Buff : target->buffs_)
		{
			if (Buff != nullptr)
				Buff->onLoseBlock(numeric_value_);
		}
		target->loseBlock(numeric_value_);
	}

	// 2.目标的格挡不为0，但小于伤害值，此时击穿护甲，造成伤害，此时事件为护甲减少，生命值减少
	else if (target->getBlockValue() > 0)
	{
		for (auto Buff : target->buffs_)
		{
			if (Buff != nullptr)
				Buff->onLoseBlock(numeric_value_);
		}
		target->loseBlock(target->getBlockValue());
		int healthLoss = numeric_value_ - target->getBlockValue();
		for (auto Buff : target->buffs_)
		{
			if (Buff != nullptr)
				Buff->onLoseHealth(healthLoss);
		}
		target->loseHealth(healthLoss);
	}

	// 3.目标的格挡为0，此时事件为生命值减少
	else
	{
		int healthLoss = numeric_value_ - target->getBlockValue();
		for (auto Buff : target->buffs_)
		{
			if (Buff != nullptr)
				Buff->onLoseHealth(healthLoss);
		}
		target->loseHealth(healthLoss);
	}
}

/*
 * 函数名称：startTurn
 * 参数：正在进行回合开始的生物的指针
 * 功能：完成该生物的回合开始类buff的结算
 */
void CombatSystem::startTurn(std::shared_ptr<Creature> creature)
{
	for (auto Buff : creature->buffs_)
	{
		if (Buff != nullptr)
		{
			Buff->onTurnStart();
		}
	}
}

/*
 * 函数名称：endTurn
 * 参数：正在进行回合结束的生物的指针
 * 功能：完成该生物的回合结束类buff的结算
 */
void CombatSystem::endTurn(std::shared_ptr<Creature> creature)
{
	for (auto Buff : creature->buffs_)
	{
		if (Buff != nullptr)
		{
			Buff->onTurnEnd();
		}
	}
}

/*
 * 函数名称：cardPlayed
 * 参数：打出的卡牌的指针
 * 功能：完成打出卡牌相关buff的结算
 */
void CombatSystem::cardPlayed(std::shared_ptr<Card> card)
{
	for (auto Buff : Player::getInstance()->buffs_)
	{
		if (Buff != nullptr)
		{
			Buff->onCardPlayed(card);
		}
	}
}

/*
 * 函数名称：exhaustCard
 * 参数：被消耗的卡牌的指针
 * 功能：完成打出卡牌相关buff的结算
 */
void CombatSystem::exhaustCard(std::shared_ptr<Card> card)
{
	card->takeEffectOnExhaust();
	for (auto Buff : Player::getInstance()->buffs_)
	{
		if (Buff != nullptr)
		{
			Buff->onExhaustCard();
		}
	}
}

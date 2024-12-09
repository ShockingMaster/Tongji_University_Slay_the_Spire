#include "IncludeAll.h"
#include <algorithm>
#include <random>
#include <chrono>

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
* 函数名称：init
* 功能：对战斗系统先进行清空，对角色进行初始化，将抽牌堆，弃牌堆，手牌中的卡牌全部进行清空
* 随机生成怪物，将卡组中所有的卡牌进行一次复制，放入抽牌堆，调用场景的初始化方法
*/
void CombatSystem::init()//需要传入怪物实例参数
{
	// 对角色进行初始化
	//Player::getInstance()->init();
	//获得角色实例
	std::shared_ptr<Player> player = Player::getInstance();
	//获得怪物实例

	//获取角色手牌
	std::vector<std::shared_ptr<Card>> cardPool = player->hand_card;
	// 打乱卡牌池，确保卡牌顺序随机
	auto rng = std::default_random_engine(std::chrono::system_clock::now().time_since_epoch().count());
	std::shuffle(cardPool.begin(), cardPool.end(), rng);


	// 清空抽牌堆、弃牌堆和手牌
	std::queue<std::shared_ptr<Card>> emptyQueue;
	std::vector<std::shared_ptr<Card>> emptyVector;
	drawPile = emptyQueue;
	hand = emptyVector;
	discardPile = emptyQueue;

	// 将前 `handSize` 张卡牌分配到手牌
	int handSize = 5;  // 假设手牌数量为 5
	for (int i = 0; i < handSize && i < cardPool.size(); ++i) {
		hand.push_back(cardPool[i]);  // 将卡牌加入手牌
	}

	// 将剩余的卡牌分配到抽牌堆
	for (size_t i = handSize; i < cardPool.size(); ++i) {
		drawPile.push(cardPool[i]);  // 将卡牌加入抽牌堆
	}

	
	/*/ 这里只是一个测试
	std::vector<std::string>  tempDeck = CardRegistry::getAllCardNames();
	if (tempDeck.empty())
	{
		CCLOG("tempDeck is empty!");
	}
	for (int i = 0;i < 9;i++)
	{
		for (auto name : tempDeck)
		{
			CCLOG("%s has been added!", name.c_str());
			drawPile.push(CardRegistry::createCard(name));
		}
	}
	*/

	// 清空怪物列表,随机生成怪物
	Monsters_.clear();  
	int numMonsters = 1; 
	for (int i = 0; i < numMonsters; i++)
	{
		// 生成怪物并加入到怪物列表
	}

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
 * 功能：完成该生物的回合开始类buff的结算，并进行回合开始操作
 */
void CombatSystem::startTurn(std::shared_ptr<Creature> creature)
{
	// 触发
	for (auto Buff : creature->buffs_)
	{
		if (Buff != nullptr)
		{
			Buff->onTurnStart();
		}
	}
	// 判断是否为玩家，如果是玩家，那么需要进行操作
	// 1.失去所有格挡
	// 2.获得能量上限的能量
	// 3.抽5张牌
	// 
	if (creature == Player::getInstance())
	{
		int energy = 4;//让我们假设玩家的能量上限目前为4
		addEnergy(Player::getInstance(),energy);
		drawCard(5);
	}
}

/*
 * 函数名称：endTurn
 * 参数：正在进行回合结束的生物的指针
 * 功能：完成该生物的回合结束类buff的结算
 */
void CombatSystem::endTurn(std::shared_ptr<Creature> creature)
{
	// 判断是否为玩家，如果是玩家，记录当前能量并归零
	if (creature == Player::getInstance())
	{
		tem_energy = Player::getInstance()->currentEnergy_;  //记录当前玩家能量
		Player::getInstance()->energyChange(0); //玩家能量归零
		// 调用前端能量变化方法,对能量进行更新
		auto currentScene = Director::getInstance()->getRunningScene();
		if (currentScene && dynamic_cast<CombatScene*>(currentScene)) { //检查是否为战斗场景
			CombatScene* combatScene = static_cast<CombatScene*>(currentScene);
			combatScene->updateEnergyDisplay();
		}
	}
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
	//首先触发卡牌被消耗的效果
	card->takeEffectOnExhaust();
	for (auto Buff : Player::getInstance()->buffs_)
	{
		if (Buff != nullptr)
		{
			Buff->onExhaustCard();
		}
	}
}

void CombatSystem::addEnergy(std::shared_ptr<Creature> user, int numeric_value_)
{
	int tempEnergy = numeric_value_;
	for (auto Buff : user->buffs_)
	{
		if (Buff != nullptr)
		{
			Buff->onGetEnergy(tempEnergy);
		}
	}
	if (user == Player::getInstance())
	{
		Player::getInstance()->energyChange(tempEnergy);
	}
	// 调用前端能量变化方法,对能量进行更新
	auto currentScene = Director::getInstance()->getRunningScene();
	if (currentScene && dynamic_cast<CombatScene*>(currentScene)) { //检查是否为战斗场景
		CombatScene* combatScene = static_cast<CombatScene*>(currentScene);
		combatScene->updateEnergyDisplay();
	}
}


/*
* 函数名称：shuffleDeck
* 参数：
* 功能：抽牌区为空时候调用，进行洗牌
*/
void CombatSystem::shuffleDeck() {
	// 临时容器：存储弃牌堆的卡牌
	std::vector<std::shared_ptr<Card>> discardCards;
	// 将弃牌堆中的卡牌取出并存储到临时容器中
	while (!discardPile.empty())
	{
		discardCards.push_back(discardPile.front());
		discardPile.pop();
	}
	// 打乱临时容器中的卡牌顺序
	auto rng = std::default_random_engine(std::chrono::system_clock::now().time_since_epoch().count());
	std::shuffle(discardCards.begin(), discardCards.end(), rng);

	// 将打乱后的卡牌放入抽牌堆
	for (const auto& card : discardCards)
	{
		drawPile.push(card);
	}

	CCLOG("Moved all cards from discard pile to draw pile and shuffled. Draw pile now has %d cards", drawPile.size());
}



/*
* 函数名称：drawCard
* 参数：抽卡的数量
* 功能：抽一定数量的牌并完成抽卡相关的buff结算
*/
void CombatSystem::drawCard(int num)
{
	int tempNum = num;
	for (auto Buff : Player::getInstance()->buffs_)
	{
		if (Buff != nullptr)
		{
			Buff->onDrawCard(tempNum);
		}
	}
	for (int i = 0;i < tempNum;i++)
	{
		for (auto Buff : Player::getInstance()->buffs_)
		{
			if (Buff != nullptr)
			{
				tempNum = 1;
				Buff->onDrawCard(tempNum);
			}
		}
		//当抽牌堆为空时进行洗牌
		if (drawPile.empty())
		{
			CCLOG("Draw pile is empty! Shuffling...");
			shuffleDeck();//调用洗牌函数
			CCLOG("No cards in my draw pile!");
		}

		//如果重新进行洗牌之后仍然为空则不进行抽牌
		if (!drawPile.empty())
		{
			// 抽取卡牌
			auto card = drawPile.front();
			drawPile.pop();
			hand.push_back(card);
			CCLOG("the hand now has %d cards", hand.size());
			
			// 调用前端抽牌堆的抽牌效果,修改
			HandPileLayer::getInstance()->drawCard(card);
		}
	}
	HandPileLayer::getInstance()->adjustHandPile();
}

int CombatSystem::getDrawPileNumber()
{
	return drawPile.size();
}

int CombatSystem::getHandNumber()
{
	return hand.size();
}

int CombatSystem::getDiscardPileNumber()
{
	return discardPile.size();
}

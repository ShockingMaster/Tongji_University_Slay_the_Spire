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
* 函数名称：init
* 功能：对战斗系统先进行清空，对角色进行初始化，将抽牌堆，弃牌堆，手牌中的卡牌全部进行清空
* 随机生成怪物，将卡组中所有的卡牌进行一次复制，放入抽牌堆，调用场景的初始化方法
*/
void CombatSystem::init()
{
	// 对角色进行初始化
	Player::getInstance()->init();
	
	// 清空抽牌堆、弃牌堆和手牌
	std::queue<std::shared_ptr<Card>> emptyQueue;
	std::vector<std::shared_ptr<Card>> emptyVector;
	drawPile = emptyQueue;
	hand = emptyVector;
	discardPile = emptyQueue;

	// 将卡组中的牌加入到抽牌堆中
	// 这里只是一个测试
	std::vector<std::string>  tempDeck = CardRegistry::getAllCardNames();
	if (tempDeck.empty())
	{
		CCLOG("tempDeck is empty!");
	}
	for (int i = 0;i < 4;i++)
	{
		for (auto name : tempDeck)
		{
			CCLOG("%s has been added!", name.c_str());
			drawPile.push(CardRegistry::createCard(name));
		}
	}

	// 清空怪物列表,随机生成怪物
	Monsters_.clear();
	int numMonsters = 2;
	for (int i = 0; i < numMonsters; i++)
	{
		Monsters_.push_back(RandomGenerator::getInstance()->getRandomMonster());
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
	for (auto Buff : user->buffs_)
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

	// 如果是玩家发起攻击，则触发遗物的onAttack效果
	if (user == Player::getInstance())
	{
		for (auto Relic : EventSystem::getInstance()->relics_)
		{
			if (Relic != nullptr)
			{
				Relic->onAttack(numeric_value_, cardName, user, target);
			}
		}
	}

	if (target == Player::getInstance())
	{
		for (auto Relic : EventSystem::getInstance()->relics_)
		{
			if (Relic != nullptr)
			{
				Relic->onAttacked(numeric_value_, user, target);
			}
		}
	}

	//防止被减至负数
	numeric_value_ = max(numeric_value_, 0);

	takeDamage(target, numeric_value_);

	auto scene = (CombatScene*)(Director::getInstance()->getRunningScene());
	scene->creatureLayer->updateDisplay();
}

/*
 * 函数名称：takeDamage
 * 参数：被攻击者指针，被攻击的数值，攻击者的指针
 * 功能：触发被攻击类buff并改写被攻击数值
 */
void CombatSystem::takeDamage(std::shared_ptr<Creature> target, int numeric_value_, std::shared_ptr<Creature> attack)
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
		if (target == Player::getInstance())
		{
			for (auto Relic : EventSystem::getInstance()->relics_)
			{
				if (Relic != nullptr)
					Relic->onLoseBlock(numeric_value_);
			}
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
		if (target == Player::getInstance())
		{
			for (auto Relic : EventSystem::getInstance()->relics_)
			{
				if (Relic != nullptr)
					Relic->onLoseBlock(numeric_value_);
			}
		}
		int healthLoss = numeric_value_ - target->getBlockValue();
		target->loseBlock(target->getBlockValue());
		for (auto Buff : target->buffs_)
		{
			if (Buff != nullptr)
				Buff->onLoseHealth(healthLoss);
		}
		if (target == Player::getInstance())
		{
			for (auto Relic : EventSystem::getInstance()->relics_)
			{
				if (Relic != nullptr)
					Relic-> onLoseHealth(healthLoss);
			}
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
		if (target == Player::getInstance())
		{
			for (auto Relic : EventSystem::getInstance()->relics_)
			{
				if (Relic != nullptr)
					Relic->onLoseHealth(healthLoss);
			}
		}
		target->loseHealth(healthLoss);
	}
	auto scene = (CombatScene*)(Director::getInstance()->getRunningScene());
	scene->creatureLayer->updateDisplay();
}

void CombatSystem::takeDamage(std::shared_ptr<Creature> target, int numeric_value_)
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
		if (target == Player::getInstance())
		{
			for (auto Relic : EventSystem::getInstance()->relics_)
			{
				if (Relic != nullptr)
					Relic->onLoseBlock(numeric_value_);
			}
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
		if (target == Player::getInstance())
		{
			for (auto Relic : EventSystem::getInstance()->relics_)
			{
				if (Relic != nullptr)
					Relic->onLoseBlock(numeric_value_);
			}
		}
		int healthLoss = numeric_value_ - target->getBlockValue();
		target->loseBlock(target->getBlockValue());
		for (auto Buff : target->buffs_)
		{
			if (Buff != nullptr)
				Buff->onLoseHealth(healthLoss);
		}
		if (target == Player::getInstance())
		{
			for (auto Relic : EventSystem::getInstance()->relics_)
			{
				if (Relic != nullptr)
					Relic->onLoseHealth(healthLoss);
			}
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
		if (target == Player::getInstance())
		{
			for (auto Relic : EventSystem::getInstance()->relics_)
			{
				if (Relic != nullptr)
					Relic->onLoseHealth(healthLoss);
			}
		}
		target->loseHealth(healthLoss);
	}
	auto scene = (CombatScene*)(Director::getInstance()->getRunningScene());
	scene->creatureLayer->updateDisplay();
}

/*
 * 函数名称：Addblock
 * 参数：加护盾对象，护盾数值，卡牌名称
 * 功能：触发护盾类buff并改写护盾值
 */

void CombatSystem::Addblock(std::shared_ptr<Creature> target, int numeric_value_) {
	//遍历使用者的buff列表，触发所有buff的Addblock效果
	int tempBlock = numeric_value_;
	for (auto Buff : target->buffs_)
	{
		if (Buff != nullptr)
			Buff->onGetBlock(tempBlock);
	}
	if (target == Player::getInstance())
	{
		for (auto Relic : EventSystem::getInstance()->relics_)
		{
			if (Relic != nullptr)
				Relic->onGetBlock(tempBlock);
		}
	}
	//防止被减至负数
	numeric_value_ = max(numeric_value_, 0);
	target->addBlock(numeric_value_);  //增加护盾

	// 进行更新
	auto scene = (CombatScene*)(Director::getInstance()->getRunningScene());
	scene->creatureLayer->updateDisplay();
}


/*
 * 函数名称：exhuastCard
 * 参数：消耗第几张卡牌，卡牌名称
 * 功能：消耗特定卡牌
 */

void CombatSystem::exhaustCard(int num, std::string cardName) 
{
	if (num < 0 || num >= hand.size()) {
		CCLOG("Error: Invalid card index %d", num);
		return;  // 如果 num 无效，直接返回
	}

	//遍历使用者的buff列表，触发所有buff的deleteCard效果
	for (auto Buff : Player::getInstance()->buffs_)
	{
		if (Buff != nullptr)
		{
			Buff->onExhaustCard();
		}
	}
	for (auto Relic : EventSystem::getInstance()->relics_)
	{
		if (Relic != nullptr)
		{
			Relic->onExhaustCard();
		}
	}

	// 触发卡牌被消耗的效果
	auto card = hand[num];
	card->takeEffectOnExhaust();

	// 调用前端效果对卡牌进行移除
	HandPileLayer::getInstance()->removeCard(card);

	// 消耗相应位置卡牌
	hand.erase(hand.begin() + num);
	CCLOG("Card '%s' at index %d has been exhuasted", cardName.c_str(), num);

	auto scene = (CombatScene*)(Director::getInstance()->getRunningScene());
	scene->creatureLayer->updateDisplay();
}


void CombatSystem::exhaustCard()
{

	for (auto& card : HandPileLayer::getInstance()->select_card_list) {
		for (auto Buff : Player::getInstance()->buffs_)
		{
			if (Buff != nullptr)
			{
				Buff->onExhaustCard();
			}
		}
		for (auto Relic : EventSystem::getInstance()->relics_)
		{
			if (Relic != nullptr)
			{
				Relic->onExhaustCard();
			}
		}

		card->takeEffectOnExhaust();

		HandPileLayer::getInstance()->removeCard(card);

	}

}


void CombatSystem::exhaustCard(std::shared_ptr<Card> card)
{
	for (auto Buff : Player::getInstance()->buffs_)
	{
		if (Buff != nullptr)
		{
			Buff->onExhaustCard();
		}
	}
	for (auto Relic : EventSystem::getInstance()->relics_)
	{
		if (Relic != nullptr)
		{
			Relic->onExhaustCard();
		}
	}
	card->takeEffectOnExhaust();

	HandPileLayer::getInstance()->removeCard(card);


	// 消耗相应位置
    hand.erase(std::remove(hand.begin(), hand.end(), card), hand.end());
}


void CombatSystem::endturn_cardPlayed() {
	std::vector<std::shared_ptr<Card>> Hand = hand;
	for (auto& card : Hand) {
		card->takeeffectonturnend(card);
	}
}

/*
* 函数名称：upgradeCard
* 参数：需要升级的卡牌的指针
* 功能：将对应卡牌进行升级，同时如果卡牌在手牌中，更新显示
*/
void CombatSystem::upgradeCard(std::shared_ptr<Card> card)
{
	// 如果卡牌没有进行过升级，那么对卡牌进行升级
	if (!card->isUpgraded())
	{
		card->upgrade();

		// 查看卡牌是否在手牌中，如果在手牌中需要更新显示
		for (int i = 0; i < hand.size();i++)
		{
			if (hand[i] == card)
			{
				auto cardSprite = HandPileLayer::getInstance()->getChildByTag(reinterpret_cast<intptr_t>(hand[i].get()));
				CardSpriteGenerator::updateCardSprite(card, cardSprite);
			}
		}
	}

	auto scene = (CombatScene*)(Director::getInstance()->getRunningScene());
	scene->creatureLayer->updateDisplay();
}


/*
 * 函数名称：startTurn
 * 参数：正在进行回合开始的生物的指针
 * 功能：完成该生物的回合开始类buff的结算，并进行回合开始操作
 */
void CombatSystem::startTurn(std::shared_ptr<Creature> creature)
{
	// 失去所有格挡
	creature->loseBlock(creature->getBlockValue());
	auto scene = (CombatScene*)(Director::getInstance()->getRunningScene());
	scene->creatureLayer->updateDisplay();

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
	if (creature == Player::getInstance())
	{
		for (auto Relic : EventSystem::getInstance()->relics_)
		{
			if (Relic != nullptr)
			{
				Relic->onTurnStart();
			}
		}
		const int energy = Player::getInstance()->getMaxEnergy();//获取玩家的最大能量上限
		addEnergy(Player::getInstance(), energy);
		drawCard(5);
	}

	scene->creatureLayer->updateDisplay();
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
	if (creature == Player::getInstance())
	{
		for (auto Relic : EventSystem::getInstance()->relics_)
		{
			if (Relic != nullptr)
			{
				Relic->onTurnEnd();
			}
		}
		const int energy = Player::getInstance()->getCurrentEnergy();//将能量清空
		addEnergy(Player::getInstance(), -energy);
		for (size_t i = 0; i < hand.size();)
		{
			std::shared_ptr<Card> card = hand[i];
			CombatSystem::getInstance()->discardPile.push(card);  // 将卡牌加入弃牌堆
			auto cardSprite = HandPileLayer::getInstance()->getChildByTag(reinterpret_cast<intptr_t>(hand[i].get()));
			HandPileLayer::getInstance()->removeChild(cardSprite);
			// 从手牌中删除卡牌
			hand.erase(hand.begin() + i);  // 删除当前元素，i 保持不变，因为元素被删除后，后面的元素会前移
		}
		// 调用前端能量变化方法,对能量进行更新
		HandPileLayer::getInstance()->updateDiscardPileDisplay();
	}

	auto scene = (CombatScene*)(Director::getInstance()->getRunningScene());
	scene->creatureLayer->updateDisplay();

}

/*
 * 函数名称：cardPlayed
 * 参数：打出的卡牌的指针
 * 功能：完成打出卡牌相关buff的结算
 */
void CombatSystem::cardPlayed(std::shared_ptr<Card> card)
{
	
	tem_card = card;
	for (auto Buff : Player::getInstance()->buffs_)
	{
		if (Buff != nullptr)
		{
			Buff->onCardPlayed(card);
		}
	}
	for (auto Relic : EventSystem::getInstance()->relics_)
	{
		if (Relic != nullptr)
		{
			Relic->onCardPlayed(card);
		}
	}

	card->takeEffect();

	int tempEnergyCost = card->getEnergyCost();
	Player::getInstance()->energyChange(-tempEnergyCost);
	// 调用前端能量变化方法,对能量进行更新
	auto currentScene = Director::getInstance()->getRunningScene();
	if (currentScene && dynamic_cast<CombatScene*>(currentScene)) { //检查是否为战斗场景
		CombatScene* combatScene = static_cast<CombatScene*>(currentScene);
		combatScene->updateEnergyDisplay();
	}

	auto scene = (CombatScene*)(Director::getInstance()->getRunningScene());
	scene->creatureLayer->updateDisplay();
}

void CombatSystem::tem_cardPlayed(std::shared_ptr<Card> card)
{
	for (auto Buff : Player::getInstance()->buffs_)
	{
		if (Buff != nullptr)
		{
			Buff->onCardPlayed(card);
		}
	}
	for (auto Relic : EventSystem::getInstance()->relics_)
	{
		if (Relic != nullptr)
		{
			Relic->onCardPlayed(card);
		}
	}
	card->takeEffect();
}

void CombatSystem::use_tem_card() {
	tem_card->tag = 1;
	tem_cardPlayed(tem_card);

}


void CombatSystem::cardPlayed(std::shared_ptr<Card> card, std::shared_ptr<Creature> creature)
{
	for (auto Buff : Player::getInstance()->buffs_)
	{
		if (Buff != nullptr)
		{
			Buff->onCardPlayed(card);
		}
	}
	for (auto Relic : EventSystem::getInstance()->relics_)
	{
		if (Relic != nullptr)
		{
			Relic->onCardPlayed(card);
		}
	}
	card->takeEffect(creature);

	int tempEnergyCost = card->getEnergyCost();
	Player::getInstance()->energyChange(-tempEnergyCost);
	// 调用前端能量变化方法,对能量进行更新
	auto currentScene = Director::getInstance()->getRunningScene();
	if (currentScene && dynamic_cast<CombatScene*>(currentScene)) { //检查是否为战斗场景
		CombatScene* combatScene = static_cast<CombatScene*>(currentScene);
		combatScene->updateEnergyDisplay();
	}

	auto scene = (CombatScene*)(Director::getInstance()->getRunningScene());
	scene->creatureLayer->updateDisplay();
}


/*
* 
* 
* 
*/
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
		for (auto Relic : EventSystem::getInstance()->relics_)
		{
			if (Relic != nullptr)
			{
				Relic->onGetEnergy(tempEnergy);
			}
		}
		Player::getInstance()->energyChange(tempEnergy);
	}
	// 调用前端能量变化方法,对能量进行更新
	auto currentScene = Director::getInstance()->getRunningScene();
	if (currentScene && dynamic_cast<CombatScene*>(currentScene)) { //检查是否为战斗场景
		CombatScene* combatScene = static_cast<CombatScene*>(currentScene);
		combatScene->updateEnergyDisplay();
	}

	auto scene = (CombatScene*)(Director::getInstance()->getRunningScene());
	scene->creatureLayer->updateDisplay();
}

/*
* 函数名称：addBuff
* 参数：增加的Buff种类，
*/
void CombatSystem::addBuff(std::shared_ptr<Buff> buff, int numeric_value)
{

	auto scene = (CombatScene*)(Director::getInstance()->getRunningScene());
	scene->creatureLayer->updateDisplay();
}

void CombatSystem::addHealth(std::shared_ptr<Creature> target, int numeric_value)
{
	int tempHealthRestore = numeric_value;
	for (auto Buff : target->buffs_)
	{
		if (Buff != nullptr)
		{
			Buff->onAddHealth(tempHealthRestore);
		}
	}
	if (target == Player::getInstance())
	{
		for (auto Relic : EventSystem::getInstance()->relics_)
		{
			if (Relic != nullptr)
			{
				Relic->onAddHealth(tempHealthRestore);
			}
		}
	}
	// 防止出现负数
	tempHealthRestore = max(0, tempHealthRestore);
	target->addHealth(tempHealthRestore);

	auto scene = (CombatScene*)(Director::getInstance()->getRunningScene());
	scene->creatureLayer->updateDisplay();
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
	for (auto Relic : EventSystem::getInstance()->relics_)
	{
		if (Relic != nullptr)
		{
			Relic->onDrawCard(tempNum);
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
			shuffleDeck();
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

			// 调用前端抽牌堆的抽牌效果
			HandPileLayer::getInstance()->drawCard(card);
		}
	}
	HandPileLayer::getInstance()->adjustHandPile();

	auto scene = (CombatScene*)(Director::getInstance()->getRunningScene());
	scene->creatureLayer->updateDisplay();
}

void CombatSystem::shuffleDeck() 
{
	// 临时容器：存储弃牌堆的卡牌
	std::vector<std::shared_ptr<Card>> discardCards;
	for (auto Buff : Player::getInstance()->buffs_)
	{
		if (Buff != nullptr)
		{
			Buff->onShuffleDeck();
		}
	}
	for (auto Relic : EventSystem::getInstance()->relics_)
	{
		if (Relic != nullptr)
		{
			Relic->onShuffleDeck();
		}
	}
	// 将弃牌堆中的卡牌取出并存储到临时容器中
	while (!discardPile.empty())
	{
		discardCards.push_back(discardPile.front());
		discardPile.pop();
	}
	// 打乱临时容器中的卡牌顺序
	RandomGenerator::getInstance()->shuffleVector(discardCards);

	// 将打乱后的卡牌放入抽牌堆
	for (const auto& card : discardCards)
	{
		drawPile.push(card);
	}

	HandPileLayer::getInstance()->updateDrawPileDisplay();
	HandPileLayer::getInstance()->updateDiscardPileDisplay();


	auto scene = (CombatScene*)(Director::getInstance()->getRunningScene());
	scene->creatureLayer->updateDisplay();
	CCLOG("Moved all cards from discard pile to draw pile and shuffled. Draw pile now has %d cards", drawPile.size());
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



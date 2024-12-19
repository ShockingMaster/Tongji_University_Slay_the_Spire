#include "IncludeAll.h"

CombatSystem* CombatSystem::instance_ = nullptr;

//����CombatSytem��Ψһʵ��
CombatSystem* CombatSystem::getInstance()
{
	if (instance_ == nullptr)
	{
		instance_ = new CombatSystem(); // ����Ψһʵ��  
	}
	return instance_;
}

/*
* �������ƣ�init
* ���ܣ���ս��ϵͳ�Ƚ�����գ��Խ�ɫ���г�ʼ���������ƶѣ����ƶѣ������еĿ���ȫ���������
* ������ɹ�������������еĿ��ƽ���һ�θ��ƣ�������ƶѣ����ó����ĳ�ʼ������
*/
void CombatSystem::init()
{
	// �Խ�ɫ���г�ʼ��
	Player::getInstance()->init();
	
	// ��ճ��ƶѡ����ƶѺ�����
	std::queue<std::shared_ptr<Card>> emptyQueue;
	std::vector<std::shared_ptr<Card>> emptyVector;
	drawPile = emptyQueue;
	hand = emptyVector;
	discardPile = emptyQueue;

	// �������е��Ƽ��뵽���ƶ���
	// ����ֻ��һ������
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

	// ��չ����б�,������ɹ���
	Monsters_.clear();
	int numMonsters = 2;
	for (int i = 0; i < numMonsters; i++)
	{
		Monsters_.push_back(RandomGenerator::getInstance()->getRandomMonster());
	}
}



/*
 * �������ƣ�onAttack
 * ������������ָ�룬��������ָ�룬����ԭ��ֵ���������ƣ�Ϊ���жϿ����Ƿ�Ϊ�������ͣ�
 * ���ܣ���ɹ����ߵĹ�����ص�buff���㣬����������buff����д������ֵ
 */
void CombatSystem::onAttack(std::shared_ptr<Creature> user, std::shared_ptr<Creature> target, int numeric_value_, std::string cardName)
{
	//���ȱ���ʹ���ߵ�buff�б���������buff��onAttackЧ��
	for (auto Buff : user->buffs_)
	{
		if (Buff != nullptr)
			Buff->onAttack(numeric_value_, cardName, user, target);
	}

	//�ٱ���Ŀ���buff�б���������buff��onAttackedЧ��
	for (auto Buff : target->buffs_)
	{
		if (Buff != nullptr)
			Buff->onAttacked(numeric_value_, user, target);
	}

	// �������ҷ��𹥻����򴥷������onAttackЧ��
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

	//��ֹ����������
	numeric_value_ = max(numeric_value_, 0);

	takeDamage(target, numeric_value_);

	auto scene = (CombatScene*)(Director::getInstance()->getRunningScene());
	scene->creatureLayer->updateDisplay();
}

/*
 * �������ƣ�takeDamage
 * ��������������ָ�룬����������ֵ�������ߵ�ָ��
 * ���ܣ�������������buff����д��������ֵ
 */
void CombatSystem::takeDamage(std::shared_ptr<Creature> target, int numeric_value_, std::shared_ptr<Creature> attack)
{
	if (target == nullptr)
	{
		CCLOG("TakeDamage����Ŀ��Ϊ�գ�");
		return;
	}
	// �������������
	// 1.Ŀ��ĸ񵲴��ڵ����˺���������ʱ���ܻ����з�����,��ʱ�¼�Ϊ���׼���
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

	// 2.Ŀ��ĸ񵲲�Ϊ0����С���˺�ֵ����ʱ�������ף�����˺�����ʱ�¼�Ϊ���׼��٣�����ֵ����
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

	// 3.Ŀ��ĸ�Ϊ0����ʱ�¼�Ϊ����ֵ����
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
		CCLOG("TakeDamage����Ŀ��Ϊ�գ�");
		return;
	}
	// �������������
	// 1.Ŀ��ĸ񵲴��ڵ����˺���������ʱ���ܻ����з�����,��ʱ�¼�Ϊ���׼���
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

	// 2.Ŀ��ĸ񵲲�Ϊ0����С���˺�ֵ����ʱ�������ף�����˺�����ʱ�¼�Ϊ���׼��٣�����ֵ����
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

	// 3.Ŀ��ĸ�Ϊ0����ʱ�¼�Ϊ����ֵ����
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
 * �������ƣ�Addblock
 * �������ӻ��ܶ��󣬻�����ֵ����������
 * ���ܣ�����������buff����д����ֵ
 */

void CombatSystem::Addblock(std::shared_ptr<Creature> target, int numeric_value_) {
	//����ʹ���ߵ�buff�б���������buff��AddblockЧ��
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
	//��ֹ����������
	numeric_value_ = max(numeric_value_, 0);
	target->addBlock(numeric_value_);  //���ӻ���

	// ���и���
	auto scene = (CombatScene*)(Director::getInstance()->getRunningScene());
	scene->creatureLayer->updateDisplay();
}


/*
 * �������ƣ�exhuastCard
 * ���������ĵڼ��ſ��ƣ���������
 * ���ܣ������ض�����
 */

void CombatSystem::exhaustCard(int num, std::string cardName) 
{
	if (num < 0 || num >= hand.size()) {
		CCLOG("Error: Invalid card index %d", num);
		return;  // ��� num ��Ч��ֱ�ӷ���
	}

	//����ʹ���ߵ�buff�б���������buff��deleteCardЧ��
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

	// �������Ʊ����ĵ�Ч��
	auto card = hand[num];
	card->takeEffectOnExhaust();

	// ����ǰ��Ч���Կ��ƽ����Ƴ�
	HandPileLayer::getInstance()->removeCard(card);

	// ������Ӧλ�ÿ���
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


	// ������Ӧλ��
    hand.erase(std::remove(hand.begin(), hand.end(), card), hand.end());
}


void CombatSystem::endturn_cardPlayed() {
	std::vector<std::shared_ptr<Card>> Hand = hand;
	for (auto& card : Hand) {
		card->takeeffectonturnend(card);
	}
}

/*
* �������ƣ�upgradeCard
* ��������Ҫ�����Ŀ��Ƶ�ָ��
* ���ܣ�����Ӧ���ƽ���������ͬʱ��������������У�������ʾ
*/
void CombatSystem::upgradeCard(std::shared_ptr<Card> card)
{
	// �������û�н��й���������ô�Կ��ƽ�������
	if (!card->isUpgraded())
	{
		card->upgrade();

		// �鿴�����Ƿ��������У��������������Ҫ������ʾ
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
 * �������ƣ�startTurn
 * ���������ڽ��лغϿ�ʼ�������ָ��
 * ���ܣ���ɸ�����ĻغϿ�ʼ��buff�Ľ��㣬�����лغϿ�ʼ����
 */
void CombatSystem::startTurn(std::shared_ptr<Creature> creature)
{
	// ʧȥ���и�
	creature->loseBlock(creature->getBlockValue());
	auto scene = (CombatScene*)(Director::getInstance()->getRunningScene());
	scene->creatureLayer->updateDisplay();

	// ����
	for (auto Buff : creature->buffs_)
	{
		if (Buff != nullptr)
		{
			Buff->onTurnStart();
		}
	}

	
	// �ж��Ƿ�Ϊ��ң��������ң���ô��Ҫ���в���
	// 1.ʧȥ���и�
	// 2.����������޵�����
	// 3.��5����
	if (creature == Player::getInstance())
	{
		for (auto Relic : EventSystem::getInstance()->relics_)
		{
			if (Relic != nullptr)
			{
				Relic->onTurnStart();
			}
		}
		const int energy = Player::getInstance()->getMaxEnergy();//��ȡ��ҵ������������
		addEnergy(Player::getInstance(), energy);
		drawCard(5);
	}

	scene->creatureLayer->updateDisplay();
}

/*
 * �������ƣ�endTurn
 * ���������ڽ��лغϽ����������ָ��
 * ���ܣ���ɸ�����ĻغϽ�����buff�Ľ���
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
		const int energy = Player::getInstance()->getCurrentEnergy();//���������
		addEnergy(Player::getInstance(), -energy);
		for (size_t i = 0; i < hand.size();)
		{
			std::shared_ptr<Card> card = hand[i];
			CombatSystem::getInstance()->discardPile.push(card);  // �����Ƽ������ƶ�
			auto cardSprite = HandPileLayer::getInstance()->getChildByTag(reinterpret_cast<intptr_t>(hand[i].get()));
			HandPileLayer::getInstance()->removeChild(cardSprite);
			// ��������ɾ������
			hand.erase(hand.begin() + i);  // ɾ����ǰԪ�أ�i ���ֲ��䣬��ΪԪ�ر�ɾ���󣬺����Ԫ�ػ�ǰ��
		}
		// ����ǰ�������仯����,���������и���
		HandPileLayer::getInstance()->updateDiscardPileDisplay();
	}

	auto scene = (CombatScene*)(Director::getInstance()->getRunningScene());
	scene->creatureLayer->updateDisplay();

}

/*
 * �������ƣ�cardPlayed
 * ����������Ŀ��Ƶ�ָ��
 * ���ܣ���ɴ���������buff�Ľ���
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
	// ����ǰ�������仯����,���������и���
	auto currentScene = Director::getInstance()->getRunningScene();
	if (currentScene && dynamic_cast<CombatScene*>(currentScene)) { //����Ƿ�Ϊս������
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
	// ����ǰ�������仯����,���������и���
	auto currentScene = Director::getInstance()->getRunningScene();
	if (currentScene && dynamic_cast<CombatScene*>(currentScene)) { //����Ƿ�Ϊս������
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
	// ����ǰ�������仯����,���������и���
	auto currentScene = Director::getInstance()->getRunningScene();
	if (currentScene && dynamic_cast<CombatScene*>(currentScene)) { //����Ƿ�Ϊս������
		CombatScene* combatScene = static_cast<CombatScene*>(currentScene);
		combatScene->updateEnergyDisplay();
	}

	auto scene = (CombatScene*)(Director::getInstance()->getRunningScene());
	scene->creatureLayer->updateDisplay();
}

/*
* �������ƣ�addBuff
* ���������ӵ�Buff���࣬
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
	// ��ֹ���ָ���
	tempHealthRestore = max(0, tempHealthRestore);
	target->addHealth(tempHealthRestore);

	auto scene = (CombatScene*)(Director::getInstance()->getRunningScene());
	scene->creatureLayer->updateDisplay();
}


/*
* �������ƣ�drawCard
* �������鿨������
* ���ܣ���һ���������Ʋ���ɳ鿨��ص�buff����
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
		//�����ƶ�Ϊ��ʱ����ϴ��
		if (drawPile.empty())
		{
			shuffleDeck();
			CCLOG("No cards in my draw pile!");
		}

		//������½���ϴ��֮����ȻΪ���򲻽��г���
		if (!drawPile.empty())
		{
			// ��ȡ����
			auto card = drawPile.front();
			drawPile.pop();
			hand.push_back(card);
			CCLOG("the hand now has %d cards", hand.size());

			// ����ǰ�˳��ƶѵĳ���Ч��
			HandPileLayer::getInstance()->drawCard(card);
		}
	}
	HandPileLayer::getInstance()->adjustHandPile();

	auto scene = (CombatScene*)(Director::getInstance()->getRunningScene());
	scene->creatureLayer->updateDisplay();
}

void CombatSystem::shuffleDeck() 
{
	// ��ʱ�������洢���ƶѵĿ���
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
	// �����ƶ��еĿ���ȡ�����洢����ʱ������
	while (!discardPile.empty())
	{
		discardCards.push_back(discardPile.front());
		discardPile.pop();
	}
	// ������ʱ�����еĿ���˳��
	RandomGenerator::getInstance()->shuffleVector(discardCards);

	// �����Һ�Ŀ��Ʒ�����ƶ�
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



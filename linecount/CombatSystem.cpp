#include "IncludeAll.h"
#include "RewardLayer.h"
#include "EndingScene.h"
CombatSystem* CombatSystem::instance_ = nullptr;

// ����CombatSytem��Ψһʵ��
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
void CombatSystem::init(int type)
{
	// �Խ�ɫ���г�ʼ��
	Player::getInstance()->init();

	// �����BOSS����ô������ս��
	if (type == BOSS)
	{
		isLastCombat = 1;
	}
	else
	{
		isLastCombat = 0;
	}

	// ��ճ��ƶѡ����ƶѺ�����
	std::queue<std::shared_ptr<Card>> emptyQueue;
	std::vector<std::shared_ptr<Card>> emptyVector;
	drawPile = emptyQueue;
	hand = emptyVector;
	discardPile = emptyQueue;

	// ���һ�ݿ���ĸ���
	std::vector<std::shared_ptr<Card>> tempVector;
	for (int i = 0; i < EventSystem::getInstance()->cards_.size(); i++)
	{
		tempVector.push_back(CardRegistry::createCard(EventSystem::getInstance()->cards_[i]->getName()));
	}
	RandomGenerator::getInstance()->shuffleVector(tempVector);

	// �������е��Ƽ��뵽���ƶ���
	for (int i = 0; i < tempVector.size(); i++)
	{
		drawPile.push(tempVector[i]);
	}
	/*
	// ���Կ���ע�ἰ����
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
	*/

	// ��չ����б�,������ɹ���
	Monsters_.clear();
	if (type == ELITE)
	{
		// ��ȡ��������
		auto monster = RandomGenerator::getInstance()->getRandomMonster(ELITE);
		for (int i = 0; i < monster->getMonsterNum(); i++)
		{
			Monsters_.push_back(MonsterRegistry::createMonster(monster->getName()));
		}
	}
	else if (type == BOSS)
	{
		auto monster = RandomGenerator::getInstance()->getRandomMonster(BOSS);
		for (int i = 0; i < monster->getMonsterNum(); i++)
		{
			Monsters_.push_back(MonsterRegistry::createMonster(monster->getName()));
		}
	}
	else
	{
		auto monster = RandomGenerator::getInstance()->getRandomMonster(NORMAL);
		for (int i = 0; i < monster->getMonsterNum(); i++)
		{
			Monsters_.push_back(MonsterRegistry::createMonster(monster->getName()));
		}
	}
}



/*
 * �������ƣ�onAttack
 * ������������ָ�룬��������ָ�룬����ԭ��ֵ���������ƣ�Ϊ���жϿ����Ƿ�Ϊ�������ͣ�
 * ���ܣ���ɹ����ߵĹ�����ص�buff���㣬����������buff����д������ֵ
 */
void CombatSystem::onAttack(std::shared_ptr<Creature> user, std::shared_ptr<Creature> target,
	int& numeric_value_, std::string cardName, bool isForIntentionUpdate)
{
	//���ȱ���ʹ���ߵ�buff�б�����������buff��onAttackЧ��
	for (auto Buff : user->buffs_)
	{
		if (Buff != nullptr)
			Buff->onAttack(numeric_value_, cardName, user, target);
	}

	//�ٱ���Ŀ���buff�б�����������buff��onAttackedЧ��
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
		audioPlayer("FastAttackSound.ogg", false);
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

	if (!isForIntentionUpdate)
	{
		takeDamage(target, numeric_value_);
		auto scene = (CombatScene*)(Director::getInstance()->getRunningScene());
		scene->creatureLayer->attackAction(user);
	}


	// �˴����ܽ��и��£������ѭ������
	/*
	auto scene = (CombatScene*)(Director::getInstance()->getRunningScene());
	scene->creatureLayer->updateDisplay();
	*/
}

void CombatSystem::combatStart()
{
	auto player = Player::getInstance();
	for (auto Buff : player->buffs_)
	{
		if (Buff != nullptr)
			Buff->onCombatStart(player);
	}
	for (auto Relic : EventSystem::getInstance()->relics_)
	{
		if (Relic != nullptr)
			Relic->onCombatStart();
	}
}

void CombatSystem::combatEnd()
{
	// �ȴ���һ�λغϽ���Ч��
	auto player = Player::getInstance();

	CombatSystem::getInstance()->endTurn(player);
	HandPileLayer::getInstance()->clearAll();

	if (player->getHealth() <= 0)
	{
		auto endScene = EndingScene::create(0);
		Director::getInstance()->pushScene(endScene);
	}
	else
	{
		for (auto Buff : player->buffs_)
		{
			if (Buff != nullptr)
				Buff->onCombatEnd(player);
		}
		for (auto Relic : EventSystem::getInstance()->relics_)
		{
			if (Relic != nullptr)
				Relic->onCombatEnd();
		}
	}
	// ���������ս��
	if (isLastCombat && player->getHealth()>0)
	{
		auto endScene = EndingScene::create(1);
		Director::getInstance()->pushScene(endScene);
	}
	else
	{
		auto blackLayer = LayerColor::create(Color4B(0, 0, 0, 200));
		Director::getInstance()->getRunningScene()->addChild(blackLayer, 100000);

		// ���� RewardLayer
		auto rewardLayer = RewardLayer::create(true, true, false, false, true);
		blackLayer->addChild(rewardLayer); // �� RewardLayer ���ӵ���ɫ��������
		auto startButton = HoverButton::create(
			"button1 (1).png",  // Ĭ��ͼƬ
			"button1 (2).png",  // ��ť��ͣʱ��ͼƬ
			"button1 (3).png"   // ��ť���ʱ��ͼƬ
		);

		// ���ð�ťλ��
		startButton->setPosition(Vec2(1800, 500));
		blackLayer->addChild(startButton);

		// ���Ӱ�ť����¼�������
		startButton->addClickEventListener([=](Ref* sender) {
			// ִ�� popScene ������������һ������
			Director::getInstance()->popScene();
			});
	}
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
		audioPlayer("AttackOnBlockSound.ogg", false);
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
		audioPlayer("DefenseBreakSound.ogg", false);
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
		audioPlayer("FastAttackSound.ogg", false);
		target->loseHealth(healthLoss);
	}
	if (target->getHealth() < 0)
	{
		CombatSystem::getInstance()->onDeath(target);
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
	//����ʹ���ߵ�buff�б�����������buff��AddblockЧ��
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

	if (numeric_value_ > 0)
	{
		audioPlayer("GetBlockSound.ogg", false);
	}
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

	//����ʹ���ߵ�buff�б�����������buff��deleteCardЧ��
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


void CombatSystem::endTurnCardPlayed() {
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
		for (int i = 0; i < hand.size(); i++)
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

//����buff
void CombatSystem::updateBuff(std::shared_ptr<Creature> creature) {
	for (auto it = creature->buffs_.begin(); it != creature->buffs_.end(); ) {
		auto Buff = *it;
		if (Buff->stack_type_ == DURATION) {
			Buff->duration_--;
			if (Buff->duration_ == 0) {
				it = creature->buffs_.erase(it); // �Ƴ�Ԫ�ز����µ�����
				continue; // �����������ĵ�������
			}
		}
		++it; // ����δ�Ƴ�ʱ����������
	}
	for (auto Buff : creature->buffs_)
	{
		if (Buff->stack_type_ != EFFECT_LAYERS) {
			CCLOG("update::have buff: %s. num is %d", typeid(*Buff).name(), Buff->duration_);
		}
		else {
			CCLOG("update::have buff: %s. num is %d", typeid(*Buff).name(), Buff->effect_layers);
		}
		
	}
	//��ʾ����
	auto scene = (CombatScene*)(Director::getInstance()->getRunningScene());
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
	updateBuff(creature);
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

/*
* �������ƣ�addToDiscardPile
* ���ܣ���һ�������Ŀ����������ƶ�
*/
void CombatSystem::addToDiscardPile(std::shared_ptr<Card> card, int num)
{
	for (int i = 0; i < num; i++)
	{
		discardPile.push(card);
	}
	// �����ʾ����
	HandPileLayer::getInstance()->updateDiscardPileDisplay();
}

void CombatSystem::onDeath(std::shared_ptr<Creature> creature)
{
	if (creature == Player::getInstance())
	{
		for (auto Relic : EventSystem::getInstance()->relics_)
		{
			Relic->onDeath();
		}
		if (Player::getInstance()->getHealth() < 0)
		{
			this->combatEnd();
		}
	}
	else
	{
		int is_all_monster_dead = 1;
		for (int i = 0; i < Monsters_.size(); i++)
		{
			if (Monsters_[i]->getHealth() > 0)
				is_all_monster_dead = 0;
		}
		if (is_all_monster_dead)
		{
			CombatSystem::combatEnd();
		}
	}
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

/*
* �������ƣ�getMonsterPointer
* ������������ָ��
* ���������ظ�ָ����CombatSystem�е�����ָ��
*/
std::shared_ptr<Creature> CombatSystem::getMonsterPointer(Creature* creature)
{
	if (creature == nullptr)
	{
		return nullptr;
	}
	for (int i = 0; i < Monsters_.size(); i++)
	{
		if (Monsters_[i].get() == creature)
		{
			return Monsters_[i];
		}
	}
	return nullptr;
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
void CombatSystem::addBuff(std::shared_ptr<Buff> buff, int numeric_value, std::shared_ptr<Creature> target)
{
	//����Ŀ���buff�б�����������buff��addBuffЧ��
	for (auto it = target->buffs_.begin(); it != target->buffs_.end(); ) {
		auto Buff = *it;
		Buff->addBuff(buff, numeric_value);
		if (Buff->effect_layers == 0&& Buff->duration_ == 0) {
			it = target->buffs_.erase(it); // �Ƴ�Ԫ�ز����µ�����
			continue; // �����������ĵ�������
		}
		++it; // ����δ�Ƴ�ʱ����������
	}
	
	if (numeric_value > 0) {
		int tag = 0;
		//buff�ܷ񱻵���
		if (buff->is_stackable_ == true) {
			for (auto Buff : target->buffs_)
			{
				//�Ѿ�����ͬ��buff
				if (Buff->name_ == buff->name_) {
					//����ʱ��
					if (Buff->stack_type_ == DURATION) {
						Buff->duration_ += numeric_value;
					}
					//Ч������
					else {
						Buff->effect_layers += numeric_value;
					}
					//����
					auto scene = (CombatScene*)(Director::getInstance()->getRunningScene());
					scene->creatureLayer->updateDisplay();
					tag = 1;
					break;
				}
			}
			if (tag == 0) {
				//����ʱ��
				if (buff->stack_type_ == DURATION) {
					buff->duration_ = numeric_value;
				}
				//Ч������
				else {
					buff->effect_layers = numeric_value;
				}
				target->buffs_.push_back(buff);
				//����
				auto scene = (CombatScene*)(Director::getInstance()->getRunningScene());
				scene->creatureLayer->updateDisplay();
			}

		}
	}
	//����
	for (auto Buff : target->buffs_)
	{
		CCLOG("have buff: %s. num is %d", typeid(*Buff).name(), Buff->effect_layers);
	}
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
	for (int i = 0; i < tempNum; i++)
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
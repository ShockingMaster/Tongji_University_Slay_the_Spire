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
	for (int i = 0; i < 9; i++)
	{
		for (auto name : tempDeck)
		{
			CCLOG("%s has been added!", name.c_str());
			drawPile.push(CardRegistry::createCard(name));
		}
	}

	// ��չ����б�,������ɹ���
	Monsters_.clear();
	int numMonsters = 1;
	for (int i = 0; i < numMonsters; i++)
	{
		// ���ɹ��ﲢ���뵽�����б�
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

	//��ֹ����������
	numeric_value_ = max(numeric_value_, 0);

	takeDamage(target, numeric_value_);
}

/*
 * �������ƣ�takeDamage
 * ��������������ָ�룬����������ֵ�������ߵ�ָ��
 * ���ܣ�������������buff����д��������ֵ
 */
void CombatSystem::takeDamage(std::shared_ptr<Creature> target, int numeric_value_, std::shared_ptr<Creature> attacker)
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
		target->loseBlock(target->getBlockValue());
		int healthLoss = numeric_value_ - target->getBlockValue();
		for (auto Buff : target->buffs_)
		{
			if (Buff != nullptr)
				Buff->onLoseHealth(healthLoss);
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
		target->loseHealth(healthLoss);
	}
}

/*
 * �������ƣ�startTurn
 * ���������ڽ��лغϿ�ʼ�������ָ��
 * ���ܣ���ɸ�����ĻغϿ�ʼ��buff�Ľ���
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
}

/*
 * �������ƣ�cardPlayed
 * ����������Ŀ��Ƶ�ָ��
 * ���ܣ���ɴ���������buff�Ľ���
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
 * �������ƣ�exhaustCard
 * �����������ĵĿ��Ƶ�ָ��
 * ���ܣ���ɴ���������buff�Ľ���
 */
void CombatSystem::exhaustCard(std::shared_ptr<Card> card)
{
	//���ȴ������Ʊ����ĵ�Ч��
	card->takeEffectOnExhaust();
	for (auto Buff : Player::getInstance()->buffs_)
	{
		if (Buff != nullptr)
		{
			Buff->onExhaustCard();
		}
	}
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
		if (drawPile.empty())
		{
			/*
			* �˴��Ȳ�������ϴ�Ƶ�����
			shuffle();
			*/
			CCLOG("No cards in my draw pile!");
		}

		// ��ȡ����
		auto card = drawPile.front();
		drawPile.pop();
		hand.push_back(card);
		CCLOG("the hand now has %d cards", hand.size());

		// ����ǰ�˳��ƶѵĳ���Ч��
		HandPileLayer::getInstance()->drawCard(card);
	}

}
#include "IncludeAll.h"

//������Ҫ������������
//0.��ͼ������Ҫ��������ֻ��Ҫһ������
//1.���ƣ���Ҫһ������������һ���������,�����ƶ�����
//2.ս��������ս���е������������ϴ��
//3.���ࣺ��������������Դ

/*
* ��Ҫ���ܣ�
* 1.����һ���������->1
* 2.����һ�����ҩˮ->3
* 3.����һ������->3
* 4.����һ���������->3
* 5.����һ������ָ�������޵�����->3
* 6.��һ�����飬����һ��Ԫ�ر�����˳�������->2
*/
RandomGenerator* RandomGenerator::getInstance()
{
	static RandomGenerator instance;
	return &instance;
}

/*
* �������ƣ�initializeGenerators
* ���ܣ�����һ���ַ����������ַ����趨������Ӳ���ʼ���������������
*/
void RandomGenerator::initializeGenerators(std::string seeds)
{
	std::hash<std::string> hasher;
	initialSeed_ = static_cast<unsigned int>(hasher(seeds));
	rng[0].seed(initialSeed_);
	rng[1].seed(initialSeed_);
	rng[2].seed(initialSeed_);
}
void RandomGenerator::initializeGenerators()
{
	srand(time(nullptr));
	initialSeed_ = rand() % INT_MAX;
	rng[0].seed(initialSeed_);
	rng[1].seed(initialSeed_);
	rng[2].seed(initialSeed_);
}
/*
* �������ƣ�getInitialSeed
* ���ܣ������趨������
*/
unsigned int RandomGenerator::getInitialSeed()
{
	return initialSeed_;
}

/*
* �������ܣ�getRandomNumber
* ���ܣ�����һ��[min��max]����
*/
int RandomGenerator::getRandomNumber(int min, int max)
{
	//ʹ�� OTHER ���������
	std::uniform_int_distribution<int> dist(min, max);
	return dist(rng[OTHER]);
}

/*
* �������ܣ�getRandomCard
* ���ܣ������п����з���һ����ָ�����ͺ�ϡ�жȵĿ���
* ���� type���������࣬Ĭ��Ϊ��ָ��
* ���� rarity������ϡ�жȣ�Ĭ��Ϊ��ָ��
*/
std::shared_ptr<Card> RandomGenerator::getRandomCard(int type, int rarity)
{
	// ʹ��CARD���������
	std::vector<std::string> allCardNames = CardRegistry::getAllCardNames();

	if (allCardNames.empty()) {
		throw std::runtime_error("No cards registered");
	}
	while (1)
	{
		// ʹ�þ��ȷֲ�������һ���������
		std::uniform_int_distribution<> dist(0, allCardNames.size() - 1);
		int randomIndex = dist(rng[CARD]);

		// ��ȡ�����������
		std::string randomCardName = allCardNames[randomIndex];

		// ������Ӧ�Ŀ��ƶ���
		auto card = CardRegistry::createCard(randomCardName);

		//�������Ҫ������з���
		if ((type == ALL || type == card->getType()) && (rarity == ALL || rarity == card->getRarity()) && card->getType() != STATUS)
		{
			return card;
		}
	}
}

std::shared_ptr<Monster> RandomGenerator::getRandomMonster(int type)
{
	// ʹ��CARD���������
	std::vector<std::string> allMonsterNames = MonsterRegistry::getAllMonsterNames();

	if (allMonsterNames.empty()) {
		throw std::runtime_error("No Monster registered");
	}
	while (1)
	{
		// ʹ�þ��ȷֲ�������һ���������
		std::uniform_int_distribution<> dist(0, allMonsterNames.size() - 1);
		int randomIndex = dist(rng[OTHER]);

		// ��ȡ�����������
		std::string randomMonsterName = allMonsterNames[randomIndex];

		// ������Ӧ�Ŀ��ƶ���
		auto monster = MonsterRegistry::createMonster(randomMonsterName);

		//�������Ҫ������з���
		if ((type == ALL || type == monster->getType()))
		{
			return monster;
		}
	}
}

std::shared_ptr<Relic> RandomGenerator::getRandomRelic(int rarity)
{
	// ʹ��OTHER���������
	std::vector<std::string> allRelicNames = RelicRegistry::getAllRelicNames();

	if (allRelicNames.empty()) {
		throw std::runtime_error("No cards registered");
	}
	while (1)
	{
		// ʹ�þ��ȷֲ�������һ���������
		std::uniform_int_distribution<> dist(0, allRelicNames.size() - 1);
		int randomIndex = dist(rng[OTHER]);

		// ��ȡ�����������
		std::string randomRelicName = allRelicNames[randomIndex];

		// ������Ӧ���������
		auto relic = RelicRegistry::createRelic(randomRelicName);

		// �������Ҫ������з���
		if ((rarity == ALL || rarity == relic->getRarity()))
		{
			return relic;
		}
	}
}

std::shared_ptr<Potion> RandomGenerator::getRandomPotion()
{
	std::vector<std::string> allPotionNames = PotionRegistry::getAllPotionNames();

	if (allPotionNames.empty()) {
		throw std::runtime_error("No Potion registered");
	}
	
	// ʹ�þ��ȷֲ�������һ���������
	std::uniform_int_distribution<> dist(0, allPotionNames.size() - 1);
	int randomIndex = dist(rng[OTHER]);

	// ��ȡ���ҩˮ����
	std::string randomPotionName = allPotionNames[randomIndex];

	// ������Ӧ��ҩˮ����
	auto relic = PotionRegistry::createPotion(randomPotionName);

	return relic;
	
}

/*
* �������ƣ�shuffleVector
* ���ܣ���������vector����˳��
*/
template<typename Elemtype>
void RandomGenerator::shuffleVector(std::vector<Elemtype>& vec)
{
	for (int i = vec.size() - 1;i > 0;i--)
	{
		std::uniform_int_distribution<> dist(0, i);
		int j = dist(rng[COMBAT]);
		std::swap(vec[i], vec[j]);
	}
}
template void RandomGenerator::shuffleVector<std::shared_ptr<Card>>(std::vector<std::shared_ptr<Card>>& vec);
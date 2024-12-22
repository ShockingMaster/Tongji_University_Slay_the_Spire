#include "IncludeAll.h"

//我们需要几个生成器：
//0.地图：不需要生成器，只需要一个种子
//1.卡牌：需要一个生成器返回一张随机卡牌,可以制定类型
//2.战斗：用于战斗中的随机数，比如洗牌
//3.其余：用于生成其他资源

/*
* 需要功能：
* 1.返回一张随机卡牌->1
* 2.返回一个随机药水->3
* 3.返回一个怪物->3
* 4.返回一个随机遗物->3
* 5.返回一个可以指定上下限的数字->3
* 6.给一个数组，返回一个元素被打乱顺序的数组->2
*/
RandomGenerator* RandomGenerator::getInstance()
{
	static RandomGenerator instance;
	return &instance;
}

/*
* 函数名称：initializeGenerators
* 功能：给定一个字符串，根据字符串设定随机种子并初始化随机种子生成器
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
* 函数名称：getInitialSeed
* 功能：返回设定的种子
*/
unsigned int RandomGenerator::getInitialSeed()
{
	return initialSeed_;
}

/*
* 函数功能：getRandomNumber
* 功能：返回一个[min，max]的数
*/
int RandomGenerator::getRandomNumber(int min, int max)
{
	//使用 OTHER 随机生成器
	std::uniform_int_distribution<int> dist(min, max);
	return dist(rng[OTHER]);
}

/*
* 函数功能：getRandomCard
* 功能：从所有卡牌中返回一个可指定类型和稀有度的卡牌
* 参数 type：卡牌种类，默认为不指定
* 参数 rarity：卡牌稀有度，默认为不指定
*/
std::shared_ptr<Card> RandomGenerator::getRandomCard(int type, int rarity)
{
	// 使用CARD随机生成器
	std::vector<std::string> allCardNames = CardRegistry::getAllCardNames();

	if (allCardNames.empty()) {
		throw std::runtime_error("No cards registered");
	}
	while (1)
	{
		// 使用均匀分布来生成一个随机索引
		std::uniform_int_distribution<> dist(0, allCardNames.size() - 1);
		int randomIndex = dist(rng[CARD]);

		// 获取随机卡牌名称
		std::string randomCardName = allCardNames[randomIndex];

		// 创建对应的卡牌对象
		auto card = CardRegistry::createCard(randomCardName);

		//如果满足要求则进行返回
		if ((type == ALL || type == card->getType()) && (rarity == ALL || rarity == card->getRarity()) && card->getType() != STATUS)
		{
			return card;
		}
	}
}

std::shared_ptr<Monster> RandomGenerator::getRandomMonster(int type)
{
	// 使用CARD随机生成器
	std::vector<std::string> allMonsterNames = MonsterRegistry::getAllMonsterNames();

	if (allMonsterNames.empty()) {
		throw std::runtime_error("No Monster registered");
	}
	while (1)
	{
		// 使用均匀分布来生成一个随机索引
		std::uniform_int_distribution<> dist(0, allMonsterNames.size() - 1);
		int randomIndex = dist(rng[OTHER]);

		// 获取随机卡牌名称
		std::string randomMonsterName = allMonsterNames[randomIndex];

		// 创建对应的卡牌对象
		auto monster = MonsterRegistry::createMonster(randomMonsterName);

		//如果满足要求则进行返回
		if ((type == ALL || type == monster->getType()))
		{
			return monster;
		}
	}
}

std::shared_ptr<Relic> RandomGenerator::getRandomRelic(int rarity)
{
	// 使用OTHER随机生成器
	std::vector<std::string> allRelicNames = RelicRegistry::getAllRelicNames();

	if (allRelicNames.empty()) {
		throw std::runtime_error("No cards registered");
	}
	while (1)
	{
		// 使用均匀分布来生成一个随机索引
		std::uniform_int_distribution<> dist(0, allRelicNames.size() - 1);
		int randomIndex = dist(rng[OTHER]);

		// 获取随机遗物名称
		std::string randomRelicName = allRelicNames[randomIndex];

		// 创建对应的遗物对象
		auto relic = RelicRegistry::createRelic(randomRelicName);

		// 如果满足要求则进行返回
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
	
	// 使用均匀分布来生成一个随机索引
	std::uniform_int_distribution<> dist(0, allPotionNames.size() - 1);
	int randomIndex = dist(rng[OTHER]);

	// 获取随机药水名称
	std::string randomPotionName = allPotionNames[randomIndex];

	// 创建对应的药水对象
	auto relic = PotionRegistry::createPotion(randomPotionName);

	return relic;
	
}

/*
* 函数名称：shuffleVector
* 功能：将给定的vector打乱顺序
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
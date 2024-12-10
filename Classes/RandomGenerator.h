#pragma once
#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <random>
#include <string>
#include <memory>
#include <mutex>
#include <vector>
#include <time.h>
#include <functional>
#include "Enum.h"
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
enum RandomType {
    CARD = 0,
    BATTLE = 1,
    OTHER = 2
};
class Card;

class RandomGenerator
{
public:
    // 获取单例指针
    static RandomGenerator* getInstance();

    // 禁止拷贝和赋值构造
    RandomGenerator(const RandomGenerator&) = delete;
    RandomGenerator& operator=(const RandomGenerator&) = delete;

    // 初始化随机数生成器,可以考虑直接用名字进行初始化
    void initializeGenerators(std::string seeds);

    // 返回随机数种子
    unsigned int getInitialSeed();

    // 获取指定生成器生成的随机数（范围：[min, max]）
    int getRandomNumber(int min, int max);

    // 获取一张随机卡牌,可以指定类型和稀有度
    std::shared_ptr<Card> getRandomCard(int type = ALL, int rarity = ALL);

    // 获取一个随机怪物,可以指定怪物强度（小怪，精英，Boss）
    std::shared_ptr<Creature> getRandomMonster(int type = NORMAL);

    // 获取一个随机遗物，可以指定稀有度
    std::shared_ptr<Relic> getRandomRelic(int rarity = ALL);

    // 获取一个随机药水
    std::shared_ptr<Potion> getRandomPotion();

    // 对数组进行打乱
    template<typename Elemtype>
    void shuffleVector(std::vector<Elemtype>&);

    // 保存生成器状态到文件
    // void saveStateToFile(const std::string& filename);

    // 从文件恢复生成器状态
    // void loadStateFromFile(const std::string& filename);

private:
    RandomGenerator() = default;
    unsigned int initialSeed_;
    std::mt19937 rng[3];  // 三个固定的生成器
};

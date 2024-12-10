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
enum RandomType {
    CARD = 0,
    BATTLE = 1,
    OTHER = 2
};
class Card;

class RandomGenerator
{
public:
    // ��ȡ����ָ��
    static RandomGenerator* getInstance();

    // ��ֹ�����͸�ֵ����
    RandomGenerator(const RandomGenerator&) = delete;
    RandomGenerator& operator=(const RandomGenerator&) = delete;

    // ��ʼ�������������,���Կ���ֱ�������ֽ��г�ʼ��
    void initializeGenerators(std::string seeds);

    // �������������
    unsigned int getInitialSeed();

    // ��ȡָ�����������ɵ����������Χ��[min, max]��
    int getRandomNumber(int min, int max);

    // ��ȡһ���������,����ָ�����ͺ�ϡ�ж�
    std::shared_ptr<Card> getRandomCard(int type = ALL, int rarity = ALL);

    // ��ȡһ���������,����ָ������ǿ�ȣ�С�֣���Ӣ��Boss��
    std::shared_ptr<Creature> getRandomMonster(int type = NORMAL);

    // ��ȡһ������������ָ��ϡ�ж�
    std::shared_ptr<Relic> getRandomRelic(int rarity = ALL);

    // ��ȡһ�����ҩˮ
    std::shared_ptr<Potion> getRandomPotion();

    // ��������д���
    template<typename Elemtype>
    void shuffleVector(std::vector<Elemtype>&);

    // ����������״̬���ļ�
    // void saveStateToFile(const std::string& filename);

    // ���ļ��ָ�������״̬
    // void loadStateFromFile(const std::string& filename);

private:
    RandomGenerator() = default;
    unsigned int initialSeed_;
    std::mt19937 rng[3];  // �����̶���������
};

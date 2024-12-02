#pragma once
#include <vector>
#include <string>
using namespace std;

class Creature;

/**
 * @class Card
 * @brief 代表一张卡牌的类
 *
 * 卡牌具有名字、描述、能量消耗、购买价格、稀有度等属性，
 * 并且可能会有针对单个或多个目标的效果。
 */
class Card{
public:
    string name_;             ///< 卡牌名称
    string description_;      ///< 卡牌描述
    int energyCost_;          ///< 卡牌能量消耗
    int moneyCost_;           ///< 卡牌商店购买价格
    int rarity_;              ///< 卡牌稀有度
    bool isEnabled_;          ///< 是否可以打出此卡牌
    bool targetNum;           ///< 施加攻击的对象数量（单个或多个目标）
    int type_;                ///< 卡牌类型（如攻击卡、防御卡等）
    int triggerTimes_;        ///< 卡牌效果触发次数
    // 构造函数
    Card(string name, string description, int energyCost, int moneyCost, int rarity, bool isEnabled, int type, int triggerTimes);

    // 取效果方法，针对单个目标
    void takeEffect(Creature* target);

    // 取效果方法，针对多个目标
    void takeEffect(vector<Creature*> targets);

    //永久升级 锻造卡牌
    void update();
};

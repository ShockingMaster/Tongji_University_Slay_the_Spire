#pragma once
#include <queue>
#include <vector>
#include "Player.h"
#include "Card.h"

using namespace std;

/**
 * @class CombatDeck
 * @brief 表示玩家的战斗卡组，包括抽牌堆、手牌和弃牌堆。
 *
 * CombatDeck 类处理玩家卡组的基本操作，例如抽卡、弃卡、洗牌、卡片升级、卡片消耗等。
 */
class CombatDeck {
public:
    queue<Card*> draw_pile_;         ///< 抽牌堆
    queue<Card*> hand_;              ///< 手牌
    queue<Card*> discard_pile_;      ///< 弃牌堆

    //洗牌
    void shuffle();

    //抽取指定数量的卡牌
    void draw(int num);

    //检查抽牌堆是否为空
    int is_draw_pile_empty();

    //检查弃牌堆是否为空
    int is_discard_pile_empty();

    //添加到手牌中
    void add_to_hand(Card* card, int num = 1);

    // 将卡牌添加到抽牌堆
    void add_to_draw_pile(Card* card, int num = 1);

    //brief 将卡牌添加到弃牌堆=
    void add_to_discard_pile(Card* card, int num = 1);

    //将卡牌从手牌丢弃到弃牌堆
    void discard(Card* card);

    //消耗卡牌

    void exhaust(Card* card);

    //临时升级卡牌
    void upgrade(Card* card);

    /**
     * @brief 选择目标并执行操作
     *
     * 该方法接受一个操作类型（如 discard、exhaust、upgrade 等），
     * 根据目标数量执行相应的操作。
     *
     * @param num 选择目标的数量
     * @param operation 执行的操作类型
     */
    template<typename operation_type>
    void choose_target(const int num, operation_type operation);

    //显示当前卡组信息
    void show_info();

    //brief 初始化卡组
    void init();
};

// 模板函数的实现
template<typename operation_type>
inline void CombatDeck::choose_target(const int num, operation_type operation) {

}

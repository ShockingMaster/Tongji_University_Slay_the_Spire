#pragma once
#include<queue>
#include<vector>
#include "Player.h"
using namespace std;
class Card;
class CombatDeck
{
public:
    queue<Card*> draw_pile_;
    queue<Card*> hand_;
    queue<Card*> discard_pile_;
                                                                    //以洗牌为例，展示buff的触发方式
    void shuffle()
    {
                                                                    //先执行正常的洗牌操作
                                                                    //再触发洗牌相关的buff
    }
    
    void draw(int num);                                             //先判断抽牌堆是否为空，洗牌后调用重复多次add_to_hand
    
    int is_draw_pile_empty();
    
    int is_discard_pile_empty();
    
    void add_to_hand(Card* card, int num = 1);
    
    void add_to_draw_pile(Card* card, int num = 1);
    
    void add_to_discard_pile(Card* card, int num = 1);
    
    void discard(Card* card);                                       //同样的，我们可以根据上面的方式进行buff的触发
    
    void exhaust(Card* card);                                       //战斗临时消耗
    
    void upgrade(Card* card);                                       //战斗临时升级
    
    template<typename operation_type>
    void choose_target(const int num, operation_type operation);    //这个玩意需要执行
    
    //opeartion_type可以是discard、exhaust、upgrade等
    
    void show_info();
    
    void init();                                                    //每场战斗开始时将固有卡组加入抽牌堆
};


template<typename operation_type>
inline void CombatDeck::choose_target(const int num, operation_type operation)
{
}

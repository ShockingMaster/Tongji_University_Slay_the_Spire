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
                                                                    //��ϴ��Ϊ����չʾbuff�Ĵ�����ʽ
    void shuffle()
    {
                                                                    //��ִ��������ϴ�Ʋ���
                                                                    //�ٴ���ϴ����ص�buff
    }
    
    void draw(int num);                                             //���жϳ��ƶ��Ƿ�Ϊ�գ�ϴ�ƺ�����ظ����add_to_hand
    
    int is_draw_pile_empty();
    
    int is_discard_pile_empty();
    
    void add_to_hand(Card* card, int num = 1);
    
    void add_to_draw_pile(Card* card, int num = 1);
    
    void add_to_discard_pile(Card* card, int num = 1);
    
    void discard(Card* card);                                       //ͬ���ģ����ǿ��Ը�������ķ�ʽ����buff�Ĵ���
    
    void exhaust(Card* card);                                       //ս����ʱ����
    
    void upgrade(Card* card);                                       //ս����ʱ����
    
    template<typename operation_type>
    void choose_target(const int num, operation_type operation);    //���������Ҫִ��
    
    //opeartion_type������discard��exhaust��upgrade��
    
    void show_info();
    
    void init();                                                    //ÿ��ս����ʼʱ�����п��������ƶ�
};


template<typename operation_type>
inline void CombatDeck::choose_target(const int num, operation_type operation)
{
}

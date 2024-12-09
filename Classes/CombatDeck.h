#pragma once
#include <queue>
#include <vector>
#include "Player.h"
#include "Card.h"

using namespace std;

/**
 * @class CombatDeck
 * @brief ��ʾ��ҵ�ս�����飬�������ƶѡ����ƺ����ƶѡ�
 *
 * CombatDeck �ദ����ҿ���Ļ�������������鿨��������ϴ�ơ���Ƭ��������Ƭ���ĵȡ�
 */
class CombatDeck {
public:
    queue<Card*> draw_pile_;         ///< ���ƶ�
    queue<Card*> hand_;              ///< ����
    queue<Card*> discard_pile_;      ///< ���ƶ�

    //ϴ��
    void shuffle();

    //��ȡָ�������Ŀ���
    void draw(int num);

    //�����ƶ��Ƿ�Ϊ��
    int is_draw_pile_empty();

    //������ƶ��Ƿ�Ϊ��
    int is_discard_pile_empty();

    //��ӵ�������
    void add_to_hand(Card* card, int num = 1);

    // ��������ӵ����ƶ�
    void add_to_draw_pile(Card* card, int num = 1);

    //brief ��������ӵ����ƶ�=
    void add_to_discard_pile(Card* card, int num = 1);

    //�����ƴ����ƶ��������ƶ�
    void discard(Card* card);

    //���Ŀ���

    void exhaust(Card* card);

    //��ʱ��������
    void upgrade(Card* card);

    /**
     * @brief ѡ��Ŀ�겢ִ�в���
     *
     * �÷�������һ���������ͣ��� discard��exhaust��upgrade �ȣ���
     * ����Ŀ������ִ����Ӧ�Ĳ�����
     *
     * @param num ѡ��Ŀ�������
     * @param operation ִ�еĲ�������
     */
    template<typename operation_type>
    void choose_target(const int num, operation_type operation);

    //��ʾ��ǰ������Ϣ
    void show_info();

    //brief ��ʼ������
    void init();
};

// ģ�庯����ʵ��
template<typename operation_type>
inline void CombatDeck::choose_target(const int num, operation_type operation) {

}

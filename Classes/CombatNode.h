#pragma once
#include "CombatDeck.h"
class Player;
class CombatNode//ս���ڵ���
{
public:
    CombatDeck combat_deck_;
    int init();//��ɳ����ĳ�ʼ��,ͬʱ���н�ɫ�͵��˵Ĵ���
    int turn;//���лغ����ļ�¼��ͨ����ż�ֱ��ǵз��غϻ��Ǽ����غ� 
};
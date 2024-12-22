#pragma once
/*
* ��һ��������ʾ��ҽ�ɫ�͹���ͼ��
* ������Ҫ�Ĺ����У�
* 1.�ṩ��ʼ����ͨ��EventSystem��ɶ�����ҽ�ɫ�ĳ�ʼ����ͨ��������г�ʼ������
* 2.�ṩ���·�����ÿ��Ѫ�����񵲲����仯ʱ���Ի�����и���
* 
*/
#pragma once
#include "cocos2d.h"
#include <memory>
#include <queue>
#include <vector>
using namespace cocos2d;
class Card;
class CombatSystem;
class Creature;
class Player;
class Monster;
class CreatureLayer : public cocos2d::Layer
{
public:

	// ���������
	static CreatureLayer* CreatureLayer::create(std::vector<std::shared_ptr<Creature>>& monsters);

	// ���������г�ʼ��
	bool CreatureLayer::init(std::vector<std::shared_ptr<Creature>>& monsters);

	// ִ�й�������
	void attackAction(std::shared_ptr<Creature> creature);

	// �����������ʾ
	void updateDisplay();
private:
	cocos2d::Sprite* playerModel;
	cocos2d::ProgressTimer* playerHealth;
	cocos2d::Label* playerHealthLabel;
	cocos2d::Label* playerBlockLabel;
	cocos2d::Sprite* playerDefend;
	std::vector<Sprite*> monsterBlock;
	std::vector<Sprite*> monsterHealthBackground;
	std::vector<ProgressTimer*> monsterHealth;
	std::vector<cocos2d::Label*> monsterHealthLabel;
	std::vector<cocos2d::Label*> monsterBlockLabel;
	std::vector<cocos2d::Sprite*> intent;
	std::vector<cocos2d::Sprite*> monsterBuff;
	std::vector<cocos2d::Sprite*> playerBuff;
	std::vector<cocos2d::Label*> monsterBuff_value;
	std::vector<cocos2d::Label*> playerBuff_value;
	std::vector<cocos2d::Label*> attack_value_list;
	std::vector<cocos2d::Sprite*> monsterModel;
};
#pragma once
#include "Creature.h"
#include "cocos2d.h"
#include "string"
#include <memory>

// 前向声明（避免循环依赖）
class MonsterStateMachine;

class Monster : public Creature
{
protected:
	// Refactored with State Pattern
	// 状态机（可选，使用State模式的怪物可以使用此成员）
	std::shared_ptr<MonsterStateMachine> stateMachine_;

public:
	Monster(int type, int fullhealth, std::string name, int monster_num) :
		type_(type) {
		max_health_ = fullhealth;
		health_ = fullhealth;
		name_ = name;
		block_ = 0;
		monster_num_ = monster_num;
	};
	void startTurn();

	void endTurn();

	int getType() const;

    void setRect(cocos2d::Vec2 position, float width, float height);

    // �ж�һ�����Ƿ��ڹ���������
    bool containsPoint(cocos2d::Vec2 point);

	int getAttackTime()  const {
		return attack_times;
	};

	int getAttackValue() const {
		return attack_numeric_value;
	};

	int getMonsterNum() const {
		return monster_num_;
	}

	virtual void takeEffect();

	virtual std::string intentionDisplay();

    // ��ȡ���������
	cocos2d::Rect getRect();

	cocos2d::Vec2 position;
	cocos2d::Rect area;  // ����һ���������������ж�
	int type_;
	int monster_num_ = 1;
	int tag = 0; //�����ж�tag
	int round_num = 1;
	int attack_times = 0;
	int attack_numeric_value = 0;
	virtual ~Monster() {};
	
};
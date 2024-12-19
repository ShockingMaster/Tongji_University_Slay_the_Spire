#pragma once
#include "Creature.h"
#include "cocos2d.h"
#include "string"
class Monster : public Creature
{
public:
	Monster(int type, int fullhealth) :
		type_(type) {
		max_health_ = fullhealth;
		health_ = fullhealth;
		block_ = 0;
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

	virtual void takeEffect();

	virtual std::string intentionDisplay();

    // ��ȡ���������
	cocos2d::Rect getRect();

	cocos2d::Vec2 position;
	cocos2d::Rect area;  // ����һ���������������ж�
	int type_;
	int tag = 0; //�����ж�tag
	int round_num = 1;
	int attack_times = 0;
	int attack_numeric_value = 0;

	virtual ~Monster() {};
};
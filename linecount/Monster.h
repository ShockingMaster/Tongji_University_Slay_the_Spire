#pragma once
#include "Creature.h"
#include "cocos2d.h"
#include "string"
class Monster : public Creature
{
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

    // 判断一个点是否在怪物区域内
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

    // 获取怪物的区域
	cocos2d::Rect getRect();

	cocos2d::Vec2 position;
	cocos2d::Rect area;  // 定义一个矩形区域用于判断
	int type_;
	int monster_num_ = 1;
	int tag = 0; //怪物行动tag
	int round_num = 1;
	int attack_times = 0;
	int attack_numeric_value = 0;
	virtual ~Monster() {};
	
};
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

    // 判断一个点是否在怪物区域内
    bool containsPoint(cocos2d::Vec2 point);
	virtual void takeEffect();

	virtual string Intention_display();

    // 获取怪物的区域
	cocos2d::Rect getRect();

	cocos2d::Vec2 position;
	cocos2d::Rect area;  // 定义一个矩形区域用于判断
	int type_;
	int tag = 0; //怪物行动tag

};
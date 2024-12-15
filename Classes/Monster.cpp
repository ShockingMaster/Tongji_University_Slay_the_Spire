#include "Monster.h"
#include "IncludeAll.h"
void Monster::startTurn()
{
	
}

void Monster::endTurn()
{

}

int Monster::getType() const
{
	return type_;
}

void Monster::setRect(cocos2d::Vec2 position, float width, float height)
{
    // 使用中心点位置来设置矩形区域
    area = cocos2d::Rect(position.x - width / 2, position.y - height / 2, width, height);
}

bool Monster::containsPoint(cocos2d::Vec2 point)
{
    return area.containsPoint(point);
}

cocos2d::Rect Monster::getRect()
{
    return area;
}

class testMonster : public Monster
{
public:
    testMonster() : Monster(NORMAL, 100) {}
};

AUTO_REGISTER_MONSTER(testMonster)
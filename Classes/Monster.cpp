#include "Monster.h"
#include "IncludeAll.h"
#include "string"
#include "random"
void Monster::startTurn()
{
	
}

void Monster::endTurn()
{

}

std::string Monster::Intention_display() {
    return "";
}

void Monster::takeEffect() {
    
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

/*
class testMonster : public Monster
{
public:
    testMonster() : Monster(NORMAL, 100) {}
    void takeEffect() {}
    std::string Intention_display() {
        string a = "test";
        return a;
    }
};

AUTO_REGISTER_MONSTER(testMonster)
*/

class Mandibular_worm : public Monster
{
public:
    Mandibular_worm() : Monster(NORMAL, 40) {}
    void takeEffect() {
        std::random_device rd;               // 用于生成随机种子
        std::mt19937 gen(rd());              // 随机数引擎
        std::uniform_real_distribution<> dist(0.0, 1.0); // [0.0, 1.0) 均匀分布
        double randomValue = dist(gen); // 生成随机数

        if (tag == 0) {
            CombatSystem::getInstance()->onAttack(std::make_shared<Mandibular_worm>(), Player::getInstance(),
                11, "Attack");
            if (randomValue < 0.6) {
                tag = 1; // 60% 的概率赋值为 1
            }
            else {
                tag = 2; // 40% 的概率赋值为 2
            }
        }
        else if (tag == 1) {
            //CombatSystem::getInstance()->addBuff(std::make_shared<Mandibular_worm>(), Player::getInstance(),11, "Attack");  //增加buff
            if (randomValue < 0.45) {
                tag = 0; // 45% 的概率赋值为 0
            }
            else {
                tag = 2; // 55% 的概率赋值为 2
            }
        }
        else{
            CombatSystem::getInstance()->onAttack(std::make_shared<Mandibular_worm>(), Player::getInstance(),
                7, "Attack");
            CombatSystem::getInstance()->Addblock(std::make_shared<Mandibular_worm>(),5 );
            if (randomValue < 0.36) {
                tag = 0; // 36% 的概率赋值为 0
            }
            else {
                tag = 1; // 64% 的概率赋值为 1
            }
        }
    }
    std::string Intention_display() {
        if (tag == 0) {
            return "Mandibular_worm_intention1";
        }
        else if (tag == 1) {
            return "Mandibular_worm_intention2";
        }
        else {
            return "Mandibular_worm_intention3";
        }
    }
};

AUTO_REGISTER_MONSTER(Mandibular_worm)
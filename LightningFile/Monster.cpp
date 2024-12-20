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

std::string Monster::intentionDisplay() {
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
            attack_value="9";
            return "attack.png";
        }
        else if (tag == 1) {
            return "defendBuff.png";
        }
        else {
            return "attackDefend.png";
        }
    }
};

AUTO_REGISTER_MONSTER(Mandibular_worm)


class sentinel : public Monster
{
public:
    sentinel() : Monster(NORMAL, 38) {}
    void takeEffect() {
        if (tag == 0) {
            CombatSystem::getInstance()->onAttack(std::make_shared<sentinel>(), Player::getInstance(),
                9, "Attack");
            tag = 1;
        }
        else if (tag == 1) {
            //玩家手中塞牌
            //CombatSystem::getInstance()->discardPile.push(CardRegistry::createCard("dazed"));
            //HandPileLayer::getInstance()->updateDiscardPileDisplay();
            tag = 0;
        }
    }
    std::string Intention_display() {
        if (tag == 0) {
            attack_value = "9";
            return "attack.png";
        }
        else {
            return "debuff1.png";
        }
    }
};

AUTO_REGISTER_MONSTER(sentinel)
*/


class Six_Fire_Souls : public Monster
{
public:
    Six_Fire_Souls() : Monster(NORMAL, 2) {}
    void takeEffect() {
        std::shared_ptr<Creature> thisMonster = CombatSystem::getInstance()->getMonsterPointer(this);
        if (tag == 0) {
            int basic_attack_value = 6;
            CombatSystem::getInstance()->onAttack(thisMonster, Player::getInstance(),
                basic_attack_value, "");
            //塞入一张灼热
            round_num++;
        }
        else if (tag == 1) {
            int basic_attack_value = 6;
            CombatSystem::getInstance()->onAttack(thisMonster, Player::getInstance(),
                basic_attack_value, "");
            CombatSystem::getInstance()->onAttack(thisMonster, Player::getInstance(),
                basic_attack_value, "");
            round_num++;
        }
        else if (tag == 2) {
            CombatSystem::getInstance()->Addblock(thisMonster, 12);
            //获得力量Buff
            round_num++;
        }
        else if (tag == 3) {
            for (int i = 0; i < 6; i++) {
                int attackValue = Player::getInstance()->getHealth() / 12 + 1;
                CombatSystem::getInstance()->onAttack(thisMonster, Player::getInstance(),
                    attackValue, "");
            }
            //塞入三张灼热
            round_num++;
        }
        if (round_num % 7 == 1 || round_num % 7 == 3 || round_num % 7 == 6) {
            tag = 0;
        }
        else if (round_num % 7 == 2 || round_num % 7 == 5) {
            tag = 1;
        }
        else if (round_num % 7 == 4) {
            tag = 2;
        }
        else if (round_num % 7 == 0) {
            tag = 3;
        }
    }
    std::string intentionDisplay() 
    {
        std::shared_ptr<Creature> thisMonster = CombatSystem::getInstance()->getMonsterPointer(this);
        // 在修改意图的时候计算造成的伤害数值和段数
        if (tag == 0) 
        {
            attack_numeric_value = 6;
            CombatSystem::getInstance()->onAttack(thisMonster, Player::getInstance(),
                attack_numeric_value, "", true);
            attack_times = 1;
            return "attack.png";
        }
        else if(tag == 1) 
        {
            attack_numeric_value = 6;
            CombatSystem::getInstance()->onAttack(thisMonster, Player::getInstance(),
                attack_numeric_value, "", true);
            attack_times = 2;
            return "attack.png";
        }
        else if (tag == 2) 
        {
            return "defendBuff.png";
        }
        else if (tag == 3) 
        {
            attack_numeric_value = Player::getInstance()->getHealth() / 12 + 1;
            CombatSystem::getInstance()->onAttack(thisMonster, Player::getInstance(),
                attack_numeric_value, "", true);
            attack_times = 6;
            return "attack.png";
        }
        else
        {
            return "";
        }
    }
};
AUTO_REGISTER_MONSTER(Six_Fire_Souls)
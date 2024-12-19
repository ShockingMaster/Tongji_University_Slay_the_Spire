#pragma once
/*
* 这一层用于显示玩家角色和怪物图像
* 我们需要的功能有：
* 1.提供初始化，通过EventSystem完成对于玩家角色的初始化，通过怪物进行初始化怪物
* 2.提供更新方法，每当血量，格挡产生变化时，对画面进行更新
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

	// 创建生物层
	static CreatureLayer* CreatureLayer::create(std::vector<std::shared_ptr<Creature>>& monsters);

	// 对生物层进行初始化
	bool CreatureLayer::init(std::vector<std::shared_ptr<Creature>>& monsters);

	// 更新生物层显示
	void updateDisplay();
private:
	cocos2d::ProgressTimer* playerHealth;
	cocos2d::Label* playerHealthLabel;
	cocos2d::Label* playerBlockLabel;
	cocos2d::Sprite* playerDefend;
	std::vector<Sprite*> monsterBlock;
	std::vector<ProgressTimer*> monsterHealth;
	std::vector<cocos2d::Label*> monsterHealthLabel;
	std::vector<cocos2d::Label*> monsterBlockLabel;
	std::vector<cocos2d::Sprite*> intent;
	std::vector<cocos2d::Label*> attack_value_list;
};
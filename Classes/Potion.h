#pragma once
#include <vector>
#include <string>
#include "Creature.h"
using namespace std;

// 前向声明 Creature 类，假设它代表游戏中的角色或敌人
class Creature;

class Potion {
public:
    string name_;            // 药水的名字
    string description_;     // 药水的描述
    int numeric_value_;      // 药水的数值（例如恢复生命、造成伤害等）
    int money_cost_;         // 药水的金钱花费
    int rarity_;             // 药水的稀有度
    bool is_enabled_;        // 是否启用
    
    // 析构函数
    virtual ~Potion() {}

    // 药水效果，给定目标
    virtual void take_effect(Creature* target) = 0;

    // 对多个目标使用药水的效果
    virtual void take_effect(vector<Creature*> targets) = 0;

    // 展示药水信息
    void show_info();
};


// HealthPotion: 恢复生命药水
class HealthPotion : public Potion {
public:
    HealthPotion() {
        name_ = "Health Potion";
        description_ = "恢复 20 点生命";
        numeric_value_ = 20;
        money_cost_ = 50;
        rarity_ = 1;  // 常见药水
        is_enabled_ = true;
    }

    void take_effect(Creature* target) override {
        // 恢复目标生命
        target->health_+=(numeric_value_);
    }
    void take_effect(vector<Creature*> targets) override {

    }
};

// PoisonPotion: 毒药瓶
class PoisonPotion : public Potion {
public:
    PoisonPotion() {
        name_ = "Poison Potion";
        description_ = "对敌人造成 10 点毒伤";
        numeric_value_ = 10;
        money_cost_ = 60;
        rarity_ = 2;
        is_enabled_ = true;
    }

    void take_effect(Creature* target) override {
        // 对目标造成毒伤
        target->health_-=(numeric_value_);
    }

    void take_effect(vector<Creature*> targets) override {
        for (auto& target : targets) {
            target->health_ -= (numeric_value_);
        }
    }
};



// BlockPotion: 抗性药水
class BlockPotion : public Potion {
public:
    BlockPotion() {
        name_ = "Block Potion";
        description_ = "增加 15 点护盾";
        numeric_value_ = 15;
        money_cost_ = 40;
        rarity_ = 1;
        is_enabled_ = true;
    }

    void take_effect(Creature* target) override {
        // 增加护盾
        target->block_+=(numeric_value_);
    }
    void take_effect(vector<Creature*> targets) override {

    }
   
};

// GoldPotion: 金钱药水
class GoldPotion : public Potion {
public:
    GoldPotion() {
        name_ = "Gold Potion";
        description_ = "获得 100 金币";
        numeric_value_ = 100;
        money_cost_ = 100;
        rarity_ = 1;
        is_enabled_ = true;
    }

    void take_effect(Creature* target) override {
        // 增加金币
        
    }
    void take_effect(vector<Creature*> targets) override {

    }
};

// ExplosivePotion: 爆炸药水
class ExplosivePotion : public Potion {
public:
    ExplosivePotion() {
        name_ = "Explosive Potion";
        description_ = "对所有敌人造成 15 点爆炸伤害";
        numeric_value_ = 15;
        money_cost_ = 120;
        rarity_ = 2;
        is_enabled_ = true;
    }

    void take_effect(Creature* target) override {
        // 对敌人造成爆炸伤害
       
    }

    void take_effect(vector<Creature*> targets) override {
        for (auto& target : targets) {
           
        }
    }
};

// StrengthPotion: 强力药水
class StrengthPotion : public Potion {
public:
    StrengthPotion() {
        name_ = "Strength Potion";
        description_ = "增加 5 点力量";
        numeric_value_ = 5;
        money_cost_ = 90;
        rarity_ = 3;
        is_enabled_ = true;
    }

    void take_effect(Creature* target) override {
        // 增加力量
       
    }

    void take_effect(vector<Creature*> targets) override {
        for (auto& target : targets) {
           
        }
    }
};

// RegenerationPotion: 再生药水
class RegenerationPotion : public Potion {
public:
    RegenerationPotion() {
        name_ = "Regeneration Potion";
        description_ = "恢复 10 点生命值，并持续 3 回合恢复";
        numeric_value_ = 10;
        money_cost_ = 70;
        rarity_ = 2;
        is_enabled_ = true;
    }

    void take_effect(Creature* target) override {
        // 恢复生命并持续恢复
      
    }

    void take_effect(vector<Creature*> targets) override {
        for (auto& target : targets) {
            
        }
    }
};

// EnergyPotion: 能量药水
class EnergyPotion : public Potion {
public:
    EnergyPotion() {
        name_ = "Energy Potion";
        description_ = "恢复 2 点能量";
        numeric_value_ = 2;
        money_cost_ = 50;
        rarity_ = 1;
        is_enabled_ = true;
    }

    void take_effect(Creature* target) override {
        // 恢复能量
        
    }

    void take_effect(vector<Creature*> targets) override {
        for (auto& target : targets) {
           
        }
    }
};





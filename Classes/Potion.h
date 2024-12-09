#pragma once
#include <vector>
#include <string>
#include "Creature.h"
using namespace std;

// ǰ������ Creature �࣬������������Ϸ�еĽ�ɫ�����
class Creature;

class Potion {
public:
    string name_;            // ҩˮ������
    string description_;     // ҩˮ������
    int numeric_value_;      // ҩˮ����ֵ������ָ�����������˺��ȣ�
    int money_cost_;         // ҩˮ�Ľ�Ǯ����
    int rarity_;             // ҩˮ��ϡ�ж�
    bool is_enabled_;        // �Ƿ�����
    
    // ��������
    virtual ~Potion() {}

    // ҩˮЧ��������Ŀ��
    virtual void take_effect(Creature* target) = 0;

    // �Զ��Ŀ��ʹ��ҩˮ��Ч��
    virtual void take_effect(vector<Creature*> targets) = 0;

    // չʾҩˮ��Ϣ
    void show_info();
};


// HealthPotion: �ָ�����ҩˮ
class HealthPotion : public Potion {
public:
    HealthPotion() {
        name_ = "Health Potion";
        description_ = "�ָ� 20 ������";
        numeric_value_ = 20;
        money_cost_ = 50;
        rarity_ = 1;  // ����ҩˮ
        is_enabled_ = true;
    }

    void take_effect(Creature* target) override {
        // �ָ�Ŀ������
        target->health_+=(numeric_value_);
    }
    void take_effect(vector<Creature*> targets) override {

    }
};

// PoisonPotion: ��ҩƿ
class PoisonPotion : public Potion {
public:
    PoisonPotion() {
        name_ = "Poison Potion";
        description_ = "�Ե������ 10 �㶾��";
        numeric_value_ = 10;
        money_cost_ = 60;
        rarity_ = 2;
        is_enabled_ = true;
    }

    void take_effect(Creature* target) override {
        // ��Ŀ����ɶ���
        target->health_-=(numeric_value_);
    }

    void take_effect(vector<Creature*> targets) override {
        for (auto& target : targets) {
            target->health_ -= (numeric_value_);
        }
    }
};



// BlockPotion: ����ҩˮ
class BlockPotion : public Potion {
public:
    BlockPotion() {
        name_ = "Block Potion";
        description_ = "���� 15 �㻤��";
        numeric_value_ = 15;
        money_cost_ = 40;
        rarity_ = 1;
        is_enabled_ = true;
    }

    void take_effect(Creature* target) override {
        // ���ӻ���
        target->block_+=(numeric_value_);
    }
    void take_effect(vector<Creature*> targets) override {

    }
   
};

// GoldPotion: ��Ǯҩˮ
class GoldPotion : public Potion {
public:
    GoldPotion() {
        name_ = "Gold Potion";
        description_ = "��� 100 ���";
        numeric_value_ = 100;
        money_cost_ = 100;
        rarity_ = 1;
        is_enabled_ = true;
    }

    void take_effect(Creature* target) override {
        // ���ӽ��
        
    }
    void take_effect(vector<Creature*> targets) override {

    }
};

// ExplosivePotion: ��ըҩˮ
class ExplosivePotion : public Potion {
public:
    ExplosivePotion() {
        name_ = "Explosive Potion";
        description_ = "�����е������ 15 �㱬ը�˺�";
        numeric_value_ = 15;
        money_cost_ = 120;
        rarity_ = 2;
        is_enabled_ = true;
    }

    void take_effect(Creature* target) override {
        // �Ե�����ɱ�ը�˺�
       
    }

    void take_effect(vector<Creature*> targets) override {
        for (auto& target : targets) {
           
        }
    }
};

// StrengthPotion: ǿ��ҩˮ
class StrengthPotion : public Potion {
public:
    StrengthPotion() {
        name_ = "Strength Potion";
        description_ = "���� 5 ������";
        numeric_value_ = 5;
        money_cost_ = 90;
        rarity_ = 3;
        is_enabled_ = true;
    }

    void take_effect(Creature* target) override {
        // ��������
       
    }

    void take_effect(vector<Creature*> targets) override {
        for (auto& target : targets) {
           
        }
    }
};

// RegenerationPotion: ����ҩˮ
class RegenerationPotion : public Potion {
public:
    RegenerationPotion() {
        name_ = "Regeneration Potion";
        description_ = "�ָ� 10 ������ֵ�������� 3 �غϻָ�";
        numeric_value_ = 10;
        money_cost_ = 70;
        rarity_ = 2;
        is_enabled_ = true;
    }

    void take_effect(Creature* target) override {
        // �ָ������������ָ�
      
    }

    void take_effect(vector<Creature*> targets) override {
        for (auto& target : targets) {
            
        }
    }
};

// EnergyPotion: ����ҩˮ
class EnergyPotion : public Potion {
public:
    EnergyPotion() {
        name_ = "Energy Potion";
        description_ = "�ָ� 2 ������";
        numeric_value_ = 2;
        money_cost_ = 50;
        rarity_ = 1;
        is_enabled_ = true;
    }

    void take_effect(Creature* target) override {
        // �ָ�����
        
    }

    void take_effect(vector<Creature*> targets) override {
        for (auto& target : targets) {
           
        }
    }
};





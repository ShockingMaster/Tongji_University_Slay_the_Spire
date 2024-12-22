#pragma once
#include<vector>
#include<iostream>

using namespace std;

class Buff;

class Creature
{
public:
    string name_;//当前血量
    string description_;//描述
    int health_;//当前生命值
    int max_health_;//最大生命值
    int block_;//当前格挡
    int can_end_turn_;//是否能够结束回合

    vector<std::shared_ptr<Buff>> buffs_;//生物buff列表

    std::string getName() const;

    void addBlock(int numeric_value);

    void loseBlock(int numeric_value);                    

    void loseHealth(int numeric_value);

    void addHealth(int numeric_value);

    int getBlockValue() const;

    int getHealth() const;

    int getMaxHealth() const;


    virtual void startTurn() {};

    virtual void endTurn() {};
};

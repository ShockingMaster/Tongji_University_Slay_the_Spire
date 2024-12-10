#pragma once
#include<vector>
#include<iostream>

using namespace std;

class Buff;

class Creature
{
public:
    
    string name_;
    
    string description_;
    
    int health_;
    
    int max_health_;
    
    int block_;
    
    int can_end_turn_;
    
    vector<Buff*> buffs_;
    
    void updateHealth();                                                    //需要调用本体的updateHealth()
    
    void updateBuff();                                                      //需要调用本体的updateBuff()
    
    void updateBlock();
    
    void addBuff(Buff* buff);

    void addBlock(int numeric_value);
    
    void takeDamage(int numeric_value);                                     //这个是即将受到伤害时进行的效果判定
    
    void show_info();
    
    void loseBlock(int numeric_value) {};
    
    void loseHealth(int numeric_value) {};
    
    void take_damage(int damage);
    
    int getBlockValue();
    
    virtual void startTurn() {};
    
    virtual void endTurn() {};
};

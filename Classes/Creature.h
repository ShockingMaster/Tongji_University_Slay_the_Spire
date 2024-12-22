#pragma once
#include<vector>
#include<iostream>

using namespace std;

class Buff;

class Creature
{
public:
    string name_;//��ǰѪ��
    string description_;//����
    int health_;//��ǰ����ֵ
    int max_health_;//�������ֵ
    int block_;//��ǰ��
    int can_end_turn_;//�Ƿ��ܹ������غ�

    vector<std::shared_ptr<Buff>> buffs_;//����buff�б�

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

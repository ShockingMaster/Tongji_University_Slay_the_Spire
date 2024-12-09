#pragma once
#include<vector>
#include "Creature.h"
using namespace std;
class Potion;

class Player : public Creature
{
public:

    string character_;
    int coins_;  
    static Player* getInstance();
    virtual void start_turn();
    virtual void end_turn();
    vector<Potion*> potions_;
    void addPotion(Potion* potion);
    void init();
    void addEnergy(int numeric_value_);
    int getEnergyValue();
    int getMaxEnergyValue();
private:
    static Player* instance_;

    int energy_;
    int maxEnergy_;
};

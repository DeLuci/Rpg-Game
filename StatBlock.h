#pragma once
using namespace std;

class StatBlock {
public:
    unsigned int Strength;
    unsigned int Dexterity;
    unsigned int Intellect;
    
    StatBlock(unsigned int s, unsigned int i, unsigned int d) {
        Strength = s;
        Intellect = i;
        Dexterity = d;
    }

    unsigned int getBaseStrength() { return   Strength; }
    unsigned int getBaseIntellect() { return  Intellect; }
    unsigned int getBaseDexterity() { return  Dexterity; }


    unsigned int getTotalStrength() { return   Strength; }
    unsigned int getTotalIntellect() { return  Intellect; }
    unsigned int getTotalDexterity() { return  Dexterity; }
protected:
    void increaseStats(unsigned int s, unsigned int i, unsigned int d) {
        Strength += s;
        Intellect += i;
        Dexterity += d;
    }
};

#pragma once
#include "PlayerCharacter.h"

class Mage : public CharacterInfo {
public:
	static const unsigned int  BASEHP = (unsigned int)10u;
	static const unsigned int  BASEMP = (unsigned int)14u;
	static const unsigned int BASESTA = (unsigned int)1u;
	static const unsigned int BASESTR = (unsigned int)1u;
	static const unsigned int BASEINT = (unsigned int)9u;
	static const unsigned int BASEAGI = (unsigned int)2u;
	// This constructor will set the stats for the user character.
	Mage() : CharacterInfo() {
    	HP->setMax(BASEHP);
    	HP->increaseCurrent(BASEHP);
    	MP->setMax(BASEMP); 
    	MP->increaseCurrent(BASEMP); 
    	STA->setMax(BASESTA); 
    	STA->increaseCurrent(BASESTA);
    	charName = "Mage";
    	increaseStats(BASESTR, BASEINT, BASEAGI);
	}
private:
// The function will increase the stats of the character by halve of the base stats
	void levelCharUp() noexcept {
    	HP->setMax((unsigned int)((BASEHP / 2.f) + HP->getMax()));
    	HP->increaseCurrent((unsigned int)(BASEHP / 2.f));
    	MP->setMax((unsigned int)((BASEMP / 2.f) + MP->getMax()));
    	MP->increaseCurrent((unsigned int)(BASEMP / 2.f));
	    STA->setMax((unsigned int)((BASESTA / 2.f) + STA->getMax()));
    	STA->increaseCurrent((unsigned int)(BASESTA / 2.f));
    	increaseStats((int)((BASESTR + 1u) / 2.f), (int)((BASEINT + 1u) / 2.f), (int)((BASEAGI + 1u) / 2.f));
		// this adds the ability to the vector Abilities
    	if (getLevel() == 2) {
        	Abilities.emplace_back("IceBolt", 7u, 4u, 0u);
    	}
	}
};
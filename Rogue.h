#pragma once
#include "PlayerCharacter.h"

class Rogue : public CharacterInfo {
public:
	static const unsigned int BASEHP = (unsigned int)14u;
	static const unsigned int BASEMP = (unsigned int)1u;
	static const unsigned int BASESTA = (unsigned int)10u;
	static const unsigned int BASESTR = (int)6u;
	static const unsigned int BASEINT = (int)3u;
	static const unsigned int BASEAGI = (int)4u;
	// This constructor will set the stats for the user character.
	Rogue() : CharacterInfo() {
		HP->setMax(BASEHP);
		HP->increaseCurrent(BASEHP);
		MP->setMax(BASEMP); 
		MP->increaseCurrent(BASEMP); 
		STA->setMax(BASESTA); 
		STA->increaseCurrent(BASESTA);
		charName = "Rogue";
		increaseStats(BASESTR, BASEINT, BASEAGI);
	}	
	
private:
	void levelCharUp() noexcept {
		HP->setMax((unsigned int)((BASEHP / 2.f) + HP->getMax()));
		HP->increaseCurrent((unsigned int)(BASEHP / 2.f));
		MP->setMax((unsigned int)((BASEMP / 2.f) + MP->getMax()));
		MP->increaseCurrent((unsigned int)(BASEMP / 2.f));
		STA->setMax((unsigned int)((BASESTA / 2.f) + STA->getMax()));
    	STA->increaseCurrent((unsigned int)(BASESTA / 2.f));
		increaseStats((int)((BASESTR + 1u) / 2.f), (int)((BASEINT + 1u) / 2.f), (int)((BASEAGI + 1u) / 2.f));

		if (getLevel() == 2) {
			Abilities.emplace_back("Back Stab", 5u, 0u, 3u);
		}
	}
};
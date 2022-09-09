#pragma once
#include "PlayerCharacter.h"

class Warrior : public CharacterInfo {
public:
	string chr_name;
	static const unsigned int BASEHP = (unsigned int)18u;
	static const unsigned int BASESTA = (unsigned int)10u;
	static const unsigned int BASEMP = (unsigned int)1u;
	static const unsigned int BASESTR = (unsigned int)6u;
	static const unsigned int BASEINT = (unsigned int)2u;
	static const unsigned int BASEAGI = (unsigned int)2u;
	// This constructor will set the stats for the user character.
	Warrior() : CharacterInfo() {
		HP->setMax(BASEHP);
		HP->increaseCurrent(BASEHP);
		charName = "Warrior";
		MP->setMax(BASEMP); 
		MP->increaseCurrent(BASEMP); 
		STA->setMax(BASESTA); 
		STA->increaseCurrent(BASESTA);
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

		increaseStats((unsigned int)((BASESTR + 1u) / 2.f), (unsigned int)((BASEINT + 1u) / 2.f), (unsigned int)((BASEAGI + 1u) / 2.f));

		if (getLevel() == 2) {
			Abilities.emplace_back("Sweep Attack", 4u, 0u, 3u);
		}
	}
};
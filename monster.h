#pragma once
#include "PointSystem.h"
#include <memory>
#include <stdlib.h>
#include <time.h>
using namespace std;

class Monster  {
public:
	PointSystem HP;
	// The attack system uses a random function to determine the damage 
	[[nodiscard]] const unsigned int Attack() const noexcept {
		srand((unsigned int)time(NULL));
		unsigned int _attack = (rand() %maxDmg) + minDmg;
		return _attack;
	};
	Monster(unsigned int hp, unsigned int min, unsigned int max) 
	: HP(hp, hp), minDmg(min), maxDmg(max) {}

	[[nodiscard]] const unsigned int getMinDamage() const noexcept {
		return minDmg;
	}

	[[nodiscard]] const unsigned int getMaxDamage() const noexcept {
		return maxDmg;
	}
	
private:
	unsigned int minDmg, maxDmg;
	Monster() = delete;
	Monster(const Monster&) = delete;
	Monster(const Monster&&) = delete;
};


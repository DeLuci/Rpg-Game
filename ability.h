#pragma once
#include <string>
using namespace std;

class Ability {
public:
    Ability(string name, uint32_t hpe, uint32_t _mpCost, uint32_t _staCost)
        : Name(name), hpEffect(hpe), mpCost(_mpCost), staCost(_staCost) {}

    const string getName() const noexcept {
        return Name;
    }
    
    const uint32_t getHealthEffect() const noexcept {
        return hpEffect;
    }

    const uint32_t getMPCost() const noexcept {
        return mpCost;
    }

    const uint32_t getSTACost() const noexcept {
        return staCost;
    }

    ~Ability() {}

    string Name;
    uint32_t hpEffect;
    uint32_t mpCost;      // assume mp if there is a mpCost
    uint32_t staCost;  // number of rounds you have to wait before using it again
};
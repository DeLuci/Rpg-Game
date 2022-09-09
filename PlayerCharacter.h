#pragma once
#include "StatBlock.h"
#include "PointSystem.h"
#include "ability.h"
#include <memory>
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
using namespace std;

class CharacterInfo : public StatBlock {
  public:
    static const unsigned int LEVEL2AT = 100u;

    CharacterInfo() : StatBlock(0, 0, 0) {
      CurrentLevel = (unsigned int)1u;
      CurrentEXP = (unsigned int)0u;
      charName = "Unknown";
      EXPToNextLevel = LEVEL2AT;
      HP = make_unique<PointSystem>(1u, 1u);
      MP = make_unique<PointSystem>(0u, 0u);
      STA = make_unique<PointSystem>(0u, 0u);
    }
    // Everytime you gain exp, the check_if_leveled() will trigger.
    void gainEXP(unsigned int gained_exp) {
      CurrentEXP += gained_exp;
      while (check_if_leveled()) {}
    }

    [[nodiscard]] unsigned int getLevel() {
      return CurrentLevel;
    }
    [[nodiscard]] unsigned int getCurrentEXP() {
      return CurrentEXP;
    }
    [[nodiscard]] unsigned int getEXPToNextLevel() {
      return EXPToNextLevel;
    }

    [[nodiscard]] string getName() {
      return charName;
    }

    void setName(string s) { 
    charName = s;
    }

    unique_ptr<PointSystem> HP;
    unique_ptr<PointSystem> MP;
    unique_ptr<PointSystem> STA;
    vector<Ability> Abilities;

    ~CharacterInfo() {}
  protected:
    string charName;
    unsigned int CurrentLevel;
    unsigned int CurrentEXP;
    unsigned int EXPToNextLevel;
    // If check_if_leveled() returns true then the levelCharUp() will pass and each character class will override the function
    virtual void levelCharUp() noexcept = 0;
    // The check_if_leveled() function will check if the gained exp is >= to the EXPToNextLevel
    [[nodiscard]] const bool check_if_leveled() noexcept{
      static const unsigned int LEVELSCALAR = 2u;
      if (CurrentEXP >= EXPToNextLevel) {
        CurrentLevel++;
        levelCharUp();
        EXPToNextLevel *= LEVELSCALAR; // The exp for next level will be multiplied by 2
        return true;
      }
    return false;
    }
};

// This is PlayerCharacter emcapsulates every function to one class.
class PlayerCharacter final {
    private:
      CharacterInfo* p_class;

      friend class ItemManager;

      PlayerCharacter() = delete;
      PlayerCharacter(const PlayerCharacter&) = delete;
      PlayerCharacter(const PlayerCharacter&&) = delete;

    public:
    // This is will point to characterInfo
      PlayerCharacter(CharacterInfo* pc) : p_class(pc) {}
      ~PlayerCharacter() {
        delete p_class;
        p_class = nullptr;
      }

        // Getters
      [[nodiscard]] const unsigned int getLevel() const noexcept { return p_class->getLevel(); }
      [[nodiscard]] const unsigned int getCurrentEXP() const noexcept{ return p_class->getCurrentEXP(); }
      [[nodiscard]] const unsigned int getEXPToNextLevel() const noexcept { return p_class->getEXPToNextLevel(); }
      [[nodiscard]] const unsigned int getCurrentHP() const noexcept{ return p_class->HP->getCurrent(); }
      [[nodiscard]] const unsigned int getMaxHP() const noexcept { return p_class->HP->getMax(); }
      [[nodiscard]] const string getClassName() const noexcept { return p_class->getName();}
      [[nodiscard]] const unsigned int getCurrentMP() const noexcept {
          if (p_class->MP) {
            return p_class->MP->getCurrent();
          }
          else {
            return 0;
          }  
      }
      [[nodiscard]] const unsigned int getMaxMP() const noexcept {
          if (p_class->MP) {
            return p_class->MP->getMax();
          }
          else {
            return 0;
          }
      }

      [[nodiscard]] const unsigned int getCurrentSTA() const noexcept {
          if (p_class->STA) {
            return p_class->STA->getCurrent();
          }
          else {
            return 0;
          }
      }
      [[nodiscard]] const unsigned int getMaxSTA() const noexcept {
          if (p_class->STA) {
            return p_class->STA->getMax();
          }
          else {
            return 0;
          }
      }
      [[nodiscard]] const unsigned int getBaseStrength() const noexcept { return p_class->getBaseStrength(); }
      [[nodiscard]] const unsigned int getBaseIntellect() const noexcept { return p_class->getBaseIntellect(); }
      [[nodiscard]] const unsigned int getBaseDexterity() const noexcept { return p_class->getBaseDexterity(); }
      // [[nodiscard]] const unsigned int meleeAttack() const noexcept {
      //   unsigned int tmp_dmg_done = 0;
      //   tmp_dmg_done = p_class->getBaseStrength() * 0.8;
      //   return tmp_dmg_done;
      // }    
      // [[nodiscard]] const unsigned int rangedAttack() const noexcept {
      //   unsigned int tmp_dmg_done = 0;
      //   tmp_dmg_done = p_class->getBaseIntellect() * 0.8;
      //   return tmp_dmg_done;
      // }
      [[nodiscard]] const unsigned int characterAttack() const noexcept {
        unsigned int tmp_dmg_done = 0;
        if (p_class->getName() == "Warrior" || p_class->getName() == "Rogue" ) {
          tmp_dmg_done = p_class->getBaseStrength() * 0.6;
        }
        else if (p_class->getName() == "Mage") {
          tmp_dmg_done = p_class->getBaseIntellect() * 0.6;
        }
        return tmp_dmg_done;
      }

      bool checkForAttribute() {
        if (p_class->getName() == "Mage" && p_class->MP->getCurrent() == 0) {
          return true;
        }
        else if ((p_class->getName() == "Warrior" || p_class->getName() == "Rogue" ) && p_class->STA->getCurrent() == 0) {
          return true;
        }
        return false;
      }

      void printNoAttribute() {
        if (p_class->getName() == "Mage" && p_class->MP->getCurrent() == 0) {
          cout << endl << "No Mana!" << endl;
					cout << "Using Basic Attack!" << endl;
        }
        else if ((p_class->getName() == "Warrior" || p_class->getName() == "Rogue" ) && p_class->STA->getCurrent() == 0) {
          cout << endl << "No Stamina!" << endl;
          cout << endl << "Using Basic Attack!" << endl;
        }
      }

      unsigned int calculateAbiliyDamage() {
        unsigned int total_damage;
        if (p_class->getName() == "Mage") {
					total_damage += ((p_class->getBaseIntellect() * 0.6) + p_class->Abilities[0].getHealthEffect() / 2);
				}
				else if (p_class->getName() == "Warrior" || p_class->getName() == "Rogue") {
					total_damage += ((p_class->getBaseStrength() * 0.4) + (p_class->getBaseDexterity() *0.3) 
						+ p_class->Abilities[0].getHealthEffect() / 2);
				}
        return total_damage;
      }

      void reduceAttribute() {
        if (p_class->getName() == "Mage") {
          p_class->MP->reduceCurrent(p_class->Abilities[0].getMPCost());
        }
        else if (p_class->getName() == "Warrior" || p_class->getName() == "Rogue") {
          p_class->STA->reduceCurrent(p_class->Abilities[0].getSTACost());
        }
      }

      // created a vector for abilities in case I wanted to add more abilities
      const vector<Ability> getAbilityList() const noexcept { return p_class->Abilities; }
      // Modifiers
      void gainEXP(unsigned int amt) noexcept { p_class->gainEXP(amt); }
      void takeDamage(unsigned int amt) noexcept { p_class->HP->reduceCurrent(amt); }
      void heal(unsigned int amt) noexcept { p_class->HP->increaseCurrent(amt); }
      void reduceMP(unsigned int amt) noexcept {p_class->MP->reduceCurrent(amt);}
      void reduceSTA(unsigned int amt) noexcept {p_class->STA->reduceCurrent(amt);}
};
#include "monster.h"
#include "PlayerCharacter.h"
#include "ability.h"
#include "StatBlock.h"
#include "Warrior.h"
#include "Rogue.h"
#include "Mage.h"
#include <iostream>
#include <stdlib.h>
#include <string>
#include <conio.h>
#include <fstream>
using namespace std;

// Decided to use these as globals, so I can use these variables in different functions.
string player_name;
string character_name;
int check_health_S = 0;
int check_health_K = 0;
int check_health_B = 0;
char c;

// This is map layout for the player to move in
char the_map1[13][26] = {
	"xxxxxxxxxxxxxxxxxxxxxxxxx",
	"x           xxxx        x",
	"x           xxxx        x",
	"x           xxxx        x",
	"x           xxxx        x",
	"x           xxxx        x",
	"x           xxxx        x",
	"xx xxxxxxxxxxxxxxxx xxxxx",
	"xx xxxxxxxxxxxxxxxx  xxxx",
	"xx xxxxxxxxxxxxxxxx  xxxx",
	"xx                   xxxx",
	"xxxxxxxxxxxxxxxxxxxxxxxxx",
	"xxxxxxxxxxxxxxxxxxxxxxxxx",
};


// Decided to keep this public so I created a struct. This struct creates a monster for the game.
struct MinionMonster {
	MinionMonster(string _name, int hp, int min, int max, int rpos, int cpos, int _expWorth) : monster(hp, min, max) {
		xpos = rpos;
		ypos = cpos;
		expWorth = _expWorth;
		name = _name;
	}
	Monster monster;
	bool isLiving() {return (monster.HP.getCurrent() > 0);} //bool to determine when the fight is over.
	int xpos;
	int ypos;
	int expWorth;
	string name;
	MinionMonster() = delete;
};

// its a way to "clear" the terminal when an action is taken
void clearScreen() {
	cout << string(50, '\n');
}

// its the same function as the top one, but this allows you to see the stats from the previous fight turn.
void battleScreen() {
	cout << string(2, '\n');
}

// this struct allows the user to create their character 
struct Player{
	Player(CharacterInfo* char_class) : player(char_class) {}
	Player() = delete;
	bool isLivin() {return (player.getCurrentHP() > 0);}
	PlayerCharacter player;
	int prev_xPos = 3;
	int prev_yPos = 3;
	int xPos = 3;
	int yPos = 3;
};

MinionMonster* skeleton = nullptr;
MinionMonster* kobold = nullptr;
Player* mainCharacter = nullptr;
MinionMonster* bowser = nullptr;

void playerMovementOnMap(Player& player1) {
	// check that the player didn't move into a wall
	if (player1.xPos == player1.prev_xPos && player1.yPos == player1.prev_yPos) {
		return;
	}
	if (the_map1[player1.xPos][player1.yPos] != 'x') {
		// draw the character at new location	
		the_map1[player1.xPos][player1.yPos] = 'P';
		// make old location a blank area 
		the_map1[player1.prev_xPos][player1.prev_yPos] = ' ';
		// update current location to be previous before next update
		player1.prev_xPos = player1.xPos;
		player1.prev_yPos = player1.yPos;
	}
	else {
		player1.xPos = player1.prev_xPos;
		player1.yPos = player1.prev_yPos;
	}
}

void checkCharacterStats(Player& player1) {
	clearScreen();
	int checking_stats;
	int option_choice;
	while(checking_stats == 0) {
		cout << endl << "Player Name: " << player_name << endl
			<< "Class Name: " << character_name << endl
    		<< "Level: " << mainCharacter->player.getLevel() << endl
			<< "Stats: " << endl
			<< "- HP: " << mainCharacter->player.getMaxHP() << endl
			<< "- MP: " << mainCharacter->player.getMaxMP() << endl
			<< "- STA: " << mainCharacter->player.getMaxSTA() << endl
			<< "- STR: " << mainCharacter->player.getBaseStrength() << endl
			<< "- INT: " << mainCharacter->player.getBaseIntellect() << endl
			<< "- DEX: " << mainCharacter->player.getBaseDexterity() << endl
			<< "Press 1 - to return to main menu." << endl;
		cin >> option_choice;
		switch(option_choice){
			case(1):
				checking_stats = 1;
				break;
			default:
				break;
		}
	}
} 

void savingGame(Player& player1) {
	ofstream saving_file;
	int skeleton_alive = 0;
	int kobold_alive = 0;
	int boss_alive = 0;
	if (check_health_S == 0) {
		skeleton_alive++;
	}
	if (check_health_K == 0) {
		kobold_alive++;
	}
	if (check_health_B == 0) {
		boss_alive++;
	}
	saving_file.open("Character_File.txt");
	saving_file << player_name << endl
		<< player1.player.getClassName() << endl
		<< player1.player.getLevel() << endl
		<< to_string(player1.xPos) << endl
		<< to_string(player1.yPos) << endl
		<< to_string(skeleton_alive) << endl
		<< to_string(kobold_alive) << endl
		<< to_string(boss_alive) << endl;
	cout << endl << "Saving File..." << endl;
	saving_file.close();
}


void createBattleSequence(Player& player1, MinionMonster& monsters) {
	clearScreen();
	// This is while loop will check if the player and monster is alive.
	while (player1.isLivin() && monsters.isLiving()) {
		int fight_choice = 0;
		// This while loop will allow the user to select their battle choice. At level 1, you dont have abilities.
		// So basic attacks is only thing you can use. But at level 2 you unlock one ability.
		while (fight_choice == 0) {
			int battle_choice = 0;
			battleScreen();
			cout << "  Player                 vs           " << monsters.name << endl;
			cout << "HP: " << player1.player.getCurrentHP() << " / " << player1.player.getMaxHP() << "                        HP: " << monsters.monster.HP.getCurrent() << " / " << monsters.monster.HP.getMax() << endl;
			cout << "MP: " << player1.player.getCurrentMP() << " / " << player1.player.getMaxMP() << "                        Min Damage: " << monsters.monster.getMinDamage() << " - Max Damage: " << monsters.monster.getMaxDamage() << endl;
			cout << "STA: " << player1.player.getCurrentSTA() << " / " << player1.player.getMaxSTA() << endl;
			cout << "Battle Options: 1 - attack, 2 - abilities" << endl;
			cin >> battle_choice;
			switch(battle_choice) {
				case(1): // Case 1 provides the basic attack scenario
					cout << endl << endl << "Using Basic Attack!" << endl;
					monsters.monster.HP.reduceCurrent(player1.player.characterAttack());
					fight_choice = 1;
					break;
				case(2):
					vector <Ability> allAbilities = player1.player.getAbilityList();
					if (player1.player.getLevel() == 1) {
						battleScreen();
						cout << endl << endl << " No abilities!" << endl;
						cout << "Using Basic Attack!" << endl;
						monsters.monster.HP.reduceCurrent(player1.player.characterAttack());
						fight_choice = 2;
						break;
					}
					else if (player1.player.getLevel() == 2) {
						// This checks if the characters have enough MP or STA to use their ability
						if (player1.player.checkForAttribute()) {
							player1.player.printNoAttribute();
							monsters.monster.HP.reduceCurrent(player1.player.characterAttack());
							break;
						}
						else {
							int fighting_choices = 0;
							while (fighting_choices == 0) {
								cout << endl << "Press 1 to use basic attack!" << endl;
								cout << "Press 2 to use abilities!" << endl;
								cout << endl << "Abilities:" << endl;
								int ability_choice = 0;
								int total_damage = 0;
								// vector for abilities - provides all the stats of the abilities.
								for (auto& elem : allAbilities) {
								cout << elem.getName() << "-> " 
									<< " Health Effect: " << elem.getHealthEffect() << "   Cost (MP): " << elem.getMPCost() 
									<< "  Cost (STA): " << elem.getSTACost() << endl;
								}
								cin >> ability_choice;
								switch(ability_choice) {
									case(1):
										cout << "Using basic Attack!!" << endl;
										monsters.monster.HP.reduceCurrent(player1.player.characterAttack());
										fight_choice = 2;
										fighting_choices = 1;
										break;
									case(2):
										monsters.monster.HP.reduceCurrent(player1.player.calculateAbiliyDamage());
										player1.player.reduceAttribute();
										fight_choice = 2;
										fighting_choices = 2;
										break;
									default:
										break;
								}
							}
						}
					}
				}
		} 
		if (monsters.monster.HP.getCurrent() > 0) {
			if (skeleton->isLiving()) {
				int damage_taking = skeleton->monster.Attack();
				cout << endl << "Monster Attacks You!" << endl;
				player1.player.takeDamage(damage_taking);
			}
			else if ((kobold->isLiving()) ) {
				int damage_taking = kobold->monster.Attack();
				cout << endl << "Monster Attacks You!" << endl;
				player1.player.takeDamage(damage_taking);
			}
			else if ((bowser->isLiving()) ) {
				int damage_taking = bowser->monster.Attack();
				cout << endl << "Monster Attacks You!" << endl;
				player1.player.takeDamage(damage_taking);
			}
		}
	}
	int dead_choice = 0;
	int press_choice;
	while (dead_choice == 0) {
		if (player1.isLivin() && !skeleton->isLiving()) {
			clearScreen();
			cout << "You won vs the Monster!\n";
			player1.player.gainEXP(skeleton->expWorth);
			cout << "exp gained: " << skeleton->expWorth << endl;
			player1.player.heal(player1.player.getMaxHP());
			cout << "Press 1 - To Continue..." << endl;
			check_health_S++;
			skeleton->xpos = 0;
			skeleton->ypos = 1;
		}
		if (player1.isLivin() && !kobold->isLiving()) {
			clearScreen();
			cout << "You won vs the Monster!\n";
			player1.player.gainEXP(kobold->expWorth);
			cout << "exp gained: " << kobold->expWorth << endl;
			player1.player.heal(player1.player.getMaxHP());
			if (player1.player.getLevel() == 2) {
				cout << "LEVEL UP!!!" << endl;
				cout << "Check your status window to see your stats!" << endl;
			}
			cout << "Press 1 - To Continue..." << endl;
			check_health_K++;
			kobold->xpos = 0;
			kobold->ypos = 2;
		}
		if (player1.isLivin() && !bowser->isLiving()) {
			clearScreen();
			cout << "You won vs the Monster!\n";
			player1.player.gainEXP(bowser->expWorth);
			cout << "exp gained: " << bowser->expWorth << endl;
			player1.player.heal(player1.player.getMaxHP());
			cout << "Press 1 - To Continue..." << endl;
			check_health_B++;
			bowser->xpos = 0;
			bowser->ypos = 3;
		}
		if (player1.player.getCurrentHP() == 0) {
			cout << "YOU DIED!!!" << endl;
			cout << "GAME OVER!!!!" << endl;
			cout << "EXITING PROGRAM..." << endl;
			exit(0);
		}
		cin >> press_choice;
		switch(press_choice) {
			case(1):
				dead_choice = 1;
				break;
			default:
				break;
		}
	}
}

void showmap() {
	clearScreen();
    for (int i = 0; i < 13; i++) {
        for (int j = 0; j < 26; j++) {
            cout << the_map1[i][j];
        }
        cout << endl;
    }
	cout << endl << "Save Game: v    End Game: m    Check Character Stats: u" << endl;
}


int main(int argc, char** argv) {
	int menu = 0;
	int counter = 0;
	int input_choice = 0;
	int choice = 0;
	int character_class = 0;
	int character_menu = 0;
	cout << "   WELCOME TO A BASIC RPG-GAME!  " << endl;
	while(menu == 0) {
		cout << "Press 1 - Load save file   Press 2 - Create New Character" << endl;
		cin >> input_choice;
		switch(input_choice) {
			case 1:
				cout << "Loading file..." << endl;
				menu = 1;
				break;
			case 2:
			// provides an overview the class stats
				cout << "Character Name: ";
				cin >> player_name;
				cout << endl;
				cout << "Select Character Class:" << endl;
				cout << "      1 - Warrior:" << endl
					<< "Uses a sword to attack monsters." << endl
					<< "     Basic Stats:" << endl
					<< "-Health Points (HP): 18" << endl
					<< "-Mana Points (MP): 0" << endl
					<< "-Stamina Points (STA): 10" << endl
					<< "-Strength (STR): 6" << endl
					<< "-Intelligence (INT): 2" << endl
					<< "-Agility (AGI): 2" << endl;
				cout << "---------------------------" << endl;
				cout << "      2 - Mage:" << endl
					<< "Uses magic to attack monsters." << endl
					<< "Basic Stats:" << endl
					<< "-Health Points (HP): 10" << endl
					<< "-Mana Points (MP): 14" << endl
					<< "-Stamina Points (STA): 0" << endl
					<< "-Strength (STR): 1" << endl
					<< "-Intelligence (INT): 9" << endl
					<< "-Agility (AGI): 2" << endl;
				cout << "---------------------------" << endl;
				cout << "      3 - Rogue:" << endl
					<< "Uses a danger to attack monsters." << endl
					<< "Basic Stats:" << endl
					<< "-Health Points (HP): 14" << endl
					<< "-Mana Points (MP): 0" << endl
					<< "-Stamina Points (STA): 10" << endl
					<< "-Strength (STR): 6" << endl
					<< "-Intelligence (INT): 3" << endl
					<< "-Agility (AGI): 4" << endl;
				// Gives the user the option to select their character.
				while(character_menu == 0) {
					if (counter > 0) { 
						cout << "Please select 1 - Warrior, 2 - Mage, 3 - Rogue!"<< endl;
					}
					cin >> character_class;
					switch(character_class) {
						case(1):
							mainCharacter = new Player(new Warrior());
							character_menu = 1;
							character_name = "Warrior";
							menu = 1;
							break;
						case(2):
							mainCharacter = new Player(new Mage());
							character_menu = 1;
							character_name = "Mage";
							menu = 1;
							break;
						case(3):
							mainCharacter = new Player(new Rogue());
							character_menu = 1;
							character_name = "Rogue";
							menu = 1;
							break;
						default:
							counter++;
							break;
					}
				}		
				break;
			case(0):
				menu == 0;
				break;
			default:
				break;
		}
	}

	if (input_choice == 1) {
		string line;
		ifstream myfile;
		vector<string> chrInfo;
		myfile.open("Character_File.txt");
		if (myfile.is_open()) {
			while (getline(myfile, line)) {
				chrInfo.push_back(line);
			}
			myfile.close();
		}
		else {
			cout << "Cannot open the save file!" << endl;
			cout << "Exiting game..." << endl;
			exit(0);
		}	
		player_name = chrInfo[0];
		character_name = chrInfo[1];
		int character_level = stoi(chrInfo[2]);
		int character_xposition = stoi(chrInfo[3]);
		int character_yposition = stoi(chrInfo[4]);
		int isSkeleton_alive = stoi(chrInfo[5]);
		int isKobold_alive = stoi(chrInfo[6]);
		int isBowser_alive = stoi(chrInfo[7]);
		
		if (character_name == "Warrior") {
			if (character_level == 1) {
				mainCharacter = new Player(new Warrior());
			}
			else if (character_level == 2) {
				mainCharacter = new Player(new Warrior());
				mainCharacter->player.gainEXP(100);
			}

		}
		else if (character_name == "Mage") {
			if (character_level == 1) {
				mainCharacter = new Player(new Mage());
			}
			else if (character_level == 2) {
				mainCharacter = new Player(new Mage());
				mainCharacter->player.gainEXP(100);
			}
		}
		else if (character_name == "Rogue") {
			if (character_level == 1) {
				mainCharacter = new Player(new Rogue());
			}
			else if (character_level == 2) {
				mainCharacter = new Player(new Rogue());
				mainCharacter->player.gainEXP(100);
			}
		}
		// if the monsters are alive, they will be place back in same location as last time
		// else they are going to placed outside the map. 
	
		if (isSkeleton_alive == 1) { 
			skeleton = new MinionMonster("skeleton", 8, 1, 3, 7, 2, 50);
			the_map1[skeleton->xpos][skeleton->ypos] = 'M';
		}
		else {
			skeleton = new MinionMonster("skeleton", 8, 1, 3, 13, 2, 50);
			the_map1[skeleton->xpos][skeleton->ypos] = 'M';
		}
		if (isKobold_alive == 1) {
			kobold = new MinionMonster("kobold", 11, 1, 4, 7, 19, 50);
			the_map1[kobold->xpos][kobold->ypos] = 'M';
		}
		else {
			kobold = new MinionMonster("kobold", 11, 1, 4, 13, 3, 50);
			the_map1[kobold->xpos][kobold->ypos] = 'M';
		}
		if (isBowser_alive == 1) {
			bowser = new MinionMonster("(BOSS) BOWSER", 18, 3, 8, 2, 20, 100);
			the_map1[bowser->xpos][bowser->ypos] = 'B';
		}
		else {
			bowser = new MinionMonster("(BOSS) BOWSER", 18, 3, 9, 13, 4, 100);
			the_map1[bowser->xpos][bowser->ypos] = 'B';
		}
		mainCharacter->xPos = character_xposition;
		cout << "Break";
		mainCharacter->yPos = character_yposition;
		the_map1[mainCharacter->xPos][mainCharacter->yPos] = 'P';
		
	}
	else if (input_choice == 2) {
		skeleton = new MinionMonster("skeleton", 8, 1, 3, 7, 2, 50);
		kobold = new MinionMonster("kobold", 11, 1, 4, 7, 19, 50);
		bowser = new MinionMonster("(BOSS) BOWSER", 18, 3, 9, 2, 20, 100);
		the_map1[bowser->xpos][bowser->ypos] = 'B';
		the_map1[skeleton->xpos][skeleton->ypos] = 'M';
		the_map1[kobold->xpos][kobold->ypos] = 'M';
		the_map1[mainCharacter->xPos][mainCharacter->yPos] = 'P';
	}

    bool running = true;

    showmap();	

    while(running) {
        cout << endl << "move (W/A/S/D): ";
		char c = getch();
		// player movement
        switch (c) {
			case 'w':
				mainCharacter->xPos--;
				break;
			case 'a':
				mainCharacter->yPos--;
				break;
			case 's':
				mainCharacter->xPos++;
				break;
			case 'd':
				mainCharacter->yPos++;
				break;
			case 'v':
				break;
			case 'u':
				break;
			case 'm':
				break;
			default:
				break;
		}
		if (c == 'u') {
			checkCharacterStats(*mainCharacter);
		}
		if (c == 'v') {
			savingGame(*mainCharacter);
		}
		if (c == 'm') {
			cout << endl << "Exiting Game..." << endl;
				delete mainCharacter;
				mainCharacter = nullptr;
				delete skeleton;
				skeleton = nullptr;
				delete kobold;
				kobold = nullptr;
				delete bowser;
				bowser = nullptr;
			exit(0);
		}
		// cin.clear();
		playerMovementOnMap(*mainCharacter);
		if (mainCharacter->xPos == skeleton->xpos && mainCharacter->yPos == skeleton->ypos) {
			createBattleSequence(*mainCharacter, *skeleton);
		}
		else if (mainCharacter->xPos == kobold->xpos && mainCharacter->yPos == kobold->ypos) {
			createBattleSequence(*mainCharacter, *kobold);
		}
		else if (mainCharacter->xPos == bowser->xpos && mainCharacter->yPos == bowser->ypos) {
			createBattleSequence(*mainCharacter, *bowser);
		}
        showmap();

    }
    return EXIT_SUCCESS;
}
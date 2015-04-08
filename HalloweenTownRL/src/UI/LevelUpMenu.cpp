/*
 * LevelUpMenu.cpp
 *
 *  Created on: Mar 28, 2015
 *      Author: josh
 */
#include <iostream>
#include "../main.hpp"
using namespace std;

LevelUpMenu::LevelUpMenu(){
	PopulateMenu(false);
}

void LevelUpMenu::PopulateMenu(bool saveGameExists){
	try{
		Clear();

		SetHeader("You leveled up! Pick an attribute to increase:");
		AddItem("Strength");
		AddItem("Constitution");
		AddItem("Agility");

	}
	catch(...){
		cerr << "An error occurred with LevelUpMenu::populateMenu"  << endl;
		throw 0;
	}
};

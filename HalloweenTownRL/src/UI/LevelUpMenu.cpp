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
	populateMenu(false);
}

void LevelUpMenu::populateMenu(bool saveGameExists){
	try{
		clear();

		setHeader("You leveled up! Pick an attribute to increase:");
		addItem("Strength");
		addItem("Constitution");
		addItem("Agility");

	}
	catch(...){
		cerr << "An error occurred with LevelUpMenu::populateMenu"  << endl;
		throw 0;
	}
};

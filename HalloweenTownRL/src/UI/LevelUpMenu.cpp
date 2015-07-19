/*
 * LevelUpMenu.cpp
 *
 *  Created on: Mar 28, 2015
 *      Author: josh
 */
#include <iostream>

#include "LevelUpMenu.hpp"
#include "MenuUI.hpp"

LevelUpMenu::LevelUpMenu(){
	Clear();

	SetHeader("You leveled up! Pick an attribute to increase:");
	AddItem("Strength");
	AddItem("Constitution");
	AddItem("Agility");
}


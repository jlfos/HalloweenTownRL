/*
 * LevelUpMenu.cpp
 *
 *  Created on: Mar 28, 2015
 *      Author: josh
 */
#include <iostream>

#include "LevelUpMenu.hpp"
#include "../LoggerWrapper.hpp"
#include "MenuUI.hpp"

LevelUpMenu::LevelUpMenu(){
	try {
		Clear();

		SetHeader("You leveled up! Pick an attribute to increase:");
		AddItem("Strength");
		AddItem("Constitution");
		AddItem("Agility");
	}
	catch (...) {
		LoggerWrapper::Error("An error occurred in LevelUpMenu::LevelUpMenu()");
		throw 0;
	}
}


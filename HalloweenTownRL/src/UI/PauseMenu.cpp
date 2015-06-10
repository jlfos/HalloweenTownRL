/*
 * PauseMenu.cpp
 *
 *  Created on: Mar 28, 2015
 *      Author: josh
 */
#include <iostream>
#include "Menu.hpp"
#include "PauseMenu.hpp"


PauseMenu::PauseMenu(){
	SetHeader("HalloweenTown");
}

void PauseMenu::PopulateMenu(bool saveGameExists){
		try{
			Clear();
			AddItem("New Game");
			if(saveGameExists){
				AddItem("Continue");
			}
			AddItem("Exit");

		}
		catch(...){
			std::cerr << "An error occurred with Menu::populateMenu"  << std::endl;
			throw 0;
		}
}


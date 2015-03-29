/*
 * PauseMenu.cpp
 *
 *  Created on: Mar 28, 2015
 *      Author: josh
 */
#include <iostream>
#include "../main.hpp"

using namespace std;

PauseMenu::PauseMenu(){
	setHeader("HalloweenTown");
}

void PauseMenu::populateMenu(bool saveGameExists){
		try{
			clear();
			addItem("New Game");
			if(saveGameExists){
				addItem("Continue");
			}
			addItem("Exit");

		}
		catch(...){
			cerr << "An error occurred with Menu::populateMenu"  << endl;
			throw 0;
		}
}


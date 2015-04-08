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
			cerr << "An error occurred with Menu::populateMenu"  << endl;
			throw 0;
		}
}


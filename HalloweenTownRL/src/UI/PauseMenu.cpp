/*
 * PauseMenu.cpp
 *
 *  Created on: Mar 28, 2015
 *      Author: josh
 */
#include <iostream>
#include "../LoggerWrapper.hpp"
#include "MenuUI.hpp"
#include "PauseMenu.hpp"


PauseMenu::PauseMenu(bool saveFileExists){

	PopulateMenu(saveFileExists);
}

void PauseMenu::PopulateMenu(bool saveGameExists){
		try{
			SetHeader("HalloweenTown");
			Clear();
			AddItem("New Game");
			if(saveGameExists){
				AddItem("Continue");
			}
			AddItem("Exit");

		}
		catch(...){
			LoggerWrapper::Error("An error occurred with PauseMenu::PopulateMenu");
			throw 0;
		}
}


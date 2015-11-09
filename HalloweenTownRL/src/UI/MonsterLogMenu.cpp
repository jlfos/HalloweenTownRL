/*
 * MonsterLog.cpp
 *
 *  Created on: Jun 30, 2015
 */
#include <iostream>
#include "../LoggerWrapper.hpp"
#include "MonsterLogMenu.hpp"
MonsterLogMenu::MonsterLogMenu(){

}

void MonsterLogMenu::PopulateMenu(bool hardMode){
	try{
		Clear();

		SetHeader("Select a monster name to open its log entry");
		AddItem("sewer mutant");
		AddItem("gremlin");
		AddItem("goblin");
		AddItem("?????");
	}
	catch(...){
		LoggerWrapper::Error("An error occurred in MonsterLogMenu::PopulateMenu");
	}
}



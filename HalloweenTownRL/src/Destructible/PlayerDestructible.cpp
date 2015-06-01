#include <iostream>
#include "../main.hpp"

PlayerDestructible::PlayerDestructible(float maxHp, float defense, const char *corpseName) :
    Destructible(maxHp, defense, 0, corpseName) {
}


void PlayerDestructible::Die(Actor *owner){
	try{
		engine.gui->PushMessage(TCODColor::red,"You died!");
		Destructible::Die(owner);
		engine.gameStatus=Engine::DEFEAT;
	}
	catch(...){
		std::cerr << "An error occurred in PlayerDestructible::Die";
		throw 0;
	}
}

void PlayerDestructible::Save(TCODZip &zip){
	try{
		zip.putInt(PLAYER);
		Destructible::Save(zip);
	}
	catch(...){
		std::cerr << "An error occurred in PlayerDestructible::Save" << std::endl;
		throw 0;
	}
}

void PlayerDestructible::Load(TCODZip &zip){
	try{
		Destructible::Load(zip);
	}
	catch(...){
		std::cerr << "An error occurred in PlayerDestructible::Load" << std::endl;
		throw 0;
	}
}

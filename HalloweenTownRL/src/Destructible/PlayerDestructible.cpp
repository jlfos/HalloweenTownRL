#include <iostream>
#include "../main.hpp"

using namespace std;

PlayerDestructible::PlayerDestructible(float maxHp, float defense, const char *corpseName) :
    Destructible(maxHp,defense,corpseName) {
}


void PlayerDestructible::die(Actor *owner){
	try{
		engine.gui->message(TCODColor::red,"You died!");
		Destructible::die(owner);
		engine.gameStatus=Engine::DEFEAT;
	}
	catch(...){
		cerr << "An error occurred in PlayerDestructible::die";
		throw 0;
	}
}

void PlayerDestructible::save(TCODZip &zip){
	try{
		zip.putInt(PLAYER);
		Destructible::save(zip);
	}
	catch(...){
		cerr << "An error occurred in PlayerDestructible::save" << endl;
		throw 0;
	}
}

void PlayerDestructible::load(TCODZip &zip){
	try{
		Destructible::load(zip);
	}
	catch(...){
		cerr << "An error occurred in PlayerDestructible::load" << endl;
		throw 0;
	}
}

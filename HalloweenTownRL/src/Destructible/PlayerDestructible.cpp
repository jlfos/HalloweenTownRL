#include <iostream>
#include "../main.hpp"

using namespace std;

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
		cerr << "An error occurred in PlayerDestructible::Die";
		throw 0;
	}
}

void PlayerDestructible::Save(TCODZip &zip){
	try{
		zip.putInt(PLAYER);
		Destructible::Save(zip);
	}
	catch(...){
		cerr << "An error occurred in PlayerDestructible::Save" << endl;
		throw 0;
	}
}

void PlayerDestructible::Load(TCODZip &zip){
	try{
		Destructible::Load(zip);
	}
	catch(...){
		cerr << "An error occurred in PlayerDestructible::Load" << endl;
		throw 0;
	}
}

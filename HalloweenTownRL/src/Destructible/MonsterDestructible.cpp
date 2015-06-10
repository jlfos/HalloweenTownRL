#include <iostream>
#include "libtcod.hpp"
#include "Destructible.hpp"
#include "../Engine.hpp"
#include "../UI/Gui.hpp"
#include "MonsterDestructible.hpp"


MonsterDestructible::MonsterDestructible(float maxHp, float defense, int experienceReward, const char *corpseName) :
    Destructible(maxHp, defense, experienceReward, corpseName) {
}


void MonsterDestructible::Load(TCODZip &zip){
	try{
		Destructible::Load(zip);
	}
	catch(...){
		std::cerr << "An error occurred in MonsterDestructible::Load";
		throw 0;
	}
}

void MonsterDestructible::Die(Actor *owner){
	try{
		engine.gui->PushMessage(TCODColor::lightGrey,"%s is dead",owner->name);
		Destructible::Die(owner);
	}
	catch(...){
		std::cerr << "An error occurred in MonsterDestructible::Die" << std::endl;
		throw 0;
	}
}

void MonsterDestructible::Save(TCODZip &zip){
	try{
		zip.putInt(MONSTER);
		Destructible::Save(zip);
	}
	catch(...){
		std::cerr << "An error occurred in MonsterDestructible::Save" << std::endl;
		throw 0;
	}
}

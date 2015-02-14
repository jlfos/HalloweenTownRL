#include <iostream>
#include "../main.hpp"

using namespace std;

MonsterDestructible::MonsterDestructible(float maxHp, float defense, const char *corpseName) :
    Destructible(maxHp,defense,corpseName) {
}


void MonsterDestructible::load(TCODZip &zip){
	try{
		Destructible::load(zip);
	}
	catch(...){
		cerr << "An error occurred in MonsterDestructible::load";
		throw 0;
	}
}

void MonsterDestructible::die(Actor *owner){
	try{
		engine.gui->message(TCODColor::lightGrey,"%s is dead",owner->name);
		Destructible::die(owner);
	}
	catch(...){
		cerr << "An error occurred in MonsterDestructible::die" << endl;
		throw 0;
	}
}

void MonsterDestructible::save(TCODZip &zip){
	try{
		zip.putInt(MONSTER);
		Destructible::save(zip);
	}
	catch(...){
		cerr << "An error occurred in MonsterDestructible::save" << endl;
		throw 0;
	}
}

#include <iostream>
#include "../main.hpp"

using namespace std;

MonsterDestructible::MonsterDestructible(float maxHp, float defense, int experienceReward, const char *corpseName) :
    Destructible(maxHp, defense, experienceReward, corpseName) {
}


void MonsterDestructible::Load(TCODZip &zip){
	try{
		Destructible::Load(zip);
	}
	catch(...){
		cerr << "An error occurred in MonsterDestructible::Load";
		throw 0;
	}
}

void MonsterDestructible::Die(Actor *owner){
	try{
		engine.gui->PushMessage(TCODColor::lightGrey,"%s is dead",owner->name);
		Destructible::Die(owner);
	}
	catch(...){
		cerr << "An error occurred in MonsterDestructible::Die" << endl;
		throw 0;
	}
}

void MonsterDestructible::Save(TCODZip &zip){
	try{
		zip.putInt(MONSTER);
		Destructible::Save(zip);
	}
	catch(...){
		cerr << "An error occurred in MonsterDestructible::Save" << endl;
		throw 0;
	}
}

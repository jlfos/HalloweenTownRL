#include "../main.hpp"

MonsterDestructible::MonsterDestructible(float maxHp, float defense, const char *corpseName) :
    Destructible(maxHp,defense,corpseName) {
}


void MonsterDestructible::load(TCODZip &zip){
	Destructible::load(zip);
}

void MonsterDestructible::die(Actor *owner){
	engine.gui->message(TCODColor::lightGrey,"%s is dead",owner->name);
	Destructible::die(owner);
}

void MonsterDestructible::save(TCODZip &zip){
	zip.putInt(MONSTER);
	Destructible::save(zip);
}

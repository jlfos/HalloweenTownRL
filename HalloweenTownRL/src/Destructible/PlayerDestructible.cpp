#include "../main.hpp"

PlayerDestructible::PlayerDestructible(float maxHp, float defense, const char *corpseName) :
    Destructible(maxHp,defense,corpseName) {
}


void PlayerDestructible::die(Actor *owner){
	engine.gui->message(TCODColor::red,"You died!");
	Destructible::die(owner);
	engine.gameStatus=Engine::DEFEAT;
}

void PlayerDestructible::save(TCODZip &zip){
	zip.putInt(PLAYER);
	Destructible::save(zip);
}

void PlayerDestructible::load(TCODZip &zip){
	Destructible::load(zip);
}

#include <iostream>
#include <string>
#include "libtcod.hpp"
#include "../Actor/Actor.hpp"
#include "../Engine.hpp"
#include "../UI/Gui.hpp"
#include "Destructible.hpp"
#include "PlayerDestructible.hpp"
#include "../Tile/TileColors.hpp"


PlayerDestructible::PlayerDestructible(float maxHp, float defense, std::string corpseName) :
    Destructible(maxHp, defense, 0, corpseName) {
}


void PlayerDestructible::Die(Actor *owner){
	try{
		engine.gui->PushMessage(TileColors::red,"You died! Press ESC to continue.");
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

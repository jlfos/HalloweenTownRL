#include <iostream>
#include <string>
#include "libtcod.hpp"
#include "../Actor/Actor.hpp"
#include "Destructible.hpp"
#include "../Engine.hpp"
#include "../UI/Gui.hpp"
#include "../LoggerWrapper.hpp"
#include "MonsterDestructible.hpp"
#include "../Tile/TileColors.hpp"


MonsterDestructible::MonsterDestructible(float maxHp, float defense, int experienceReward, std::string corpseName, TCODColor corpseColor, int corpseCharacter) :
	Destructible(maxHp, defense, experienceReward, corpseName, corpseColor, corpseCharacter) {

}

MonsterDestructible::MonsterDestructible(float maxHp, float defense, int experienceReward, std::string corpseName, TCODColor corpseColor) :
	Destructible(maxHp, defense, experienceReward, corpseName, corpseColor) {

}


MonsterDestructible::MonsterDestructible(float maxHp, float defense, int experienceReward, std::string corpseName) :
    Destructible(maxHp, defense, experienceReward, corpseName) {
}




void MonsterDestructible::Load(TCODZip &zip){
	try{
		Destructible::Load(zip);
	}
	catch(...){
		LoggerWrapper::Error("An error occurred in MonsterDestructible::Load");
		throw 0;
	}
}

void MonsterDestructible::Die(Actor *owner){
	try{
		engine.gui->PushMessage(TileColors::greyLight,"The %s is dead", (owner->name).c_str() );
		Destructible::Die(owner);
	}
	catch(...){
		LoggerWrapper::Error("An error occurred in MonsterDestructible::Die");
		throw 0;
	}
}

void MonsterDestructible::Save(TCODZip &zip){
	try{
		zip.putInt(MONSTER);
		Destructible::Save(zip);
	}
	catch(...){
		LoggerWrapper::Error("An error occurred in MonsterDestructible::Save");
		throw 0;
	}
}

/*
 * BossDestructible.cpp
 *
 *  Created on: Jul 2, 2015
 */
#include <iostream>
#include <string>
#include "../Actor/Actor.hpp"
#include "BossDestructible.hpp"
#include "Destructible.hpp"
#include "../Engine.hpp"
#include "../UI/Gui.hpp"
#include "../LoggerWrapper.hpp"
#include "../Tile/TileColors.hpp"

BossDestructible::BossDestructible(float maxHp, float defense, int exp, std::string corpseName) :
		Destructible(maxHp, defense, exp, corpseName){
}

void BossDestructible::Die(Actor* owner){
	try{
		engine.gui->PushMessage(TileColors::red,"The %s has died!", owner->name.c_str());
		Destructible::Die(owner);
		engine.gameStatus=Engine::VICTORY;
	}
	catch(...){
		LoggerWrapper::Error("An error occurred in BossDestructible::Die");
		throw 0;
	}
}

void BossDestructible::Save(TCODZip &zip){
	try{
		zip.putInt(PLAYER);
		Destructible::Save(zip);
	}
	catch(...){
		LoggerWrapper::Error("An error occurred in BossDestructible::Save");
		throw 0;
	}
}

void BossDestructible::Load(TCODZip &zip){
	try{
		Destructible::Load(zip);
	}
	catch(...){
		LoggerWrapper::Error("An error occurred in BossDestructible::Load");
		throw 0;
	}
}




/*
 * MeleeWeapon.cpp
 *
 *  Created on: Jul 1, 2015
 */




#include <iostream>
#include "MeleeWeapon.hpp"
#include "../Ai/Ai.hpp"
#include "../Attacker.hpp"
#include "../Actor/Actor.hpp"
#include "../Destructible/Destructible.hpp"
#include "../Engine.hpp"
#include "../UI/Gui.hpp"
#include "Item.hpp"
#include "../Tile/TileColors.hpp"
MeleeWeapon::MeleeWeapon(float amount) : Item(ItemType::MELEE_WEAPON), amount(amount){

}

MeleeWeapon::~MeleeWeapon(){

}

bool MeleeWeapon::Use(Actor *owner, Actor *wearer){
	try{
		if(wearer->attacker){
			//TODO move this out
			engine.gui->PushMessage(TileColors::grey,"You equipped the %s.", owner->name.c_str() );
			wearer->attacker->setBasePower(amount);
			wearer->attacker->setWeapon(owner->name);
			return true;
		}
		return false;
	}
	catch(...){
		std::cerr << "An error occurred with MeleeWeapon::Use" ;
		throw 0;
	}
}

void MeleeWeapon::Load(TCODZip &zip){
	try{
		amount=zip.getFloat();
	}
	catch(...){
		std::cerr << "An error occurred with MeleeWeapon::Load"  << std::endl;
		throw 0;
	}
}

void MeleeWeapon::Save(TCODZip &zip){
	try{
		zip.putInt(ItemType::MELEE_WEAPON);
		zip.putFloat(amount);
	}
	catch(...){
		std::cerr << "An error occurred with MeleeWeapon::Save"  << std::endl;
		throw 0;
	}
}

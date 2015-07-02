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
#include "Pickable.hpp"
MeleeWeapon::MeleeWeapon(float amount) : amount(amount){

}

MeleeWeapon::~MeleeWeapon(){

}

bool MeleeWeapon::Use(Actor *owner, Actor *wearer){
	try{
		if(wearer->attacker){
			wearer->attacker->SetPower(amount);
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
		zip.putInt(PickableType::MELEE_WEAPON);
		zip.putFloat(amount);
	}
	catch(...){
		std::cerr << "An error occurred with MeleeWeapon::Save"  << std::endl;
		throw 0;
	}
}

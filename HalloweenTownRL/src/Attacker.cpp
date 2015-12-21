#include <iostream>
#include "Actor/Actor.hpp"
#include "Attacker.hpp"
#include "Destructible/Destructible.hpp"
#include "Engine.hpp"
#include "UI/Gui.hpp"
#include "LoggerWrapper.hpp"
#include "Tile/TileColors.hpp"

Attacker::Attacker(float basePower) :Attacker(basePower, "") {

}

Attacker::Attacker(float basePower, std::string weaponName):basePower(basePower), modifierPower(0), weapon(weaponName){

}

void Attacker::Attack(Actor *owner, Actor *target){
	try{
		if(target->destructible && ! target->destructible->IsDead()){
			if(getAttackPower() - target->destructible->GetDefense()>0){
				engine.gui->PushMessage(owner==engine.player ? TileColors::red : TileColors::greyLight,
					"%s attacks %s for %g hit points.", (owner->name).c_str(), (target->name).c_str(),
					getAttackPower() - target->destructible->GetDefense());
			}
			else{
				engine.gui->PushMessage(TileColors::greyLight,
					"%s attacks %s but it has no effect!", (owner->name).c_str(), (target->name).c_str());
			}
			target->destructible->TakeDamage(target, getAttackPower());
		}
		else{
			engine.gui->PushMessage(TileColors::greyLight,
				"%s attacks %s in vain.", (owner->name).c_str(), (target->name).c_str());
		}
	}
	catch(...){
		LoggerWrapper::Error("An error occurred with Attacker::Attack");
		throw 0;
	}
}

void Attacker::Load(TCODZip &zip){
	try{
		basePower=zip.getFloat();
	}
	catch(...){
		LoggerWrapper::Error("An error occurred with Attacker::Load");
		throw 0;
	}
}

void Attacker::setBasePower(float basePower){
	this->basePower = basePower;
}

float Attacker::getBasePower(){
	return basePower;
}

float Attacker::getModifierPower(){
	return modifierPower;
}

void Attacker::setModifierPower(float modifierPower){
	this->modifierPower = modifierPower;
}
float Attacker::getAttackPower(){
	return basePower + modifierPower;
}

void Attacker::setWeapon(std::string weapon){
	this->weapon = weapon;
}

std::string Attacker::getWeapon(){
	return weapon;
}

void Attacker::Save(TCODZip &zip){
	try{
		zip.putFloat(basePower);
	}
	catch(...){
		LoggerWrapper::Error("An error occurred with Attacker::Save");
		throw 0;
	}
}

#include <iostream>
#include "Actor/Actor.hpp"
#include "Attacker.hpp"
#include "Destructible/Destructible.hpp"
#include "Engine.hpp"
#include "UI/Gui.hpp"
#include "Tile/TileColors.hpp"

Attacker::Attacker(float basePower) :Attacker(basePower) {

}

Attacker::Attacker(float basePower, std::string weaponName):basePower(basePower), modifierPower(0), weapon(weaponName){

}

void Attacker::Attack(Actor *owner, Actor *target){
	try{
		if(target->destructible && ! target->destructible->IsDead()){
			if(basePower - target->destructible->GetDefense()>0){
				engine.gui->PushMessage(owner==engine.player ? TileColors::red : TileColors::lightGrey,
					"%s attacks %s for %g hit points.", (owner->name).c_str(), (target->name).c_str(),
					basePower - target->destructible->GetDefense());
			}
			else{
				engine.gui->PushMessage(TileColors::lightGrey,
					"%s attacks %s but it has no effect!", (owner->name).c_str(), (target->name).c_str());
			}
			target->destructible->TakeDamage(target, basePower);
		}
		else{
			engine.gui->PushMessage(TileColors::lightGrey,
				"%s attacks %s in vain.", (owner->name).c_str(), (target->name).c_str());
		}
	}
	catch(...){
		std::cerr << "An error occurred with Attacker::Attack"  << std::endl;
		throw 0;
	}
}

void Attacker::Load(TCODZip &zip){
	try{
		basePower=zip.getFloat();
	}
	catch(...){
		std::cerr << "An error occurred with Attacker::Load"  << std::endl;
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
		std::cerr << "An error occurred with Attacker::Save"  << std::endl;
		throw 0;
	}
}

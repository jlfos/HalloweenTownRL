#include <iostream>
#include "Actor/Actor.hpp"
#include "Attacker.hpp"
#include "Destructible/Destructible.hpp"
#include "Engine.hpp"
#include "UI/Gui.hpp"
#include "Tile/TileColors.hpp"

Attacker::Attacker(float power) :power(power){
}

void Attacker::Attack(Actor *owner, Actor *target){
	try{
		if(target->destructible && ! target->destructible->IsDead()){
			if(power - target->destructible->GetDefense()>0){
				engine.gui->PushMessage(owner==engine.player ? TileColors::red : TileColors::lightGrey,
					"%s attacks %s for %g hit points.", owner->name, target->name,
					power - target->destructible->GetDefense());
			}
			else{
				engine.gui->PushMessage(TileColors::lightGrey,
					"%s attacks %s but it has no effect!", owner->name, target->name);
			}
			target->destructible->TakeDamage(target, power);
		}
		else{
			engine.gui->PushMessage(TileColors::lightGrey,
				"%s attacks %s in vain.",owner->name, target->name);
		}
	}
	catch(...){
		std::cerr << "An error occurred with Attacker::Attack"  << std::endl;
		throw 0;
	}
}

void Attacker::Load(TCODZip &zip){
	try{
		power=zip.getFloat();
	}
	catch(...){
		std::cerr << "An error occurred with Attacker::Load"  << std::endl;
		throw 0;
	}
}

void Attacker::SetPower(float power){
	this->power = power;
}

float Attacker::GetPower(){
	return power;
}

void Attacker::Save(TCODZip &zip){
	try{
		zip.putFloat(power);
	}
	catch(...){
		std::cerr << "An error occurred with Attacker::Save"  << std::endl;
		throw 0;
	}
}

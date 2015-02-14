#include <iostream>
#include "main.hpp"

using namespace std;

Attacker::Attacker(float power) :power(power){
}

void Attacker::attack(Actor *owner, Actor *target){
	try{
		if(target->destructible && ! target->destructible->isDead()){
			if(power - target->destructible->defense>0){
				engine.gui->message(owner==engine.player ? TCODColor::red : TCODColor::lightGrey,
					"%s attacks %s for %g hit points.", owner->name, target->name,
					power - target->destructible->defense);
			}
			else{
				engine.gui->message(TCODColor::lightGrey,
					"%s attacks %s but it has no effect!", owner->name, target->name);
			}
			target->destructible->takeDamage(target, power);
		}
		else{
			engine.gui->message(TCODColor::lightGrey,
				"%s attacks %s in vain.",owner->name,target->name);
		}
	}
	catch(...){
		cerr << "An error occurred with Attacker::attack"  << endl;
		throw 0;
	}
}

void Attacker::load(TCODZip &zip){
	try{
		power=zip.getFloat();
	}
	catch(...){
		cerr << "An error occurred with Attacker::load"  << endl;
		throw 0;
	}
}

void Attacker::save(TCODZip &zip){
	try{
		zip.putFloat(power);
	}
	catch(...){
		cerr << "An error occurred with Attacker::save"  << endl;
		throw 0;
	}
}

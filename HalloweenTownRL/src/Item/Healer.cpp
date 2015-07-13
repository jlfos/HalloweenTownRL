#include <iostream>
#include "Healer.hpp"
#include "../Ai/Ai.hpp"
#include "../Actor/Actor.hpp"
#include "../Destructible/Destructible.hpp"
#include "Item.hpp"
Healer::Healer(float amount) : Item(ItemType::HEALER), amount(amount){
}

Healer::~Healer(){

}

bool Healer::Use(Actor *owner, Actor *wearer){
	try{
		if(wearer->destructible){
			float amountHealed = wearer->destructible->Heal(amount);
			if(amountHealed > 0){
				return Item::Use(owner, wearer);
			}
		}
		return false;
	}
	catch(...){
		std::cerr << "An error occurred with Healer::Use" ;
		throw 0;
	}
}

void Healer::Load(TCODZip &zip){
	try{
		amount=zip.getFloat();
	}
	catch(...){
		std::cerr << "An error occurred with Healer::Load"  << std::endl;
		throw 0;
	}
}

void Healer::Save(TCODZip &zip){
	try{
		zip.putInt(HEALER);
		zip.putFloat(amount);
	}
	catch(...){
		std::cerr << "An error occurred with Healer::Save"  << std::endl;
		throw 0;
	}
}

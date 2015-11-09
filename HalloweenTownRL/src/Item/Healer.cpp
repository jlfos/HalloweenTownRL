#include <iostream>
#include "Healer.hpp"
#include "../Ai/Ai.hpp"
#include "../Actor/Actor.hpp"
#include "../Destructible/Destructible.hpp"
#include "Item.hpp"
#include "../LoggerWrapper.hpp"

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
		LoggerWrapper::Error("An error occurred with Healer::Use");
		throw 0;
	}
}

void Healer::Load(TCODZip &zip){
	try{
		amount=zip.getFloat();
	}
	catch(...){
		LoggerWrapper::Error("An error occurred with Healer::Load");
		throw 0;
	}
}

void Healer::Save(TCODZip &zip){
	try{
		zip.putInt(HEALER);
		zip.putFloat(amount);
	}
	catch(...){
		LoggerWrapper::Error("An error occurred with Healer::Save");
		throw 0;
	}
}

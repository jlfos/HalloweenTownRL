#include <iostream>
#include "../main.hpp"

using namespace std;

Healer::Healer(float amount) : amount(amount){

}

Healer::~Healer(){

}

bool Healer::Use(Actor *owner, Actor *wearer){
	try{
		if(wearer->destructible){
			float amountHealed = wearer->destructible->Heal(amount);
			if(amountHealed > 0){
				return Pickable::Use(owner, wearer);
			}
		}
		return false;
	}
	catch(...){
		cerr << "An error occurred with Healer::Use" ;
		throw 0;
	}
}

void Healer::Load(TCODZip &zip){
	try{
		amount=zip.getFloat();
	}
	catch(...){
		cerr << "An error occurred with Healer::Load"  << endl;
		throw 0;
	}
}

void Healer::Save(TCODZip &zip){
	try{
		zip.putInt(HEALER);
		zip.putFloat(amount);
	}
	catch(...){
		cerr << "An error occurred with Healer::Save"  << endl;
		throw 0;
	}
}

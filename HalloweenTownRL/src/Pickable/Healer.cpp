#include <iostream>
#include "../main.hpp"

using namespace std;

Healer::Healer(float amount) : amount(amount){

}

bool Healer::use(Actor *owner, Actor *wearer){
	try{
		if(wearer->destructible){
			float amountHealed = wearer->destructible->heal(amount);
			if(amountHealed > 0){
				return Pickable::use(owner, wearer);
			}
		}
		return false;
	}
	catch(...){
		cerr << "An error occurred with Healer::use" ;
		throw 0;
	}
}

void Healer::load(TCODZip &zip){
	try{
		amount=zip.getFloat();
	}
	catch(...){
		cerr << "An error occurred with Healer::load"  << endl;
		throw 0;
	}
}

void Healer::save(TCODZip &zip){
	try{
		zip.putInt(HEALER);
		zip.putFloat(amount);
	}
	catch(...){
		cerr << "An error occurred with Healer::save"  << endl;
		throw 0;
	}
}

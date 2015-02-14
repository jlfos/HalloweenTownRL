#include <iostream>
#include "../main.hpp"

using namespace std;

bool Pickable::pick(Actor *owner, Actor *wearer){
	try{
		if(wearer->container && wearer->container->add(owner)){
			engine.actors.remove(owner);
			return true;
		}
		return false;
	}
	catch(...){
		cerr << "An error occurred with Pickable::pick"  << endl;
		throw 0;
	}
}

bool Pickable::use(Actor *owner, Actor *wearer){
	try
	{
		if(wearer->container){
			wearer->container->remove(owner);
			delete owner;
			return true;
		}
		return false;
	}
	catch(...){
		cerr << "An error occurred with Pickable::use"  << endl;
		throw 0;
	}
}


Pickable *Pickable::create(TCODZip &zip){
	try{
		PickableType type = (PickableType)zip.getInt();
		Pickable *pickable=nullptr;
		switch(type){
			case HEALER: pickable = new Healer(0); break;
		}
		pickable->load(zip);
		return pickable;
	}
	catch(...){
		cerr << "An error occurred with Pickable::create"  << endl;
		throw 0;
	}
}

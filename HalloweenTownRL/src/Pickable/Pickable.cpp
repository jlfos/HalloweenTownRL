#include "../main.hpp"


bool Pickable::pick(Actor *owner, Actor *wearer){
	if(wearer->container && wearer->container->add(owner)){
		engine.actors.remove(owner);
		return true;
	}
	return false;
}

bool Pickable::use(Actor *owner, Actor *wearer){
	if(wearer->container){
		wearer->container->remove(owner);
		delete owner;
		return true;
	}
	return false;
}


Pickable *Pickable::create(TCODZip &zip){
	PickableType type = (PickableType)zip.getInt();
	Pickable *pickable=nullptr;
	switch(type){
		case HEALER: pickable = new Healer(0); break;
	}
	pickable->load(zip);
	return pickable;
}

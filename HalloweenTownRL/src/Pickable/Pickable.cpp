#include <iostream>

#include "../Actor/Actor.hpp"
#include "../Container.hpp"
#include "../Engine.hpp"
#include "Healer.hpp"
#include "Pickable.hpp"

bool Pickable::Pick(Actor *owner, Actor *wearer){
	try{
		if(wearer->container && wearer->container->Add(owner)){
			engine.actors.remove(owner);
			return true;
		}
		return false;
	}
	catch(...){
		std::cerr << "An error occurred with Pickable::Pick"  << std::endl;
		throw 0;
	}
}

bool Pickable::Use(Actor *owner, Actor *wearer){
	try
	{
		if(wearer->container){
			wearer->container->Remove(owner);
			delete owner;
			return true;
		}
		return false;
	}
	catch(...){
		std::cerr << "An error occurred with Pickable::Use"  << std::endl;
		throw 0;
	}
}


Pickable *Pickable::Create(TCODZip &zip){
	try{
		PickableType type = (PickableType)zip.getInt();
		Pickable *pickable=nullptr;
		switch(type){
			case HEALER: pickable = new Healer(0); break;
		}
		pickable->Load(zip);
		return pickable;
	}
	catch(...){
		std::cerr << "An error occurred with Pickable::Create"  << std::endl;
		throw 0;
	}
}

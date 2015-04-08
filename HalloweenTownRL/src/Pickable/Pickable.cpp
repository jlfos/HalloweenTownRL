#include <iostream>
#include "../main.hpp"

using namespace std;

bool Pickable::Pick(Actor *owner, Actor *wearer){
	try{
		if(wearer->container && wearer->container->Add(owner)){
			engine.actors.remove(owner);
			return true;
		}
		return false;
	}
	catch(...){
		cerr << "An error occurred with Pickable::Pick"  << endl;
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
		cerr << "An error occurred with Pickable::Use"  << endl;
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
		cerr << "An error occurred with Pickable::Create"  << endl;
		throw 0;
	}
}

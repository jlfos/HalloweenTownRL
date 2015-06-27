#include <iostream>
#include "libtcod.hpp"
#include "Actor/Actor.hpp"
#include "Container.hpp"
#include "Tile/TileColors.hpp"

Container::Container(int size): size(size), inventory(0){
	try{

	}
	catch(...){
		std::cerr << "An error occurred with Container::Container"  << std::endl;
		throw 0;
	}
}

Container::~Container(){
	try{
		inventory.clearAndDelete();
	}
	catch(...){
		std::cerr << "An error occurred with Container::~Container"  << std::endl;
		throw 0;
	}
}

bool Container::Add(Actor *actor){
	try{
		if(size>0 && inventory.size() >= size){
			return false;
		}
		inventory.push(actor);
		return true;
	}
	catch(...){
		std::cerr << "An error occurred with Container::Add"  << std::endl;
		throw 0;
	}
}

void Container::Remove(Actor *actor){
	try{
	inventory.remove(actor);
	}
	catch(...){
		std::cerr << "An error occurred with Container::Remove" << std::endl;
		throw 0;
	}
}

void Container::Load(TCODZip &zip){
	try{
		size = zip.getInt();
		int nbActors = zip.getInt();
		while(nbActors > 0){
			Actor *actor = new Actor(0,0,0,nullptr, TileColors::white);
			actor->Load(zip);
			inventory.push(actor);
			nbActors--;
		}
	}
	catch(...){
		std::cerr << "An error occurred with Container::Load"  << std::endl;
		throw 0;
	}
}


void Container::Save(TCODZip &zip){
	try{
		zip.putInt(size);
		zip.putInt(inventory.size());
		for(Actor *it : inventory){
			(it)->Save(zip);
		}
	}
	catch(...){
		std::cerr << "An error occurred with Container::Save"  << std::endl;
		throw 0;
	}
}

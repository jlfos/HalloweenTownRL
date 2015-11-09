#include <iostream>
#include "libtcod.hpp"
#include "Actor/Actor.hpp"
#include "Container.hpp"
#include "LoggerWrapper.hpp"
#include "Tile/TileColors.hpp"

Container::Container(int size): size(size), inventory(0){
	try{

	}
	catch(...){
		LoggerWrapper::Error("An error occurred with Container::Container");
		throw 0;
	}
}

Container::~Container(){
	try{
		inventory.clearAndDelete();
	}
	catch(...){
		LoggerWrapper::Error("An error occurred with Container::~Container");
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
		LoggerWrapper::Error("An error occurred with Container::Add");
		throw 0;
	}
}

void Container::Remove(Actor *actor){
	try{
	inventory.remove(actor);
	}
	catch(...){
		LoggerWrapper::Error("An error occurred with Container::Remove");
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
		LoggerWrapper::Error("An error occurred with Container::Load");
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
		LoggerWrapper::Error("An error occurred with Container::Save");
		throw 0;
	}
}

#include <iostream>
#include "main.hpp"

using namespace std;

Container::Container(int size): size(size), inventory(0){
	try{

	}
	catch(...){
		cerr << "An error occurred with Container::Container"  << endl;
		throw 0;
	}
}

Container::~Container(){
	try{
		inventory.clearAndDelete();
	}
	catch(...){
		cerr << "An error occurred with Container::~Container"  << endl;
		throw 0;
	}
}

bool Container::add(Actor *actor){
	try{
		if(size>0 && inventory.size() >= size){
			return false;
		}
		inventory.push(actor);
		return true;
	}
	catch(...){
		cerr << "An error occurred with Container::add"  << endl;
		throw 0;
	}
}

void Container::remove(Actor *actor){
	try{
	inventory.remove(actor);
	}
	catch(...){
		cerr << "An error occurred with Container::remove" << endl;
		throw 0;
	}
}

void Container::load(TCODZip &zip){
	try{
		size = zip.getInt();
		int nbActors = zip.getInt();
		while(nbActors > 0){
			Actor *actor = new Actor(0,0,0,nullptr, TCODColor::white);
			actor->load(zip);
			inventory.push(actor);
			nbActors--;
		}
	}
	catch(...){
		cerr << "An error occurred with Container::load"  << endl;
		throw 0;
	}
}


void Container::save(TCODZip &zip){
	try{
		zip.putInt(size);
		zip.putInt(inventory.size());
		for(Actor *it : inventory){
			(it)->save(zip);
		}
	}
	catch(...){
		cerr << "An error occurred with Container::save"  << endl;
		throw 0;
	}
}

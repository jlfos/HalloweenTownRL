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

bool Container::Add(Actor *actor){
	try{
		if(size>0 && inventory.size() >= size){
			return false;
		}
		inventory.push(actor);
		return true;
	}
	catch(...){
		cerr << "An error occurred with Container::Add"  << endl;
		throw 0;
	}
}

void Container::Remove(Actor *actor){
	try{
	inventory.remove(actor);
	}
	catch(...){
		cerr << "An error occurred with Container::Remove" << endl;
		throw 0;
	}
}

void Container::Load(TCODZip &zip){
	try{
		size = zip.getInt();
		int nbActors = zip.getInt();
		while(nbActors > 0){
			Actor *actor = new Actor(0,0,0,nullptr, TCODColor::white);
			actor->Load(zip);
			inventory.push(actor);
			nbActors--;
		}
	}
	catch(...){
		cerr << "An error occurred with Container::Load"  << endl;
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
		cerr << "An error occurred with Container::Save"  << endl;
		throw 0;
	}
}

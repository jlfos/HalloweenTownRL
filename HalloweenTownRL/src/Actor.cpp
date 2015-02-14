#include <iostream>
#include "main.hpp"

using namespace std;

Actor::Actor(int x, int y, int ch, const char *name, const TCODColor &col) :
    x(x),y(y),ch(ch),col(col), name(name),
    blocks(true), attacker(nullptr), destructible(nullptr), ai(nullptr),
    pickable(nullptr), container(nullptr){
}
 
Actor::~Actor(){
	try{
		if(attacker) delete attacker;
		if(destructible) delete destructible;
		if(ai) delete ai;
		if(pickable) delete pickable;
		if(container) delete container;
	}
	catch(...){
		cerr << "An error occurred with Actor::~Actor"  << endl;
		throw 0;
	}
}
void Actor::render() const {
	try{
		TCODConsole::root->setChar(x,y,ch);
		TCODConsole::root->setCharForeground(x,y,col);
	}
	catch(...){
		cerr << "An error occurred with Actor::render"  << endl;
		throw 0;
	}
}

void Actor::update(){
	try{
		if( ai ) ai->update(this);
	}
	catch(...){
		cerr << "An error occurred with Actor::update"  << endl;
		throw 0;
	}
}

void Actor::save(TCODZip &zip){
	try{
		zip.putInt(x);
		zip.putInt(y);
		zip.putInt(ch);
		zip.putColor(&col);
		zip.putString(name);
		zip.putInt(blocks);
		zip.putInt(attacker !=  nullptr);
		zip.putInt(destructible != nullptr);
		zip.putInt(ai != nullptr);
		zip.putInt(pickable != nullptr);
		zip.putInt(container != nullptr);
		if(attacker) attacker->save(zip);
		if(destructible) destructible->save(zip);
		if(ai)ai->save(zip);
		if(pickable)pickable->save(zip);
		if(container)container->save(zip);
	}
	catch(...){
		cerr << "An error occurred with Actor::save"  << endl;
		throw 0;
	}
}


void Actor::load(TCODZip &zip){
	try{
		x=zip.getInt();
		y=zip.getInt();
		ch=zip.getInt();
		col=zip.getColor();
		name=strdup(zip.getString());
		blocks=zip.getInt();
		bool hasAttacker = zip.getInt();
		bool hasDestructible = zip.getInt();
		bool hasAi = zip.getInt();
		bool hasPickable = zip.getInt();
		bool hasContainer = zip.getInt();
		if(hasAttacker){
			attacker = new Attacker(0.0f);
			attacker->load(zip);
		}
		if(hasDestructible){
			destructible = Destructible::create(zip);
		}
		if(hasAi){
			ai = Ai::create(zip);
		}
		if(hasPickable){
			pickable = Pickable::create(zip);
		}
		if(hasContainer){
			container = new Container(0);
			container->load(zip);
		}
	}
	catch(...){
		cerr << "An error occurred with Actor::load"  << endl;
		throw 0;
	}
}

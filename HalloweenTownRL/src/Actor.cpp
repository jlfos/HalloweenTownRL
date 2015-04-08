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
void Actor::Render() const {
	try{
		TCODConsole::root->setChar(x,y,ch);
		TCODConsole::root->setCharForeground(x,y,col);
	}
	catch(...){
		cerr << "An error occurred with Actor::Render"  << endl;
		throw 0;
	}
}

void Actor::Update(){
	try{
		if( ai ) ai->Update(this);
	}
	catch(...){
		cerr << "An error occurred with Actor::Update"  << endl;
		throw 0;
	}
}

void Actor::Save(TCODZip &zip){
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
		if(attacker) attacker->Save(zip);
		if(destructible) destructible->Save(zip);
		if(ai)ai->Save(zip);
		if(pickable)pickable->Save(zip);
		if(container)container->Save(zip);
	}
	catch(...){
		cerr << "An error occurred with Actor::Save"  << endl;
		throw 0;
	}
}


void Actor::Load(TCODZip &zip){
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
			attacker->Load(zip);
		}
		if(hasDestructible){
			destructible = Destructible::Create(zip);
		}
		if(hasAi){
			ai = Ai::Create(zip);
		}
		if(hasPickable){
			pickable = Pickable::Create(zip);
		}
		if(hasContainer){
			container = new Container(0);
			container->Load(zip);
		}
	}
	catch(...){
		cerr << "An error occurred with Actor::Load"  << endl;
		throw 0;
	}
}

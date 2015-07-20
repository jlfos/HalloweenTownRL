#include <iostream>
#include <string>
#include "Actor.hpp"
#include "../Ai/Ai.hpp"
#include "../Attacker.hpp"
#include "../Container.hpp"
#include "../Destructible/Destructible.hpp"
#include "../Engine.hpp"
#include "../UI/Gui.hpp"
#include "../Item/Item.hpp"

Actor::Actor(int x, int y, int ch, std::string name, const TCODColor &col) :
    x(x),y(y),ch(ch),col(col), name(name),
    blocks(true), attacker(nullptr), destructible(nullptr), ai(nullptr),
    item(nullptr), container(nullptr), lightsource(nullptr){
}
 
Actor::~Actor(){
	try{
		if(attacker) delete attacker;
		if(destructible) delete destructible;
		if(ai) delete ai;
		if(item) delete item;
		if(container) delete container;
		if(lightsource) delete lightsource;
	}
	catch(...){
		std::cerr << "An error occurred with Actor::~Actor"  << std::endl;
		throw 0;
	}
}
void Actor::Render() const {
	try{
		engine.gui->setCharAdjusted(x, y, ch);
		engine.gui->setForegroundAdjusted(x, y, col);
	}
	catch(...){
		std::cerr << "An error occurred with Actor::Render"  << std::endl;
		throw 0;
	}
}

void Actor::Update(){
	try{
		if( ai ) ai->Update(this);
	}
	catch(...){
		std::cerr << "An error occurred with Actor::Update"  << std::endl;
		throw 0;
	}
}

void Actor::Save(TCODZip &zip){
	try{
		zip.putInt(x);
		zip.putInt(y);
		zip.putInt(ch);
		zip.putColor(&col);
		zip.putString(name.c_str());
		zip.putInt(blocks);
		zip.putInt(attacker !=  nullptr);
		zip.putInt(destructible != nullptr);
		zip.putInt(ai != nullptr);
		zip.putInt(item != nullptr);
		zip.putInt(container != nullptr);
		if(attacker) attacker->Save(zip);
		if(destructible) destructible->Save(zip);
		if(ai) ai->Save(zip);
		if(item) item->Save(zip);
		if(container) container->Save(zip);
		if(lightsource) lightsource->Save(zip);
	}
	catch(...){
		std::cerr << "An error occurred with Actor::Save"  << std::endl;
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
			item = Item::Create(zip);
		}
		if(hasContainer){
			container = new Container(0);
			container->Load(zip);
		}
	}
	catch(...){
		std::cerr << "An error occurred with Actor::Load"  << std::endl;
		throw 0;
	}
}

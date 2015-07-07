#include <iostream>

#include "../Actor/Actor.hpp"
#include "../Container.hpp"
#include "../Engine.hpp"
#include "Healer.hpp"
#include "Item.hpp"

bool Item::Pick(Actor *owner, Actor *wearer){
	try{
		if(wearer->container && wearer->container->Add(owner)){
			engine.actors.remove(owner);
			return true;
		}
		return false;
	}
	catch(...){
		std::cerr << "An error occurred with Item::Pick"  << std::endl;
		throw 0;
	}
}

bool Item::Use(Actor *owner, Actor *wearer){
	try
	{
		if(wearer->container){
			if(getType() == ItemType::HEALER){
				wearer->container->Remove(owner);
				delete owner;
			}
			return true;
		}
		return false;
	}
	catch(...){
		std::cerr << "An error occurred with Item::Use"  << std::endl;
		throw 0;
	}
}


Item *Item::Create(TCODZip &zip){
	try{
		ItemType type = (ItemType)zip.getInt();
		Item *Item=nullptr;
		switch(type){
			case ItemType::HEALER: Item = new Healer(0); break;
		}
		Item->Load(zip);
		return Item;
	}
	catch(...){
		std::cerr << "An error occurred with Item::Create"  << std::endl;
		throw 0;
	}
}

Item::ItemType Item::getType(){
	return type;
}

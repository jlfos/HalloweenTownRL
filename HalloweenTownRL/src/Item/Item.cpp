#include <iostream>

#include "../Actor/Actor.hpp"
#include "../Container.hpp"
#include "../Engine.hpp"
#include "Healer.hpp"
#include "Item.hpp"
#include "../LoggerWrapper.hpp"
Item::Item(ItemType type){
	this->type = type;
}


bool Item::Pick(Actor *owner, Actor *wearer){
	try{
		if(wearer->container && wearer->container->Add(owner)){
			engine.actors.remove(owner);
			return true;
		}
		return false;
	}
	catch(...){
		LoggerWrapper::Error("An error occurred with Item::Pick");
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
		LoggerWrapper::Error("An error occurred with Item::Use");
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
		LoggerWrapper::Error("An error occurred with Item::Create");
		throw 0;
	}
}

Item::ItemType Item::getType(){
	return type;
}

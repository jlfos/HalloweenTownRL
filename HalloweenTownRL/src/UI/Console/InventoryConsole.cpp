/*
 * InventoryConsole.cpp
 *
 *  Created on: Jul 11, 2015

 */
#include <vector>
#include <string>
#include <iostream>
#include "libtcod.hpp"
#include "../../Actor/Actor.hpp"
#include "ConsoleFrame.hpp"
#include "ConsoleSelection.hpp"
#include "ConsoleUI.hpp"
#include "../../Container.hpp"
#include "ConsoleUI.hpp"
#include "../../Engine.hpp"
#include "../Gui.hpp"
#include "InventoryConsole.hpp"
#include "../../Item/Item.hpp"
#include "../../LoggerWrapper.hpp"
#include "Message.hpp"
#include "../../Tile/TileColors.hpp"

namespace InventoryConstants{
static const int WIDTH=25;
static const int HEIGHT=28;
}

InventoryConsole::InventoryConsole():
	ConsoleUI(InventoryConstants::WIDTH, InventoryConstants::HEIGHT, engine.screenWidth/2 - InventoryConstants::WIDTH/2, engine.screenHeight/2 - InventoryConstants::HEIGHT/2)
{

	frame = new ConsoleFrame("Inventory", TileColors::white);
	try{
		std::vector<ConsoleLine*> cl;

		player = engine.player;
		char charIndex = 'a';

		//TODO Create a generic function to generate messages and takes a generic indexer. You'll have to switch from TCODList or commit to it.
		for(Actor* item : player->container->inventory){
			std::string stuff = "(";
			Message message(stuff.append(1, charIndex) + ") " + item->name, TileColors::white);
			ConsoleLine *line = new ConsoleLine(message);
			cl.push_back(line);
			charIndex++;
		}
		selection = new ConsoleSelection(cl.size(), TileColors::blue);
		PopulateKeyMapping();
		SetConsoleLines(cl);
		Display();
		Flush();
		UserInput();
	}
	catch(...){
		LoggerWrapper::Error("An error occurred in InventoryConsole::InventoryConsole()");
		throw 0;
	}
}


void InventoryConsole::UserInput(){
	try{
	bool inventoryMode = true;
	TCOD_key_t lastKey;
		while(inventoryMode){
			TCODSystem::checkForEvent(TCOD_EVENT_KEY_PRESS, &lastKey, NULL);
			switch(lastKey.vk){
			case TCODK_UP:
				if(selection){
					if(selection->decrementSelection()){
						Display();
						Flush();
					}
				}
				break;
			case TCODK_DOWN:
				if(selection){
					if(selection->incrementSelection()){
						Display();
						Flush();
					}
				}
				break;
			case TCODK_ENTER:
			{
				if(selection){
					UseItem(GetItem(selection->getSelection()-1));
					inventoryMode = false;
				}

			}
			break;
				break;
			case TCODK_ESCAPE :inventoryMode = false; break;

			case TCODK_CHAR: {
				int mapping = keyMapping[lastKey.c];

				if(mapping < player->container->inventory.size()){
					inventoryMode = false;
					UseItem(GetItem(mapping));
				}

			}
			break;

			case TCODK_SPACE: {

				if(selection){
					Actor* item = GetItem(selection->getSelection()-1);

					if(item!= nullptr){
						std::string itemName = item->name;
						unsigned int descriptionWidth = 30;
						std::string m = engine.GetItemDescription(itemName);
						//TODO remove this hard coded '5'. Try to get the size of the map portion of the screen OR subtract the size of the player UI console from the startY value
						ConsoleUI description(m, descriptionWidth, engine.screenWidth/2, engine.screenHeight/2 - 5);
						description.frame = new ConsoleFrame(item->name, TileColors::white);
						description.Display();
						description.Flush();
						bool descriptionMode = true;
						while(descriptionMode){
							TCODSystem::checkForEvent(TCOD_EVENT_KEY_PRESS, &lastKey, NULL);
							switch(lastKey.vk){
							case TCODK_ESCAPE:{
								descriptionMode = false;
								break;
							}
							default:
								break;
							}

						}
						description.Clear();
						Display();
						Flush();
					}
				}
			}
			break;
			default:

					break;
			}
		}
	}
	catch(...){
		LoggerWrapper::Error("An error occurred in InventoryConsole::UserInput()");
		throw 0;
	}
}

Actor* InventoryConsole::GetItem(int itemIndex){
	try{
		if(itemIndex>=player->container->inventory.size()){
			return nullptr;
		}
		Actor* selectedItem = player->container->inventory.get(itemIndex);
		if(selectedItem==nullptr){
			LoggerWrapper::Error("Selected item was null");
			throw 0;
		}
		return selectedItem;
	}
	catch(...){
		LoggerWrapper::Error("An error occurred in InventoryConsole::GetItem");
		throw 0;
	}
}

void InventoryConsole::UseItem(Actor* item){
	try {
		if(item != nullptr){
			item->item->Use(item, player);
			engine.gameStatus=Engine::NEW_TURN;
		}
	}
	catch (...) {
		LoggerWrapper::Error("An error occurred in InventoryConsole::UseItem");
		throw 0;
	}
}

void InventoryConsole::PopulateKeyMapping(){
	try {
		char key = 'a';
		for(int i = 0; i < player->container->size; i++, key++){
			keyMapping[key] = i;
		}
	}
	catch (...) {
		LoggerWrapper::Error("An error occurred in InventoryConsole::PopulateKeyMapping");
		throw 0;
	}
}

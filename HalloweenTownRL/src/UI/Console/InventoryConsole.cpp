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
		populateKeyMapping();
		setConsoleLines(cl);
		display();
		flush();
		userInput();
	}
	catch(...){
		std::cerr << "An error occurred in InventoryConsole::InventoryConsole()"  << std::endl;
	}
}


void InventoryConsole::userInput(){
	try{
	bool inventoryMode = true;
	TCOD_key_t lastKey;
		while(inventoryMode){
			TCODSystem::checkForEvent(TCOD_EVENT_KEY_PRESS, &lastKey, NULL);
			switch(lastKey.vk){
			case TCODK_UP:
				if(selection){
					if(selection->decrementSelection()){
						display();
						flush();
					}
				}
				break;
			case TCODK_DOWN:
				if(selection){
					if(selection->incrementSelection()){
						display();
						flush();
					}
				}
				break;
			case TCODK_ENTER:
			{
				if(selection){
					useItem(getItem(selection->getSelection()-1));
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
					useItem(getItem(mapping));
				}

			}
			break;

			case TCODK_SPACE: {

				if(selection){
					Actor* item = getItem(selection->getSelection()-1);

					if(item!= nullptr){
						std::string itemName = item->name;
						unsigned int descriptionWidth = 30;
						std::string m = engine.getItemDescription(itemName);
						//TODO remove this hard coded '5'. Try to get the size of the map portion of the screen OR subtract the size of the player UI console from the startY value
						ConsoleUI description(m, descriptionWidth, engine.screenWidth/2, engine.screenHeight/2 - 5);
						description.frame = new ConsoleFrame(item->name, TileColors::white);
						description.display();
						description.flush();
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
						description.clear();
						display();
						flush();
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
		std::cerr << "An error occurred in InventoryConsole::userInput()" << std::endl;
		throw 0;
	}
}

Actor* InventoryConsole::getItem(int itemIndex){
	try{
		if(itemIndex>=player->container->inventory.size()){
//			std::cerr << "The item index " +std::to_string(itemIndex) + " is out of bounds" << std::endl;
			return nullptr;
		}
		Actor* selectedItem = player->container->inventory.get(itemIndex);
		if(selectedItem==nullptr){
			std::cerr << "Selected item was null" << std::endl;
			throw 0;
		}
		return selectedItem;
	}
	catch(...){
		std::cerr << "An error occurred in InventoryConsole::getItem" << std::endl;
		throw 0;
	}
}

void InventoryConsole::useItem(Actor* item){
		if(item != nullptr){
			item->item->Use(item, player);
			engine.gameStatus=Engine::NEW_TURN;
		}
}

void InventoryConsole::populateKeyMapping(){
	char key = 'a';
	for(int i = 0; i < player->container->size; i++, key++){
		keyMapping[key] = i;
	}

}

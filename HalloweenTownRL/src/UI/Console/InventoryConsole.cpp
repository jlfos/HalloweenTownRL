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
static const int WIDTH=50;
static const int HEIGHT=28;
}

InventoryConsole::InventoryConsole():
	ConsoleUI(InventoryConstants::WIDTH, InventoryConstants::HEIGHT, engine.screenWidth/2 - InventoryConstants::WIDTH/2, engine.screenHeight/2 - InventoryConstants::HEIGHT/2, true, "inventory")
{

	try{
		std::vector<ConsoleLine*> cl;



		player = engine.player;
		char charIndex = 'a';

		for(Actor* item : player->container->inventory){
			std::string stuff = "(";
			Message message(stuff.append(1, charIndex) + ") " + item->name, TileColors::white, TileColors::grey);
			ConsoleLine *line = new ConsoleLine(message);
			cl.push_back(line);
			charIndex++;
		}
		populateKeyMapping();
		setConsoleLines(cl);
		display();
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
				if(decrementSelection())
					display();
				break;
			case TCODK_DOWN:
				if(incrementSelection())
					display();
				break;
			case TCODK_ENTER:
			{
				getItem(getSelection()-1);
				inventoryMode = false;
			}
			break;
				break;
			case TCODK_ESCAPE :inventoryMode = false; break;

			case TCODK_CHAR: {
				int mapping = keyMapping[lastKey.c];

				if(mapping < player->container->inventory.size()){
					inventoryMode = false;
					getItem(mapping);
				}

			}
			break;

			case TCODK_SPACE: {
				std::string knife = "knife";
				ConsoleUI description(20, 16, 30, 20, true, knife);
				std::string m = "A blade with a handle attached. What more do you need?";
				std::vector<Message> messages = Gui::wordWrapText(m, 20);
//				ConsoleLine* cl = new ConsoleLine(message);
				std::vector<ConsoleLine*> cls;
				for(Message m : messages){
					ConsoleLine* cl = new ConsoleLine(m);
					cls.push_back(cl);
				}


				description.setConsoleLines(cls);
				description.display();
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

void InventoryConsole::getItem(int itemIndex){
	try{
		if(itemIndex>=player->container->inventory.size()){
			std::cerr << "The item index " +std::to_string(itemIndex) + " is out of bounds" << std::endl;
			throw 0;
		}
		Actor* selectedItem = player->container->inventory.get(itemIndex);
		if(selectedItem==nullptr){
			std::cerr << "Selected item was null" << std::endl;
			throw 0;
		}
		selectedItem->item->Use(selectedItem, player);
		engine.gameStatus=Engine::NEW_TURN;
	}
	catch(...){
		std::cerr << "An error occurred in InventoryConsole::getItem" << std::endl;
	}
}

void InventoryConsole::populateKeyMapping(){
	char key = 'a';
	for(int i = 0; i < player->container->inventory.size(); i++, key++){
		keyMapping[key] = i;
	}

}

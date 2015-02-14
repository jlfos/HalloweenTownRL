#include <iostream>
#include "../main.hpp"

using namespace std;

Menu::~Menu(){
	try{
		clear();
	}
	catch(...){
		cerr << "An error occurred with Menu::~Menu"  << endl;
		throw 0;
	}
}

void Menu::clear(){
	try{
		items.clearAndDelete();
	}
	catch(...){
		cerr << "An error occurred with Menu::clear"  << endl;
		throw 0;
	}
}

void Menu::populateMenu(bool saveGameExists){
	try{
		clear();
		addItem(MenuItemCode::NEW_GAME, "New Game");
		if(saveGameExists){
			addItem(MenuItemCode::CONTINUE, "Continue");
		}
		addItem(MenuItemCode::EXIT, "Exit");
	}
	catch(...){
		cerr << "An error occurred with Menu::populateMenu"  << endl;
		throw 0;
	}
}

void Menu::addItem(MenuItemCode code, const char *label){
	try{
		MenuItem *item = new MenuItem();
		item->code = code;
		item->label = label;
		items.push(item);
	}
	catch(...){
		cerr << "An error occurred with Menu::addItem"  << endl;
		throw 0;
	}
}

Menu::MenuItemCode Menu::pick(){
	try{
		static TCODImage img("temp.png");
		int selectedItem = 0;
		while(!TCODConsole::isWindowClosed() ){
			img.blit2x(TCODConsole::root, 0, 0);
			int currentItem = 0;
			for(MenuItem* it: items){
				if(currentItem == selectedItem){
					TCODConsole::root->setDefaultForeground(TCODColor::lighterOrange);
				}
				else{
					TCODConsole::root->setDefaultForeground(TCODColor::lightGrey);
				}
				TCODConsole::root->print(10, 10+currentItem*3, (it)->label);
				currentItem++;
				TCODConsole::flush();

				TCOD_key_t key;
				TCODSystem::checkForEvent(TCOD_EVENT_KEY_PRESS, &key, nullptr);
				switch(key.vk){
					case TCODK_UP:
						selectedItem--;
						if(selectedItem < 0){
							selectedItem = items.size() - 1;
						}
						break;
					case TCODK_DOWN:
						selectedItem = (selectedItem + 1) % items.size();
						break;
					case TCODK_ENTER :
						return items.get(selectedItem)->code;
					default:
						break;

				}

			}
		}
		return NONE;
	}
	catch(...){
		cerr << "An error occurred with Menu::pick"  << endl;
		throw 0;
	}
}

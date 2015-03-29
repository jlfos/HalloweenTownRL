#include <iostream>
#include "../main.hpp"

using namespace std;

Menu::Menu(){
	try{

	}
	catch(...){
		cerr << "An error occurred with Menu::Menu" << endl;
		throw 0;
	}
}

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
		items.clear();
	}
	catch(...){
		cerr << "An error occurred with Menu::clear"  << endl;
		throw 0;
	}
}


void Menu::addItem(string item){
	try{
		items.push(item);
	}
	catch(...){
		cerr << "An error occurred with Menu::addItem"  << endl;
		throw 0;
	}
}

void Menu::setHeader(string header){
	this->header = header;
}

string Menu::pick(){
	try{
//		static TCODImage img("tree.png");
		int selectedItem = 0;
		TCODConsole::root->clear();
		TCODConsole::root->setDefaultForeground(TCODColor::darkOrange);
		TCODConsole::root->printEx(10, 6, TCOD_bkgnd_flag_t::TCOD_BKGND_COLOR_BURN, TCOD_alignment_t::TCOD_LEFT, header.c_str());
		while(!TCODConsole::isWindowClosed()){
//			img.blit2x(TCODConsole::root, 0, 0);
			int currentItem = 0;
			for(string it: items){
				if(currentItem == selectedItem){
					TCODConsole::root->setDefaultForeground(TCODColor::lighterOrange);
				}
				else{
					TCODConsole::root->setDefaultForeground(TCODColor::lightGrey);
				}
				TCODConsole::root->print(10, 15+currentItem*3, it.c_str());
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
						return items.get(selectedItem);
					default:
						break;

				}

			}
		}
		return "NONE";
	}
	catch(...){
		cerr << "An error occurred with Menu::pick"  << endl;
		throw 0;
	}
}

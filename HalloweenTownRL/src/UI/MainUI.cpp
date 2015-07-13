#include <iostream>
#include "MenuUI.hpp"
#include "../Tile/TileColors.hpp"

MenuUI::MenuUI() {
	try {

	} catch (...) {
		std::cerr << "An error occurred with Menu::Menu" << std::endl;
		throw 0;
	}
}

MenuUI::~MenuUI() {
	try {
		Clear();
	} catch (...) {
		std::cerr << "An error occurred with Menu::~Menu" << std::endl;
		throw 0;
	}
}

void MenuUI::Clear() {
	try {
		items.clear();
	} catch (...) {
		std::cerr << "An error occurred with Menu::clear" << std::endl;
		throw 0;
	}
}

void MenuUI::AddItem(std::string item) {
	try {
		items.push(item);
	} catch (...) {
		std::cerr << "An error occurred with Menu::addItem" << std::endl;
		throw 0;
	}
}

void MenuUI::SetHeader(std::string header) {
	this->header = header;
}

std::string MenuUI::Pick() {
	try {
		//static TCODImage img("tree.png");
		int selectedItem = 0;
		TCODConsole::root->clear();
		TCODConsole::root->setDefaultForeground(TileColors::darkOrange);
		TCODConsole::root->printEx(10, 6,
				TCOD_bkgnd_flag_t::TCOD_BKGND_COLOR_BURN,
				TCOD_alignment_t::TCOD_LEFT, header.c_str());
		while (!TCODConsole::isWindowClosed()) {
			//img.blit2x(TCODConsole::root, 0, 0);
			int currentItem = 0;
			for (std::string it : items) {
				if (currentItem == selectedItem) {
					TCODConsole::root->setDefaultForeground(
							TileColors::lighterOrange);
				} else {
					TCODConsole::root->setDefaultForeground(
							TileColors::lightGrey);
				}
				TCODConsole::root->print(10, 15 + currentItem * 3, it.c_str());
				currentItem++;
				TCODConsole::flush();

				TCOD_key_t key;
				TCODSystem::checkForEvent(TCOD_EVENT_KEY_PRESS, &key, nullptr);
				switch (key.vk) {
				case TCODK_UP:
					selectedItem--;
					if (selectedItem < 0) {
						selectedItem = items.size() - 1;
					}
					break;
				case TCODK_DOWN:
					selectedItem = (selectedItem + 1) % items.size();
					break;
				case TCODK_ENTER:
					return items.get(selectedItem);
				default:
					break;

				}

			}
		}
		return "NONE";
	} catch (...) {
		std::cerr << "An error occurred with Menu::pick" << std::endl;
		throw 0;
	}
}



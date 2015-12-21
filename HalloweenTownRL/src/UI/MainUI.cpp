#include <iostream>
#include "../LoggerWrapper.hpp"
#include "MenuUI.hpp"
#include "../Tile/TileColors.hpp"

MenuUI::MenuUI() {
	try {

	}
	catch (...) {
		LoggerWrapper::Error("An error occurred with Menu::Menu");
		throw 0;
	}
}

MenuUI::~MenuUI() {
	try {
		Clear();
	}
	catch (...) {
		LoggerWrapper::Error("An error occurred with Menu::~Menu");
		throw 0;
	}
}

void MenuUI::Clear() {
	try {
		items.clear();
	}
	catch (...) {
		LoggerWrapper::Error("An error occurred with Menu::Clear");
		throw 0;
	}
}

void MenuUI::AddItem(std::string item) {
	try {
		items.push(item);
	}
	catch (...) {
		LoggerWrapper::Error("An error occurred with Menu::addItem");
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
		TCODConsole::root->setDefaultForeground(TileColors::orangeDark);
		TCODConsole::root->printEx(10, 6,
				TCOD_bkgnd_flag_t::TCOD_BKGND_COLOR_BURN,
				TCOD_alignment_t::TCOD_LEFT, header.c_str());
		while (!TCODConsole::isWindowClosed()) {
			//img.blit2x(TCODConsole::root, 0, 0);
			int currentItem = 0;
			for (std::string it : items) {
				if (currentItem == selectedItem) {
					TCODConsole::root->setDefaultForeground(
							TileColors::orangeLighter);
				} else {
					TCODConsole::root->setDefaultForeground(
							TileColors::greyLight);
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
	}
	catch (...) {
		LoggerWrapper::Error("An error occurred with Menu::pick");
		throw 0;
	}
}



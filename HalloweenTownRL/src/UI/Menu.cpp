#include "../main.hpp"

Menu::~Menu(){
	clear();
}

void Menu::clear(){
	items.clearAndDelete();
}

void Menu::addItem(MenuItemCode code, const char *label){
	MenuItem *item = new MenuItem();
	item->code = code;
	item->label = label;
	items.push(item);
}

Menu::MenuItemCode Menu::pick(){
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

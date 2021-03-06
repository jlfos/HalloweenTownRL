/*
 * ScreenUI.cpp

 *
 *  Created on: Jul 18, 2015
 */
#include "libtcod.hpp"
#include "../LoggerWrapper.hpp"
#include "ScreenUI.hpp"

ScreenUI::ScreenUI() {
}

ScreenUI::~ScreenUI(){

}

void ScreenUI::Show() {
	try {
		int y=1;
		TCODConsole::root->clear();
		float colorCoef=0.4f;
		for(Message message : (log)) {
				TCODConsole::root->setDefaultForeground(message.getBackgroundColor() * colorCoef);
				TCODConsole::root->print(1, y, message.getText().c_str(), message.getBackgroundColor());
				y++;
		}
		TCODConsole::root->flush();

		bool screeMode = true;
		TCOD_key_t lastKey;
		while (screeMode) {
			TCODSystem::checkForEvent(TCOD_EVENT_KEY_PRESS, &lastKey, NULL);
			switch (lastKey.vk) {
			case TCODK_ESCAPE:
				screeMode = false;
		//				engine.Load();
				break;
			default:
				break;
			}
		}
	}
	catch (...) {
		LoggerWrapper::Error("An error occurred in ScreenUI::Show");
		throw 0;
	}
}

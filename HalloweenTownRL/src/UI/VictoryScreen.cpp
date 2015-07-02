/*
 * VictoryScreen.cpp
 *
 *  Created on: Jul 2, 2015
 *      Author: josh
 */
#include <string>
#include "libtcod.hpp"
#include "../Engine.hpp"
#include "../Tile/TileColors.hpp"
#include "VictoryScreen.hpp"

VictoryScreen::VictoryScreen(){
	text = "As the monster in front of you dies, you watch as the world melts around you. You reappear back in the real world, as if you had never left it.";
}

void VictoryScreen::show(){
	int y=1;
		TCODConsole::root->clear();

		float colorCoef=0.4f;
		TCODConsole::root->setDefaultForeground(TileColors::white * colorCoef);
		TCODConsole::root->printRect(1, y, 80, 50, "%s", text.c_str());
		TCODConsole::root->flush();

		bool logMode = true;
		TCOD_key_t lastKey;
		while (logMode) {
			TCODSystem::checkForEvent(TCOD_EVENT_KEY_PRESS, &lastKey, NULL);
			switch (lastKey.vk) {
			case TCODK_ESCAPE:
				logMode = false;
				engine.Load();
				break;
			default:
				break;
			}
		}
}





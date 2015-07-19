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
	log = new std::vector<Message>(Message::wordWrapText(text, engine.screenWidth));
}






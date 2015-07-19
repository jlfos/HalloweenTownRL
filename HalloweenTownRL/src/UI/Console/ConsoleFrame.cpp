/*
 * ConsoleFrame.cpp
 *
 *  Created on: Jul 18, 2015
 */
#include <string>
#include "libtcod.hpp"
#include "ConsoleFrame.hpp"


ConsoleFrame::ConsoleFrame(std::string frameTitle, TCODColor frameColor): frameTitle(frameTitle), frameColor(frameColor){

}

TCODColor ConsoleFrame::getFrameColor() {
	return frameColor;
}

std::string ConsoleFrame::getFrameTitle() {
	return frameTitle;
}





